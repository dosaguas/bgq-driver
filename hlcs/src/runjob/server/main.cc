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
#include "common/unexpected.h"

#include "server/Server.h"
#include "server/Options.h"

#include <iostream>

int
main(int argc, char** argv)
{
    std::set_terminate( runjob::log_stack_trace );

    int status = EXIT_SUCCESS;
    try {
        using namespace runjob::server;

        // parse args and properties
        Options options(argc, argv);

        if ( options.getHelp() ) {
            options.help(std::cout);
        } else if (options.getVersion() ) {
            options.version(std::cout);
            std::cout << std::endl;
        } else {
            // create and start server
            const Server::Ptr server(
                    Server::create( options )
                    );
            status = server->start();
        }
    } catch ( const boost::program_options::error& e ) {
        // eat this error, we have already logged it
        status = EXIT_FAILURE;
    }

    exit( status );
}
