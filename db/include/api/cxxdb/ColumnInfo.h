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

#ifndef CXXDB_COLUMN_INFO_H_
#define CXXDB_COLUMN_INFO_H_

#include <sqltypes.h>

#include <string>
#include <vector>

namespace cxxdb {

struct ColumnInfo
{
    std::string name;
    SQLSMALLINT data_type;
    SQLULEN size;
    SQLSMALLINT decimal_digits;
    bool nullable;
};

typedef std::vector<ColumnInfo> ColumnInfos;

} // namespace cxxdb

#endif
