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

#include "common/ArgParse.h"

#include "lib/BGMasterClientApi.h"
#include "lib/exceptions.h"

#include <utility/include/Exec.h>
#include <utility/include/Log.h>

#include <boost/tokenizer.hpp>

#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>

LOG_DECLARE_FILE( "master" );

BGMasterClient client;
Args* pargs;

pid_t
startMaster(
        std::string& path,
        std::string& args
        )
{
    std::string path_and_args = path + args;
    int pipefd;
    std::string error;
    // Can we check to see if the lock file gets updated before exiting?
    return Exec::fexec(pipefd, path_and_args, error, false);
}

void
doStart(
        std::string& target,
        std::string location=""
        )
{
    std::string commandstring = "";
    if (target.length() != 0) {
        try {
            commandstring = pargs->get_props()->getValue("master.binmap", target);
        } catch (std::invalid_argument& e) {
            std::cerr << "Properties file error, invalid alias found. Error is: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(" ");
    tokenizer tok(commandstring, sep);

    int i = 0;
    std::string path="";
    std::string arguments="";
    for (tokenizer::iterator beg=tok.begin(); beg!=tok.end();++beg){
        if (i == 0) {
            path = (*beg);
        } else {
            arguments += (*beg);
            arguments += " ";
        }
        ++i;
    }

    std::string st;

    pid_t child = 0;
    if (target == "bgmaster_server" || target == "bgmaster") { // bgmaster_server
        std::string propstr = "--properties";
        std::string properties = (*pargs)[propstr];
        if (properties.length() != 0)
            arguments += " --properties " + properties;
        child = startMaster(path, arguments);
        int exit_status = 0;
        if (child != 0)
            waitpid(child, &exit_status, 0);
    } else {
        LOG_DEBUG_MSG("Sending start message " << path << " " << arguments << " for " << target);
        BGAgentId id(location);
        BinaryId started_id;
        try {
            started_id = client.start(target, &id);
        } catch(exceptions::BGMasterError& e) {
            std::cerr << "Unable to start all selected binaries. Error is: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
        st = started_id;
    }

    if (child) {
        std::cout << "Started " << target << ":" << ++child << std::endl;
    } else {
        std::cout << "Started " << target << ":" << st << std::endl;
    }
}

void
help()
{
    std::cerr << "Starts a controlled process or bgmaster_server." << std::endl;
    std::cerr << "Administrative authority required." << std::endl;
}

void
usage()
{
    std::cerr << "master_start [ alias ] | [ \"bgmaster\" ] [ \"binaries\" ] [ --properties filename ] [ --help ] [ --host host:port ] [ --verbose verbosity ]" << std::endl;
}

int main(int argc, const char** argv)
{
    std::vector<std::string> validargs;
    std::vector<std::string> singles;
    validargs.push_back("*"); // One argument without a "--" is allowed
    Args largs(argc, argv, &usage, &help, validargs, singles);
    pargs = &largs;
    client.initProperties(pargs->_props);

    // See if "bgmaster" is specified
    bool bgmaster = false;
    bool binaries = false;
    for (Args::iterator it = pargs->begin();
        it != pargs->end(); ++it) {
        if (*it == "bgmaster") {
            bgmaster = true;
        }
        if (*it == "binaries") {
            binaries = true;
        }
    }

    std::string s = "";
    if (bgmaster)
        s = "bgmaster_server";
    else if(binaries) {
        s = "";
    }
    else {
        if (pargs->size() != 0)
            s = *(pargs->begin());
        else {
            usage();
            exit(1);
        }
    }

    if (s != "bgmaster_server") {
        try {
            client.connectMaster(pargs->get_portpairs());
        }
        catch(exceptions::BGMasterError& e) {
            std::cerr << "Unable to contact bgmaster_server, server may be down." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    doStart(s);
}