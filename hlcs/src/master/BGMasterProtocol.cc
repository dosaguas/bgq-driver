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

#include <utility/include/Log.h>
#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include "BGMasterProtocol.h"

LOG_DECLARE_FILE( "master" );

BGMasterProtocol::~BGMasterProtocol() {
    // Lock and unlock so that we don't destruct 
    // in the middle of a send/receive operation.
    // If the op is hung on a blocking send/receive,
    // we can't destruct, but if that ever happens,
    // we've got bigger problems that need to be addressed.
    boost::mutex::scoped_lock scoped_lock(_sr_lock);
    scoped_lock.unlock();
}
void BGMasterProtocol::sendOnly(const std::string& requestName, const XML::Serializable& requestObject) {
    LOG_INFO_MSG(__FUNCTION__);
    boost::mutex::scoped_lock scoped_lock(_sr_lock);
    if(!_requester) {
        throw CxxSockets::CxxError(-1, "No requester socket.");
    }

    // Send the request
    CxxSockets::Message requestNameMessage(requestName);
    CxxSockets::Message requestMessage;
    requestObject.write(requestMessage);
    LOG_TRACE_MSG("sending request name");
    _requester->Send(requestNameMessage);
    LOG_TRACE_MSG("sent name, sending message");
    _requester->Send(requestMessage);
    LOG_TRACE_MSG("sent message");
}

void BGMasterProtocol::sendReceive(const std::string& requestName,
                                        const XML::Serializable& requestObject,
                                        const std::string& replyName,
                                        XML::Serializable& replyObject) {
    LOG_INFO_MSG(__FUNCTION__);
    boost::mutex::scoped_lock scoped_lock(_sr_lock);
    if(!_requester) {
        throw CxxSockets::CxxError(-1, "No requester socket.");
    }

    // Send the request
    CxxSockets::Message requestNameMessage(requestName);
    CxxSockets::Message requestMessage;
    requestObject.write(requestMessage);
    LOG_TRACE_MSG("sending request name");
    _requester->Send(requestNameMessage);
    LOG_TRACE_MSG("sent name, sending message");
    _requester->Send(requestMessage);
    LOG_TRACE_MSG("sent message");

    // Receive the reply
    CxxSockets::Message replyClassName;
    CxxSockets::Message replyMessage;

    LOG_TRACE_MSG("receiving reply class name");
    _requester->Receive(replyClassName);
    LOG_TRACE_MSG("reply class name received");
    if(replyClassName.str() != replyName) {
        LOG_ERROR_MSG("Bad reply: " << replyClassName.str() << "!=" << replyName);
    }

    LOG_TRACE_MSG("receiving reply");
    _requester->Receive(replyMessage);
    LOG_TRACE_MSG("reply received");

    std::istringstream is(replyMessage.str());
    replyObject.read(is);
}

