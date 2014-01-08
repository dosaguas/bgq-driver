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
/* (C) Copyright IBM Corp.  2011, 2011                              */
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


#include "JobIdOption.hpp"

#include <utility/include/Log.h>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <stdexcept>
#include <string>


using boost::lexical_cast;

using std::string;


LOG_DECLARE_FILE( "bgws" );


namespace bgws {


std::istream& operator>>( std::istream& is, JobIdOption& job_id_option_out )
{
    string s;
    is >> s;

    // If argument is empty string, then is >> s will set failbit, which will cause boost::program_options to throw, but we want this to be OK so clear the fail bit.
    if ( is.rdstate() & std::ios::failbit ) {
        is.clear( is.rdstate() & ~std::ios::failbit );
    } else {
        try {
            BGQDB::job::Id id(lexical_cast<BGQDB::job::Id>( s ));

            job_id_option_out.set( id );

        } catch ( std::exception& e ) {
            LOG_DEBUG_MSG( "Inavalid job ID option, '" << s << "'" );
            // ignore.
        }
    }

    return is;
}


} // namespace bgws
