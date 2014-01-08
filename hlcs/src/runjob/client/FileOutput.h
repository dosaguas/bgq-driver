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
#ifndef RUNJOB_CLIENT_FILE_OUTPUT_H
#define RUNJOB_CLIENT_FILE_OUTPUT_H

#include "client/Output.h"

namespace runjob {
namespace client {
namespace options {

class Parser;

} // options

/*!
 * \brief Handle output for stdout or stderr to a file
 */
class FileOutput : public Output
{
public:
    /*!
     * \brief ctor.
     */
    FileOutput(
            const boost::shared_ptr<const options::Parser>& options,    //!< [in]
            boost::asio::io_service& io_service,                        //!< [in]
            int fd,                                                     //!< [in]
            const char* label                                           //!< [in]
         );

private:
    void __attribute__ ((visibility("hidden"))) write();
    void __attribute__ ((visibility("hidden"))) writeImpl(
            ssize_t rc,
            int error
            );

private:
    boost::asio::io_service& _io_service;
    const int _fd;
};

} // client
} // runjob

#endif