void BGMasterProtocol::initializeRequester(int ipv, const std::string& host, std::string port, std::string security, int attempts) {
     LOG_INFO_MSG(__FUNCTION__);
   // zero attempts means try 'forever'
    CxxSockets::SockAddrList remote_list(ipv, host, port);

    // Connect to the server.
    // Let the caller decide the exception handling policy
    int retries = 0;
    bool forever = false;
    int timeout = 500;
    const int timeout_max = 3000000;

    (attempts == 0)?forever=true:forever=false;

    // Normally, we pass socket exceptions back to the client.
    // Here, however, we make use of them.
    bool connected = false;
    BOOST_FOREACH(CxxSockets::SockAddr& remote, remote_list) {
        if(connected) {
            break;
        }
        _remote = remote;
        while(retries < attempts || forever) {
            CxxSockets::SecureTCPSocketPtr sock(new CxxSockets::SecureTCPSocket(remote.family(), 0, CxxSockets::SECURE, CxxSockets::CRYPTALL));
            try {
                bgq::utility::ClientPortConfiguration port_config(0, bgq::utility::ClientPortConfiguration::ConnectionType::Command);
                port_config.setProperties(_props, "");
                port_config.notifyComplete();
                sock->Connect(remote, port_config);
            } catch(CxxSockets::SockHardError& e) {
                LOG_WARN_MSG("Server not available.  " << e.what() << " Retrying." );
                if(timeout < timeout_max)
                    timeout *= 10;
                usleep(timeout);
                ++retries;
                continue;
            } catch(CxxSockets::SockSoftError& e) {
                LOG_DEBUG_MSG("Caught " << e.what() << " exception.  Retrying");
                if(timeout < timeout_max)
                    timeout *= 10;
                usleep(timeout);
                ++retries;
                continue;
            } catch(CxxSockets::CxxError& e) {
                LOG_ERROR_MSG("Unexpected error on connecting socket: " << e.what());
                ++retries;
                continue;
            }
            _requester = sock;
            connected = true;
            break;
        }
    }

    if(retries >= attempts && !forever) {
        std::ostringstream msg;
        msg << "Retries timed out attempting to connect to " << host << ":" << port;
        LOG_ERROR_MSG(msg.str());
        throw CxxSockets::SockHardError(EAGAIN, msg.str());
    }

    // Keepalive.  Going to fire 'em off pretty quick because LNs
    // may go away without warning.  
    // 1) Two seconds from the last data packet, we send our first probe.
    // 2) Send a probe every second.
    // 3) Only three probes to consider connection dead.
    // So what this means is that after we send a packet, we wait five
    // seconds.  If we haven't sent another, we start sending probes.
    // We send a probe every second and if the other side doesn't respond
    // in three seconds, we consider the connection dead.
    // So, if the connection is not responsive for five seconds, we
    // consider it bad.
    _requester->setProbe(true, 2, 1, 3);
}

void BGMasterProtocol::initializeResponder(CxxSockets::SecureTCPSocketPtr sock) {
     LOG_INFO_MSG(__FUNCTION__);
     _responder = sock;
    // Keepalive.  Going to fire 'em off pretty quick because LNs
    // may go away without warning.  
    // 1) Five seconds from the last data packet, we send our first probe.
    // 2) Send a probe every second.
    // 3) Only three probes to consider connection dead.
    // So what this means is that after we send a packet, we wait five
    // seconds.  If we haven't sent another, we start sending probes.
    // We send a probe every second and if the other side doesn't respond
    // in three seconds, we consider the connection dead.
    // So, if the connection is not responsive for five seconds, we
    // consider it bad.
    _responder->setProbe(true, 2, 1, 3);
}

void BGMasterProtocol::setRequester(CxxSockets::SecureTCPSocketPtr sock) {
    LOG_INFO_MSG(__FUNCTION__);
    _requester = sock;
}

void BGMasterProtocol::sendReply(const std::string& requestName, XML::Serializable& replyObject) {
    if(!_responder) return;
    LOG_INFO_MSG("Sending " << requestName << " reply.");
    // First send the class name
    CxxSockets::Message replyClassName(requestName);
    if(_responder) {
        _responder->Send(replyClassName);
    } else {
        std::ostringstream msg;
        msg << "repsonder not yet initialized.";
        LOG_ERROR_MSG(msg.str());
        throw CxxSockets::SockSoftError(EAGAIN, msg.str());
    }

    // Then send the message
    CxxSockets::Message replyMessage;
    replyObject.write(replyMessage);
    LOG_TRACE_MSG("sending reply");
    _responder->Send(replyMessage);
    LOG_TRACE_MSG("sent reply");
}

void BGMasterProtocol::getName(std::string& requestName) {
    LOG_INFO_MSG(__FUNCTION__);
    if(!_responder) return;
    CxxSockets::Message classNameMessage;
    LOG_TRACE_MSG("receiving class name");
    _responder->Receive(classNameMessage);
    LOG_TRACE_MSG("received class name");
    requestName = classNameMessage.str();
}

void BGMasterProtocol::getObject(XML::Serializable* requestObject) {
    LOG_INFO_MSG(__FUNCTION__);
    if(!_responder) return;
    CxxSockets::MsgMap mess_map;

    CxxSockets::Message msg;
    LOG_TRACE_MSG("receiving object");
    _responder->Receive(msg);
    LOG_TRACE_MSG("received object");
    std::istringstream is(msg.str());
    requestObject->read(is);
}
