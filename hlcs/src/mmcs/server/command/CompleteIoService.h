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

#ifndef MMCS_SERVER_COMMAND_COMPLETE_IO_SERVICE_H_
#define MMCS_SERVER_COMMAND_COMPLETE_IO_SERVICE_H_

#include "common/AbstractCommand.h"

#include <db/include/api/BGQDBlib.h>

namespace mmcs {
namespace server {
namespace command {

class CompleteIoService: public common::AbstractCommand
{
public:
    CompleteIoService(const char* name, const char* description, const Attributes& attributes)
      : AbstractCommand(name,description,attributes) { _usage = "complete_io_service <location>";}
    static  CompleteIoService* build();    // factory method
    void execute(
            std::deque<std::string> args,
            mmcs_client::CommandReply& reply,
            DBConsoleController* pController,
            BlockControllerTarget* pTarget=NULL
            );
    bool checkArgs(std::deque<std::string>& args) { if (args.size() != 1) return false; else return true;}
    void help(
            std::deque<std::string> args,
            mmcs_client::CommandReply& reply
            );
private:
    BGQDB::STATUS impl(
            const std::string& location,
            std::string& containingBlock
        );
};

} } } // namespace mmcs::server::command

#endif
