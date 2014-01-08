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
/*!
 * \page job_status
 *
 * Displays BG/Q job information
 *
 * \section SYNOPSIS
 *
 * job_status id [OPTIONS]
 *
 * \section DESCRIPTION
 *
 * This documentation was generated for driver DRIVER_NAME with revision VERSION_NUMBER on DOCUMENTATION_DATE.
 *
 * Lists job information.
 *
 * \section OPTIONS
 *
 * \subsection id --id
 *
 * job ID
 *
 * \subsection details --details
 *
 * Display detailed information about each I/O node participating in the job.
 *
 * COMMON_ARGUMENTS_GO_HERE
 *
 * \section exit EXIT STATUS
 *
 * 0 on success, 1 otherwise.
 *
 * \section examples EXAMPLES
 *
 * user@bgq ~> job_status 43
 * 2 I/O connections
 * Location     Compute Nodes  Drained  Killed  HW Failure  Ended  Error  Exited  Loaded  Output  Running
 * R00-ID-J02              16                                              x       x        x   
 * R00-ID-J03              16                                              x       x        x   
 * user@bgq ~> 
 *
 * In this example, job 43 is using two I/O nodes: R00-ID-J02 and R00-ID-J02. Each of them have
 * successfully loaded the job, began standard output services, and the job is running.
 *
 * \section AUTHOR
 *
 * IBM
 *
 * \section copyright COPYRIGHT
 *
 * © Copyright IBM Corp. 2010, 2011
 *
 * \section also SEE ALSO
 *
 * - \link runjob_server runjob_server \endlink
 */

#include "server/commands/job_status/Options.h"

#include "common/defaults.h"
#include "common/logging.h"
#include "common/properties.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <iostream>

LOG_DECLARE_FILE( runjob::server::commands::log );

