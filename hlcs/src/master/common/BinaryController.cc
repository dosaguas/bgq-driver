/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2011                              */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under the                      */
/* Eclipse Public License (EPL).                                    */
/*                                                                  */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#include "BinaryController.h"
#include "Ids.h"

#include "../lib/exceptions.h"

#include <utility/include/Exec.h>
#include <utility/include/Log.h>

#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/scope_exit.hpp>
#include <boost/tokenizer.hpp>

#include <sstream>
#include <string>

#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

LOG_DECLARE_FILE("master");

#define LOGGING_DECLARE_ID_MDC(value) \
    log4cxx::MDC _location_mdc( "ID", std::string("{") + boost::lexical_cast<std::string>(value) + "} " );

extern volatile int signal_number;

void
switchBackUID(
        uid_t my_euid,
        gid_t my_egid
        )
{
    LOG_TRACE_MSG(__FUNCTION__);
    // Done. Go back to our user, but do it through root.
    int rc = seteuid(0); // Go back to root while we do this.
    if (rc != 0) {
        char errorText[256];
        LOG_ERROR_MSG("Cannot change uid back to 'root'. Current uid is " << geteuid() << ". " << std::string(strerror_r(errno, errorText, 256)));
    }

    rc = setegid(0);
    if (rc != 0) {
        char errorText[256];
        LOG_ERROR_MSG("Could not set effective gid back to 'root': " << std::string(strerror_r(errno, errorText, 256)));
    }

    rc = setegid(my_egid);
    if (rc != 0) {
        char errorText[256];
        LOG_ERROR_MSG("Could not set effective gid back to " << my_egid << ": " << std::string(strerror_r(errno, errorText, 256)));
    }

    rc = seteuid(my_euid);
    if (rc != 0) {
        char errorText[256];
        LOG_ERROR_MSG("Cannot reset effective uid to " << my_euid << ". Current euid is " << geteuid() << ".  " << std::string(strerror_r(errno, errorText, 256)));
    }
    LOG_DEBUG_MSG("Current uid: " << geteuid() << " Current gid: " << getegid());
}

BinaryId
BinaryController::startBinary(
        const std::string& user_list
        )
{
    //LOGGING_DECLARE_ID_MDC(_binid.str());
    LOG_TRACE_MSG(__FUNCTION__);
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    // Pull out the full path and see if it exists
    boost::char_separator<char> sep(" ");
    tokenizer nametok(_binary_bin_path, sep);
    tokenizer::iterator b = nametok.begin();
    std::string path = (*b);

    if (_user.length() == 0) {
        LOG_WARN_MSG("No user id defined for alias " << _alias_name << " running under default user.");
    } else {
        if (user_list.find(_user) == std::string::npos) {
            std::ostringstream err;
            err << "Invalid user id " << _user << " specified.";
            LOG_ERROR_MSG(err.str());
            _exec_error = err.str();
            BinaryId badid(-1, "");
            return badid;
        }
    }

    pid_t pid = 0;
    if (boost::filesystem::exists(path)) {
        std::string error;
        int pipefd = 0;
        pid = Exec::fexec(pipefd, _binary_bin_path, error, true, _logfile.c_str(), "", _user);
        LOG_DEBUG_MSG("Returned from fexec for " << _alias_name << " pid=" << pid);
        if (pid < 0) {
            LOG_DEBUG_MSG("Error from exec: " << error);
            _exec_error = error;
            BinaryId badid(pid,"");
            return badid;
        }
    } else {
        std::ostringstream msg;
        msg << "Command or path " << path << " does not exist.";
        LOG_ERROR_MSG(msg.str());
        throw exceptions::FileError(exceptions::WARN, msg.str());
    }

    // If we get this far, we're successful.
    BinaryId child(pid, _host.ip());
    _binid = child;
    if (_user.length() == 0) {
        LOG_INFO_MSG("Started binary id " << _binid.str() << " alias " << _alias_name);
    } else {
        LOG_INFO_MSG("Started binary id " << _binid.str() << " alias " << _alias_name << " under user " << _user);
    }
    return _binid;
}

bool
isRunning(
        pid_t pid
        )
{
    LOG_TRACE_MSG(__FUNCTION__);
    bool retval = false;
    if (pid != 0) {
        int rc = kill(pid, 0);	// send a 0 signal just to test the pid
        if (rc == 0) {			// normal return implies process is running
            retval = true;
        } else {
            if (errno != ESRCH) {		// ESRCH is normal for a terminated process
                char errorText[256];
                LOG_INFO_MSG("isRunning() failed, assuming process still running. " << std::string(strerror_r(errno, errorText, 256)));
                retval = true;
            }
        }
    }
    return retval;
}

