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
/* (C) Copyright IBM Corp.  2012, 2012                              */
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

#ifndef MMCS_SERVER_BC_SERVICECARD_INFO_H_
#define MMCS_SERVER_BC_SERVICECARD_INFO_H_

#include "BCIconInfo.h"

namespace mmcs {
namespace server {

class BCServicecardInfo : public BCIconInfo
{
public:
    BCServicecardInfo(const std::string& loc) : BCIconInfo(loc) {}
    std::string cardName() { return "S"; }
};

} } // namespace mmcs::server

#endif