namespace runjob {
namespace server {
namespace commands {
namespace job_status {

Options::Options(
        unsigned int argc,
        char** argv
        ) :
    runjob::commands::Options( defaults::ServerCommandService, runjob::server::commands::log, runjob::commands::Message::Tag::JobStatus, argc, argv ),
    _options("Options"),
    _details()
{
    namespace po = boost::program_options;
    _options.add_options()
        ("id", po::value(&_job), "job ID")
        ("details", po::bool_switch(&_details), "display detailed I/O node information")
        ;

    // id is positional
    _positionalArgs.add( "id", 1 );

    // add generic args
    Options::add(
            runjob::server::commands::PropertiesSection,
            _options
            );
}

void
Options::doHelp(
        std::ostream& os
        ) const
{
    os << _options << std::endl;
}

const char*
Options::description() const
{
    return 
        "Get detailed information about job <id>.\n"
        "\n"
        "Requires Read authority.";
}

void
Options::doValidate() const
{
    if ( _job == 0 ) {
        BOOST_THROW_EXCEPTION( boost::program_options::error("missing required option 'id'") );
    }
    
    const runjob::commands::request::JobStatus::Ptr request(
            boost::static_pointer_cast<runjob::commands::request::JobStatus>( this->getRequest() )
            );

    request->setId( _job );
}

void
Options::doHandle(
        const runjob::commands::Response::Ptr& msg
        ) const
{
    const runjob::commands::response::JobStatus::Ptr response(
            boost::dynamic_pointer_cast<runjob::commands::response::JobStatus>(msg)
            );
    if ( !response ) {
        LOG_FATAL_MSG( "received unknown response type: " << runjob::commands::Message::toString(msg->getTag()) );
        return;
    }

    // get I/O connections
    const runjob::commands::response::JobStatus::Connections& connections = response->_connections;

    // output number of connections
    std::cout << 
        connections.size() << " I/O connection" <<
        ( connections.size() == 1 ? "" : "s" ) <<
        std::endl
        ;

    if ( !_details ) {
        this->summary( connections );
    } else {
        this->details( connections );
    }

    if ( !response->_killTimeout.is_not_a_date_time() ) {
        std::cout << std::endl;
        std::cout << "kill timeout expires in: " << response->_killTimeout.total_seconds() << " seconds" << std::endl;
    }
}

void
Options::details(
        const runjob::commands::response::JobStatus::Connections& connections
        ) const
{
    // calculate maximum size of each value when converted to a string
    size_t location = strlen("Location");
    size_t nodes = strlen("Compute Nodes");
    size_t drained = strlen("Drained");
    size_t killed = strlen("Killed");
    size_t hwfail = strlen("HW Failure");
    size_t ended = strlen("Ended");
    size_t error = strlen("Error");
    size_t exited = strlen("Exited");
    size_t loaded = strlen("Loaded");
    size_t output_started = strlen("Output");
    size_t running = strlen("Running");
    BOOST_FOREACH( const runjob::commands::response::JobStatus::Connections::value_type& io, connections ) {
        char buf[32] = {0};
        (void)bg_uci_toString( io._location, buf );
        location = strlen(buf) > location ? strlen(buf) : location;
        const std::string nodesString = boost::lexical_cast<std::string>(io._computes);
        nodes = nodesString.size() > nodes  ? nodesString.size() : nodes;
    }

    // create format string using sizes previously calculated
    std::ostringstream formatting;
    formatting << "%-" << location + 1 << "s ";
    formatting << "%" << nodes + 1 << "s ";
    formatting << "%=" << drained + 1 << "s ";
    formatting << "%=" << killed + 1 << "s ";
    formatting << "%=" << hwfail + 1 << "s ";
    formatting << "%=" << ended + 1 << "s ";
    formatting << "%=" << error + 1 << "s ";
    formatting << "%=" << exited + 1 << "s ";
    formatting << "%=" << loaded + 1 << "s ";
    formatting << "%=" << output_started + 1 << "s ";
    formatting << "%=" << running + 1 << "s\n";
    std::cout << boost::format(formatting.str()) %
        "Location" %
        "Compute Nodes" %
        "Drained" %
        "Killed" %
        "HW Failure" %
        "Ended" %
        "Error" %
        "Exited" %
        "Loaded" %
        "Output" %
        "Running";
    BOOST_FOREACH( const runjob::commands::response::JobStatus::Connections::value_type& io, connections ) {
        char buf[32] = {0};
        (void)bg_uci_toString( io._location, buf );

        std::cout << boost::format(formatting.str()) %
            buf %
            io._computes %
            (io._drained ? "x" : " ") %
            (io._killed ? "x" : " ") %
            (io._hardwareFailure ? "x" : " ") %
            (io._ended ? "x" : " ") %
            (io._error ? "x" : " ") %
            (io._exited ? "x" : " ") %
            (io._loaded ? "x" : " ") %
            (io._outputStarted ? "x" : " ") %
            (io._running ? "x" : " ")
            ;
    }
}

void
Options::summary(
        const runjob::commands::response::JobStatus::Connections& connections
        ) const
{
    size_t drained, killed, hwFailure, ended, error, exited, loaded, output, running;
    drained = killed = hwFailure = ended = error = exited = loaded = output = running = 0;
    BOOST_FOREACH( const auto& io, connections ) {
        drained += (io._drained ? 1 : 0 );
        killed += (io._killed ? 1 : 0 );
        hwFailure += (io._hardwareFailure ? 1 : 0);
        ended += (io._ended ? 1 : 0 );
        error += (io._error ? 1 : 0 );
        exited += (io._exited ? 1 : 0 );
        loaded += (io._loaded ? 1 : 0 );
        output += (io._outputStarted ? 1 : 0 );
        running += (io._running ? 1 : 0 );
    }

    std::cout << "Drained:    " << drained << " of " << connections.size() << " I/O nodes" << std::endl;
    std::cout << "Killed:     " << killed << " of " << connections.size() << " I/O nodes" << std::endl;
    std::cout << "HW Failure: " << hwFailure << " of " << connections.size() << " I/O nodes" << std::endl;
    std::cout << "Ended:      " << ended << " of " << connections.size() << " I/O nodes" << std::endl;
    std::cout << "Error:      " << error << " of " << connections.size() << " I/O nodes" << std::endl;
    std::cout << "Exited:     " << exited << " of " << connections.size() << " I/O nodes" << std::endl;
    std::cout << "Loaded:     " << loaded << " of " << connections.size() << " I/O nodes" << std::endl;
    std::cout << "Output:     " << output << " of " << connections.size() << " I/O nodes" << std::endl;
    std::cout << "Running     " << running << " of " << connections.size() << " I/O nodes" << std::endl;

    std::cout << std::endl;
}

} // job_status
} // commands
} // server
} // runjob