int
BinaryController::stopBinary(
        int signal
        )
{
    LOG_TRACE_MSG(__FUNCTION__);
    // This is how this works:
    // signal_number is a process global variable that may be set when we catch a signal and want to propagate it
    // to the child processes. So, we check that first and if it is set, we send it to the child processes.  If it
    // ISN'T set, then we use the signal passed to us as the initial signal... unless it is unspecified (zero).
    // In that case, we default to SIGTERM. After we try our initial signal, we wait a bit and start sending
    // SIGKILLs to REALLY kill it.
    LOGGING_DECLARE_ID_MDC(_binid.str());
    LOG_DEBUG_MSG("Stopping binary " << _binid.str());

    stopping(true);
    int pid = _binid.get_pid();
    uid_t my_euid = geteuid();
    gid_t my_egid = getegid();

    bool isroot = false;

    if (pid != 0) {
        // Switch back to root so we can switch to assigned user.
        if (_user.length() != 0) {
            int rc = seteuid(0); // Go back to root while we do this.
            if (rc != 0) {
                char errorText[256];
                LOG_ERROR_MSG("Cannot change uid to root. Current uid is " << geteuid() << ". " << std::string(strerror_r(errno, errorText, 256)));
            } else {
                isroot = true;
                LOG_INFO_MSG("Changed effective uid to 'root'.");
            }
        }

        if (_user.length() != 0 && isroot) {
            struct passwd* my_entry;
            my_entry = getpwnam(_user.c_str());
            if (my_entry == NULL) {
                LOG_ERROR_MSG("Could not get password entry for " << _user << ". Check that " << _user << " is a valid user on this system.");
            } else {
                int rc = setegid(my_entry->pw_uid);
                if (rc != 0) {
                    char errorText[256];
                    LOG_ERROR_MSG("Could not set real and effective gids of this process: " << std::string(strerror_r(errno, errorText, 256)));
                }

                rc = seteuid(my_entry->pw_uid);
                if (rc != 0) {
                    char errorText[256];
                    LOG_ERROR_MSG("Could not set real and effective uids of this process: " << std::string(strerror_r(errno, errorText, 256)));
                }
            }
        }

        BOOST_SCOPE_EXIT( (&my_euid) (&my_egid) ) {
            switchBackUID(my_euid, my_egid);
        } BOOST_SCOPE_EXIT_END;

        LOG_DEBUG_MSG("Running as " << geteuid() << " to kill " << pid << " with user " << _user);

        const boost::posix_time::ptime kill_sent_time = boost::posix_time::second_clock::local_time();
        unsigned timeout = 25000;
        bool signalSent = false;
        while (isRunning(pid)) {
            if (signal_number == 0) {
                if (signalSent == false) {
                    if (signal == 0) {
                        signal = SIGTERM;
                        LOG_DEBUG_MSG("Killing " << pid << " with a SIGTERM");
                        int killrc = ::kill(pid, SIGTERM);
                        if (killrc < 0) {
                            LOG_WARN_MSG("Failed to kill " << pid << " with SIGTERM.");
                        }
                        signalSent = true;
                    } else {
                        LOG_DEBUG_MSG("Killing " << pid << " with signal " << signal);
                        int killrc = ::kill(pid, signal);
                        if (killrc < 0) {
                            LOG_WARN_MSG("Failed to kill " << pid << " with signal " << signal);
                        }
                        signalSent = true;
                    }
                }
            } else {
                LOG_DEBUG_MSG("Killing " << pid << " with signal " << signal_number);
                signal = signal_number;
                int killrc = ::kill(pid, signal_number);
                if (killrc < 0) {
                    LOG_WARN_MSG("Failed to kill " << pid << " with signal " << signal);
                }
                signalSent = true;
            }

            const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
            const boost::posix_time::time_duration td = now - kill_sent_time;

            if (td.total_seconds() > 300) { // Five minute timeout.
                LOG_INFO_MSG("Initial signal failed. Killing " << pid << " with a SIGKILL.");
                signal = SIGKILL;
                int killrc = ::kill(pid, SIGKILL);
                if (killrc < 0) {
                    LOG_ERROR_MSG("Failed to kill " << pid << " with SIGKILL.");
                }
                break;
            }
            usleep(timeout);
            timeout *= 2;
            // Max the sleep period to 5 seconds
            if (timeout > 5000000) {
                timeout = 5000000;
            }
        }
    }
    return signal;
}