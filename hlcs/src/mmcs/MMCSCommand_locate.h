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
/* (C) Copyright IBM Corp.  2005, 2011                              */
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

/*!
 * \file MMCSCommand_locate.h
 * The locate command shows the addressing information for nodes and idos in a block.
 */

#ifndef _MMCSCOMMAND_LOCATE_H
#define _MMCSCOMMAND_LOCATE_H

#include "MMCSCommand.h"

class MMCSCommand_locate: public MMCSCommand
{
public:
    /*!
    ** MMCSCommand_locate constructor
    ** The constructor is normally invoked only by the build() method
    ** @param name the command name
    ** @param desc a brief, one line description
    ** @param attr describes the attributes and requirements of the command
    */
    MMCSCommand_locate(const char* name, const char* description, const MMCSCommandAttributes& attributes)
      : MMCSCommand(name,description,attributes) { usage = "[<target>] locate [neighbors [verbose]] [ras_format]";}

    /*!
    ** build() - MMCSCommand factory
    ** This is invoked at MMCS startup when MMCS builds its list of commands
    ** @return an MMCSCommand object for this specific command
    */
    static  MMCSCommand_locate* build();	// factory method

    /*!
    ** cmdname() - Returns the command name
    ** This returns the name of the command. It is a static method so that it
    ** can be used before the object is built, in conjunction with build,
    ** for inserting the command objects into a map.
    ** @return command name
    */
    static  std::string cmdname() { return "locate"; }

    /*!
    ** execute() - Perform specific MMCS command
    ** @param args the command arguments
    ** @param reply       the command output stream. Refer to class MMCSCommandReply
    ** @param pController the ConsoleController object that the command is to work on
    */
    void execute(std::deque<std::string> args,
			 MMCSCommandReply& reply,
			 ConsoleController* pController,
			 BlockControllerTarget* pTarget=NULL);

    bool checkArgs(std::deque<std::string>& args) { return true; }

    /*!
    ** help() - Print extended command help to the reply stream
    ** @param args the help command arguments
    ** @param reply the command output stream. Refer to class MMCSCommandReply
    */
    void help(std::deque<std::string> args,
		      MMCSCommandReply& reply);
};

#endif
