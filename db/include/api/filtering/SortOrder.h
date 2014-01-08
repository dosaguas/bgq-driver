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

#ifndef BGQDB_FILTERING_SORT_ORDER_H_
#define BGQDB_FILTERING_SORT_ORDER_H_

/*!
 * \file api/filtering/BlockFilter.h
 * \brief SortOrder enumeration.
 */

namespace BGQDB {
namespace filtering {

/*!
 * \brief Sort order options.
 */
struct SortOrder
{
    /*!
     * \brief Sort order values.
     */
    enum Value {
        Ascending,  //!< Sort in ascending order
        Descending  //!< Sort in descending order
    };
};

} // filtering
} // BGQDB

#endif
