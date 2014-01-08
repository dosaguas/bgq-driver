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

#include "cxxdb/AbstractStatement.h"

#include "BasicConnection.h"

#include "cxxdb/ConnectionHandle.h"
#include "cxxdb/exceptions.h"
#include "cxxdb/StatementHandle.h"

#include <utility/include/Log.h>

#include <sqlext.h>

LOG_DECLARE_FILE( "database" );

#define CHECK_VALID { if ( ! isValid() ) { CXXDB_THROW_EXCEPTION( cxxdb::InvalidObjectException( "Statement", __FUNCTION__ ) ); } }

namespace cxxdb {

AbstractStatement::AbstractStatement(
        BasicConnectionPtr connection_ptr,
        ConnectionHandle& connection_handle,
        const std::string& sql,
        const ParameterNames& parameter_names
) :
    _conn_ptr( connection_ptr ),
    _handle_ptr( new StatementHandle( connection_handle ) )
{
    _handle_ptr->prepare( sql );

    _parameters_ptr.reset( new Parameters( *_handle_ptr, parameter_names ) );
}


Parameters& AbstractStatement::parameters()
{
    CHECK_VALID;

    return *_parameters_ptr;
}


AbstractStatement::~AbstractStatement()
{
    if ( ! _handle_ptr ) {
        return;
    }

    BasicConnectionPtr conn_ptr(_conn_ptr.lock());
    if ( conn_ptr ) {
        conn_ptr->notifyStatementDestroyed( this );
    }
}


void AbstractStatement::_invalidate()
{

    if ( ! _handle_ptr ) {
        return;
    }

    LOG_DEBUG_MSG( "invalidating statement" );

    _handle_ptr.reset();
}


} // namespace cxxdb
