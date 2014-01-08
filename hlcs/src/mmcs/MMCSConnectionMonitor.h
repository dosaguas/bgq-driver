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

#include "MMCSThread.h"
#include "ConsoleController.h"

#ifndef MMCSCONNECTIONMONITOR_H
#define MMCSCONNECTIONMONITOR_H

//******************************************************************************
// Class for polling the mmcs connection
//******************************************************************************
class ConnectionMonitor : public MMCSThread
{
    ConsoleController* _controller;
public:
    ConnectionMonitor() {};
    void setConsole(ConsoleController* p) { _controller = p; }
    void* threadStart();
};

#endif
