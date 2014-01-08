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


#ifndef MMCS_SERVER_COMMAND_SQL_H_
#define MMCS_SERVER_COMMAND_SQL_H_


#include "common/AbstractCommand.h"


namespace mmcs {
namespace server {
namespace command {


/*!
** sql <sql-string>
** execute <sql-string>
** This command cannot be used to run any sql statement that returns output (i.e., queries).
*/
class Sql: public common::AbstractCommand
{
public:
    Sql(const char* name, const char* description, const Attributes& attributes)
      : AbstractCommand(name,description,attributes) { usage = "sql <sql-string>";}
    static  Sql* build();    // factory method
    static  std::string cmdname() { return "sql"; }
    void execute(std::deque<std::string> args,
             mmcs_client::CommandReply& reply,
             common::ConsoleController* pController,
             BlockControllerTarget* pTarget=NULL);
    bool checkArgs(std::deque<std::string>& args) { if (args.size() == 0) return false; else return true; }
    void help(std::deque<std::string> args,
              mmcs_client::CommandReply& reply);
};

} } } // namespace mmcs::server::command

#endif