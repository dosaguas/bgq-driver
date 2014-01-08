/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/* Blue Gene/Q                                                      */
/* (C) Copyright IBM Corp.  2010, 2012                              */
/* US Government Users Restricted Rights - Use, duplication or      */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM    */
/*   Corp.                                                          */
/*                                                                  */
/* This software is available to you under the Eclipse Public       */
/* License (EPL).                                                   */
/*                                                                  */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#include <sstream>
#include <iostream>
#include <vector>
#include <ras/include/RasEvent.h>
#include <hwi/include/bqc/gea_dcr.h>

using namespace std;

#include "_gea_interrupt_state.cc"
#include "utils.h"

extern "C" {

  Decoder GEA_DECODERS[] = {
    { GEA_DCR( GEA_INTERRUPT_STATE__MACHINE_CHECK ),       decode_GEA_INTERRUPT_STATE },
    { GEA_DCR( INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK ),  decode_INTERNAL_ERROR },
  };

  void fw_GEA_machineCheckDecoder( RasEvent& event, const vector<uint64_t>& mbox ) {
    walkDetails( GEA_DECODERS, sizeof(GEA_DECODERS)/sizeof(GEA_DECODERS[0]), mbox, event, "DETAILS" );
  }


}

