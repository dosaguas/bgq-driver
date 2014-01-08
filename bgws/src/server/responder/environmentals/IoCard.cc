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

/*! \page ioCardEnvironmentalsResource /bg/environmentals/ioCard

I/O card environmentals.

- \subpage ioCardEnvironmentalsResourceGet

 */

/*! \page ioCardEnvironmentalsResourceGet GET /bg/environmentals/ioCard

Query I/O card environmentals.

\note This resource supports \ref partialResponse "partial responses".


\section Authorization

The user must have hardware READ authority.


\section queryParms Query parameters

- location: \ref wildcardFormat "wildcard string"
- interval: a \ref timeIntervalFormat "time interval". If no interval, will return most recent for each location.
- v08: \ref numberComparisonFormat "double-precision floating point number comparison"
- v14: \ref numberComparisonFormat "double-precision floating point number comparison"
- v25: \ref numberComparisonFormat "double-precision floating point number comparison"
- v33: \ref numberComparisonFormat "double-precision floating point number comparison"
- v120: \ref numberComparisonFormat "double-precision floating point number comparison"
- v15: \ref numberComparisonFormat "double-precision floating point number comparison"
- v09: \ref numberComparisonFormat "double-precision floating point number comparison"
- v10: \ref numberComparisonFormat "double-precision floating point number comparison"
- v120p: \ref numberComparisonFormat "double-precision floating point number comparison"
- v33p: \ref numberComparisonFormat "double-precision floating point number comparison"
- v12: \ref numberComparisonFormat "double-precision floating point number comparison"
- v18: \ref numberComparisonFormat "double-precision floating point number comparison"
- v12p: \ref numberComparisonFormat "double-precision floating point number comparison"
- v15p: \ref numberComparisonFormat "double-precision floating point number comparison"
- v18p: \ref numberComparisonFormat "double-precision floating point number comparison"
- v25p: \ref numberComparisonFormat "double-precision floating point number comparison"
- temp: \ref numberComparisonFormat "integer number comparison"
- sort: Sets the sort order. Format is [&lt;direction&gt;]&lt;field&gt;
  - Fields are:
    - location
    - time
    - v08
    - v14
    - v25
    - v33
    - v120
    - v15
    - v09
    - v10
    - v120p
    - v33p
    - v12
    - v18
    - v12p
    - v15p
    - v18p
    - v25p
    - temp


\section responseFormat JSON response format

<pre>
[
  {
    "location": &quot;<i>location</i>&quot;,
    "time": &quot;<i>\ref timestampFormat "timestamp"</i>&quot;,
    "v08": <i>number</i>,
    "v14": <i>number</i>,
    "v25": <i>number</i>,
    "v33": <i>number</i>,
    "v120": <i>number</i>,
    "v15": <i>number</i>,
    "v09": <i>number</i>,
    "v10": <i>number</i>,
    "v120p": <i>number</i>,
    "v33p": <i>number</i>,
    "v12": <i>number</i>,
    "v18": <i>number</i>,
    "v12p": <i>number</i>,
    "v15p": <i>number</i>,
    "v18p": <i>number</i>,
    "v25p": <i>number</i>,
    "temp": <i>number</i>
  },
  ...
]
</pre>

\section Errors

HTTP status: 403 Forbidden
- authority: The user doesn't have hardware READ authority.

 */


#include "IoCard.hpp"

#include "../../dbConnectionPool.hpp"
#include "../../Error.hpp"
#include "../../RequestRange.hpp"

#include "../../query/env/IoCardOptions.hpp"
#include "../../query/env/Query.hpp"

#include "capena-http/server/Request.hpp"
#include "capena-http/server/Response.hpp"

#include "chiron-json/json.hpp"

#include <db/include/api/tableapi/gensrc/bgqtableapi.h>

#include <utility/include/Log.h>

#include <string>


using std::string;


LOG_DECLARE_FILE( "bgws" );


namespace bgws {
namespace responder {
namespace environmentals {


const capena::http::uri::Path IoCard::RESOURCE_PATH(capena::http::uri::Path() / "environmentals" / "ioCard");


void IoCard::doGet()
{

    if ( ! _userHasHardwareRead() ) {
        LOG_WARN_MSG( "Could not get environmentals because " << getRequestUserInfo() << " doesn't have authority." );

        BOOST_THROW_EXCEPTION( Error(
                "Could not get environmentals because the user doesn't have authority.",
                "getEnvironmentals", "authority", Error::Data(),
                capena::http::Status::Forbidden
            ) );

        return;
    }


    const capena::server::Request &request(_getRequest());

    static const unsigned DefaultRangeSize(50), MaxRangeSize(100);
    RequestRange req_range( request, DefaultRangeSize, MaxRangeSize );

    auto conn_ptr(dbConnectionPool::getConnection());
    uint64_t all_count(0);
    cxxdb::ResultSetPtr rs_ptr;

    boost::shared_ptr<query::env::IoCardOptions> options_ptr( new query::env::IoCardOptions() );

    options_ptr->setArgs( request.getUrl().getQuery().calcArguments(), req_range );

    query::env::Query query( options_ptr );

    query.execute(
            conn_ptr,
            &all_count,
            &rs_ptr
        );

    json::ArrayValue arr_val;
    json::Array &arr(arr_val.get());

    if ( all_count != 0 ) {
        while ( rs_ptr->fetch() ) {
            json::Object &obj(arr.addObject());
            obj.set( "location", rs_ptr->columns()[BGQDB::DBTIocardenvironment::LOCATION_COL].getString() );
            obj.set( "time", rs_ptr->columns()[BGQDB::DBTIocardenvironment::TIME_COL].getTimestamp() );
            obj.set( "v08", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV08_COL].as<double>() );
            obj.set( "v14", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV14_COL].as<double>() );
            obj.set( "v25", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV25_COL].as<double>() );
            obj.set( "v33", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV33_COL].as<double>() );
            obj.set( "v120", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV120_COL].as<double>() );
            obj.set( "v15", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV15_COL].as<double>() );
            obj.set( "v09", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV09_COL].as<double>() );
            obj.set( "v10", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV10_COL].as<double>() );
            obj.set( "v120p", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV120P_COL].as<double>() );
            obj.set( "v33p", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV33P_COL].as<double>() );
            obj.set( "v12", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV12_COL].as<double>() );
            obj.set( "v18", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV18_COL].as<double>() );
            obj.set( "v12p", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV12P_COL].as<double>() );
            obj.set( "v15p", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV15P_COL].as<double>() );
            obj.set( "v18p", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV18P_COL].as<double>() );
            obj.set( "v25p", rs_ptr->columns()[BGQDB::DBTIocardenvironment::VOLTAGEV25P_COL].as<double>() );
            obj.set( "temp", rs_ptr->columns()[BGQDB::DBTIocardenvironment::TEMPMONITOR_COL].as<int32_t>() );
        }
    }


    capena::server::Response &response(_getResponse());

    req_range.updateResponse( response, arr.size(), all_count );

    response.setContentTypeJson();
    response.headersComplete();

    json::Formatter()( arr_val, response.out() );
}


} } } // namespace bgws::responder::environmentals
