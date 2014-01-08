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
/* (C) Copyright IBM Corp.  2011, 2012                              */
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

//! \file  SimStdioController.cc
//! \brief Methods for bgcios::stdio::SimStdioController class.

// Includes
#include "SimStdioController.h"
#include <ramdisk/include/services/common/logging.h>
#include <ramdisk/include/services/IosctlMessages.h>
#include <ramdisk/include/services/MessageUtility.h>
#include <ramdisk/include/services/ServicesConstants.h>
#include <poll.h>
#include <errno.h>
#include <stdlib.h>

#include <boost/foreach.hpp>

#include <fstream>
#include <iomanip>

using namespace bgcios::stdio;

LOG_DECLARE_FILE("cios.stdiod");


SimStdioController::SimStdioController(StdioConfigPtr config) : StdioController(config)
{
   // Initialize private data.
   _simId = _config->getSimulationId();

   // Set work directory.
   std::ostringstream wdir;
   wdir << bgcios::SimulationDirectory << _simId << "/";
   _workDirectory = wdir.str();

   // Set path to iosd command channel.
   std::ostringstream iosdPath;
   iosdPath << _workDirectory << bgcios::IosctlCommandChannelName << "." << getppid();
   _iosdCmdChannelPath = iosdPath.str();
}

SimStdioController::~SimStdioController()
{
   _dataListener.reset();
   _dataChannel.reset();
   free(_inboundMessage);
   free(_outboundMessage);
}

int
SimStdioController::startup(in_port_t dataChannelPort)
{
   // Build the path to the command channel.
   std::ostringstream cmdChannelPath;
   cmdChannelPath << _workDirectory << bgcios::StdioCommandChannelName << "." << getppid();

   // Create the command channel socket.
   try {
      _cmdChannel = LocalDatagramSocketPtr(new LocalDatagramSocket(cmdChannelPath.str()));
      _cmdChannel->setRecvBufferSize(512*1024);
   }
   catch (SocketError& e) {
      LOG_ERROR_MSG("error creating command channel '" << cmdChannelPath.str() << "': " << e.what());
      return e.errcode();
   }

   // Create listening socket for data channel.
   try {
      _dataListener = bgcios::InetStreamSocketPtr(new bgcios::InetStreamSocket(dataChannelPort, true));
   }
   catch (bgcios::SocketError& e) {
      LOG_ERROR_MSG("error creating listening socket for data channel: " << e.what());
      _dataListener.reset();
      return e.errcode();
   }

   // Get the address assigned by TCP and store port number in a file.
   std::ostringstream portFileName;
   portFileName << _workDirectory << "standardio.port";
   std::ofstream portFile(portFileName.str().c_str(), std::ios_base::trunc);
   portFile << _dataListener->getPort() << std::endl;
   LOG_CIOS_DEBUG_MSG("stored port number " << _dataListener->getPort() << " in file " << portFileName.str());

   // Prepare socket to listen for connections.
   try {
      _dataListener->listen(1);
   }
   catch (bgcios::SocketError& e) {
      LOG_ERROR_MSG("error listening for new connections: " << e.what());
      return e.errcode();
   }
   LOG_CIOS_DEBUG_MSG("listening for new data channel connections on fd " << _dataListener->getSd() << " using address " << _dataListener->getName());

   return 0;
}

int
SimStdioController::cleanup(void)
{
   LOG_CIOS_DEBUG_MSG("running cleanup ...");

   // Remove the port number file.
   std::ostringstream portFileName;
   portFileName << _workDirectory << "standardio.port";
   if (::unlink(portFileName.str().c_str()) != 0) {
      LOG_ERROR_MSG("error removing port number file " << portFileName.str() << ": " << bgcios::errorString(errno));
   }

   // Remove the command channel.
   _cmdChannel.reset();

   return 0;
}

void
SimStdioController::eventMonitor(void)
{
   const int cmdChannel   = 0;
   const int dataChannel  = 1;
   const int dataListener = 2;
   const int numFds       = 3;

   pollfd pollInfo[numFds];
   int timeout = -1; // 10000; // 10 sec

   // Initialize the pollfd structure.
   pollInfo[cmdChannel].fd = _cmdChannel->getSd();
   pollInfo[cmdChannel].events = POLLIN;
   pollInfo[cmdChannel].revents = 0;
   LOG_CIOS_TRACE_MSG("added command channel using fd " << pollInfo[cmdChannel].fd << " to descriptor list");

   pollInfo[dataChannel].fd = _dataChannel == NULL ? -1 : _dataChannel->getSd();
   pollInfo[dataChannel].events = POLLIN;
   pollInfo[dataChannel].revents = 0;
   LOG_CIOS_TRACE_MSG("added data channel using fd " << pollInfo[dataChannel].fd << " to descriptor list");

   pollInfo[dataListener].fd = _dataListener->getSd();
   pollInfo[dataListener].events = POLLIN;
   pollInfo[dataListener].revents = 0;
   LOG_CIOS_TRACE_MSG("added data channel listener using fd " << pollInfo[dataListener].fd << " to descriptor list");

   // Process events until told to stop.
   while (!_done) {

      // Wait for an event on one of the descriptors.
      LOG_CIOS_DEBUG_MSG("polling for events with timeout " << timeout << "...")
      int rc = poll(pollInfo, numFds, timeout);

      // There was no data so try again.
      if (rc == 0) {
         continue;
      } 

      // There was an error so log the failure and try again.
      if (rc == -1) {
         int err = errno;
         if (err == EINTR) {
            LOG_CIOS_TRACE_MSG("poll returned EINTR, continuing ...");
            continue;
         }

         LOG_ERROR_MSG("error polling socket descriptors: " << bgcios::errorString(err));
         return;
      } 

      // Check for an event on the command channel.
      if (pollInfo[cmdChannel].revents & POLLIN) {
         LOG_CIOS_TRACE_MSG("input event available on command channel");
         commandChannelHandler();
      }

      // Check for an event on the data channel.
      if (pollInfo[dataChannel].revents & POLLIN) {
         LOG_CIOS_TRACE_MSG("input event available on data channel");
         dataChannelHandler();
      }

      // Check for an event on the data channel listener.
      if (pollInfo[dataListener].revents & POLLIN) {
         LOG_CIOS_TRACE_MSG("input event available on data channel listener");
         pollInfo[dataListener].revents = 0;
         
         // Make a new data channel connected to runjob.
         const InetSocketPtr incoming = makeDataChannel();
         if (!incoming) {
            LOG_ERROR_MSG("error making new data channel");
            continue;
         }

         LOG_INFO_MSG_FORCED("data channel is connected to " << incoming->getPeerName() << " using fd " << incoming->getSd());

         // Handle the Authenticate message which must be sent first.
         if (!dataChannelHandler(incoming)) {
             _dataChannel = incoming;
             pollInfo[dataChannel].fd = _dataChannel->getSd();
             LOG_INFO_MSG_FORCED("data channel is connected to " << _dataChannel->getPeerName() << " using fd " << _dataChannel->getSd());;
         }
      }
   }

   // Reset for next time.
   _done = false;

   return;
}

int
SimStdioController::commandChannelHandler(void)
{
   // Receive a message from the command channel.
   std::string peer;
   int err = recvFromCommandChannel(peer, _inboundMessage);

   // An error occurred receiving a message.
   if (err != 0) {
      LOG_ERROR_MSG("error receiving message from command channel: " << bgcios::errorString(err));
      if (err == EPIPE) { // When command channel closes, stop handling events.
         _done = true;
      }

      return err;
   }

   // Make sure the service field is correct.
   bgcios::MessageHeader *msghdr = (bgcios::MessageHeader *)_inboundMessage;
   if ((msghdr->service != bgcios::StdioService) && (msghdr->service != bgcios::IosctlService)) {
      LOG_ERROR_MSG("Job " << msghdr->jobId << ": message service " << (int)msghdr->service << " is wrong, header: " << bgcios::printHeader(*msghdr));
      sendErrorAckToCommandChannel(peer, bgcios::WrongService, 0);
      return 0;
   }

   // Make the sure protocol version is a match.
   if (msghdr->version != ProtocolVersion) {
      LOG_ERROR_MSG("Job " << msghdr->jobId << ": message protocol version " << (int)msghdr->version << " does not match stdiod version " << (int)ProtocolVersion);
      sendErrorAckToCommandChannel(peer, bgcios::VersionMismatch, ProtocolVersion);
      return 0;
   }

   // Handle the message.
   LOG_CIOS_DEBUG_MSG("Job " << msghdr->jobId << ": " << bgcios::toString(msghdr) << " message is available on command channel");
   switch (msghdr->type) {

      case WriteStdout:
         err = writeStdout();
         break;

      case WriteStderr:
         err = writeStderr();
         break;

      case CloseStdio:
         err = closeStdio();
         break;

      case bgcios::iosctl::Terminate:
         err = terminate();
         break;

      default:
         LOG_ERROR_MSG("Job " << msghdr->jobId << ": message type " << msghdr->type << " is not supported, header: " << bgcios::printHeader(*msghdr));
         err = sendErrorAckToCommandChannel(peer, bgcios::UnsupportedType, 0);
         break;
   }

   if (err == 0) {
      LOG_CIOS_DEBUG_MSG("Job " << msghdr->jobId << ": ack message sent successfully");
   }
   else {
      LOG_ERROR_MSG("Job " << msghdr->jobId << ": error sending ack message: " << bgcios::errorString(err));
   }

   return 0;
}

int
SimStdioController::dataChannelHandler(InetSocketPtr authOnly)
{
   InetSocketPtr& dataChannel( authOnly ? authOnly : _dataChannel );

   // Receive a message from the data channel.
   int err = recvFromDataChannel(_inboundMessage, dataChannel);

   // When data channel closes, stop handling events.
   if (err == EPIPE) {
      LOG_CIOS_DEBUG_MSG("data channel connected to " << dataChannel->getPeerName() << " is closed");
      dataChannel.reset();
      return err;
   }

   // An error occurred receiving a message.
   if (err != 0) {
      LOG_ERROR_MSG("error receiving message from data channel: " << bgcios::errorString(err));
      return err;
   }

   // Make sure the service field is correct.
   bgcios::MessageHeader *msghdr = (bgcios::MessageHeader *)_inboundMessage;
   if (msghdr->service != bgcios::StdioService) {
      LOG_ERROR_MSG("Job " << msghdr->jobId << ": message service " << msghdr->service << " is wrong");
      sendErrorAckToDataChannel(bgcios::WrongService, bgcios::StdioService);
      return 1;
   }

   // Make the sure protocol version is a match.
   if (msghdr->version != ProtocolVersion) {
      LOG_ERROR_MSG("Job " << msghdr->jobId << ": message protocol version " << (int)msghdr->version << " does not match stdiod version " << (int)ProtocolVersion);
      sendErrorAckToDataChannel(bgcios::VersionMismatch, ProtocolVersion);
      return 1;
   }

   // Make sure the Authenticate message is arriving when expected.
   if (authOnly && msghdr->type != Authenticate) {
      LOG_CIOS_TRACE_MSG("Job " << msghdr->jobId << ": expected Authenticate message");
      return 1;
   } else if (!authOnly && msghdr->type == Authenticate) {
      LOG_CIOS_TRACE_MSG("Job " << msghdr->jobId << ": unexpected Authenticate message");
      dataChannel.reset();
      return 1;
   }

   // Handle the message.
   LOG_CIOS_DEBUG_MSG("Job " << msghdr->jobId << ": " << toString(msghdr->type) << " message is available");
   switch (msghdr->type) {
      case StartJob: 
          err = startJob();
          break;
      case ChangeConfig: 
          err = changeConfig();
          break;
      case Authenticate:
          err = authenticate(authOnly);
          break;
      case Reconnect:
          err = reconnect();
          break;

      default:
         LOG_ERROR_MSG("Job " << msghdr->jobId << ": message type " << msghdr->type << " is not supported");
         // send error
         break;
   }

   return err;
}

int
SimStdioController::writeStdout(void)
{
   // Get pointer to inbound WriteStdout message.
   WriteStdioMessage *inMsg = (WriteStdioMessage *)_inboundMessage;

   // Forward message to the data channel.
   int err = sendToDataChannel(inMsg);

   // Send ack message to job monitor.
   WriteStdioAckMessage *outMsg = (WriteStdioAckMessage *)_outboundMessage;
   memcpy(&(outMsg->header), &(inMsg->header), sizeof(MessageHeader));
   outMsg->header.type = WriteStdoutAck;
   outMsg->header.length = sizeof(WriteStdioAckMessage);
   outMsg->header.returnCode = bgcios::Success;
   if (err != 0) {
      LOG_ERROR_MSG("error forwarding WriteStdout message to data channel: " << bgcios::errorString(err));
      outMsg->header.returnCode = bgcios::SendError;
      outMsg->header.errorCode = (uint32_t)err;
   }
   std::ostringstream monitorPath;
   monitorPath << bgcios::SimulationDirectory << _simId << "/job." << inMsg->header.jobId;
   return sendToCommandChannel(monitorPath.str(), outMsg);
}

int
SimStdioController::writeStderr(void)
{
   // Get pointer to inbound WriteStderr message.
   WriteStdioMessage *inMsg = (WriteStdioMessage *)_inboundMessage;

   // Forward message to the data channel.
   int err = sendToDataChannel(inMsg);

   // Send ack message to job monitor.
   WriteStdioAckMessage *outMsg = (WriteStdioAckMessage *)_outboundMessage;
   memcpy(&(outMsg->header), &(inMsg->header), sizeof(MessageHeader));
   outMsg->header.type = WriteStderrAck;
   outMsg->header.length = sizeof(WriteStdioAckMessage);
   outMsg->header.returnCode = bgcios::Success;
   if (err != 0) {
      LOG_ERROR_MSG("error forwarding WriteStderr message to data channel: " << bgcios::errorString(err));
      outMsg->header.returnCode = bgcios::SendError;
      outMsg->header.errorCode = (uint32_t)err;
   }
   std::ostringstream monitorPath;
   monitorPath << bgcios::SimulationDirectory << _simId << "/job." << inMsg->header.jobId;
   return sendToCommandChannel(monitorPath.str(), outMsg);
}

int
SimStdioController::closeStdio(void)
{
   // Get pointer to inbound CloseStdio message.
   CloseStdioMessage *inMsg = (CloseStdioMessage *)_inboundMessage;

   // Forward message to the data channel.
   int err = sendToDataChannel(inMsg);

   // Send ack message to job monitor.
   CloseStdioAckMessage *outMsg = (CloseStdioAckMessage *)_outboundMessage;
   memcpy(&(outMsg->header), &(inMsg->header), sizeof(MessageHeader));
   outMsg->header.type = CloseStdioAck;
   outMsg->header.length = sizeof(CloseStdioAckMessage);
   outMsg->header.returnCode = bgcios::Success;
   if (err != 0) {
      LOG_ERROR_MSG("error forwarding CloseStdio message to data channel: " << bgcios::errorString(err));
      outMsg->header.returnCode = bgcios::SendError;
      outMsg->header.errorCode = (uint32_t)err;
   }
   std::ostringstream monitorPath;
   monitorPath << bgcios::SimulationDirectory << _simId << "/job." << inMsg->header.jobId;
   return sendToCommandChannel(monitorPath.str(), outMsg);
}

int
SimStdioController::startJob(void)
{
   // Get pointer to inbound StartJob message.
   StartJobMessage *inMsg = (StartJobMessage *)_inboundMessage;

   // Build StartJobAck message in outbound buffer.
   StartJobAckMessage *outMsg = (StartJobAckMessage *)_outboundMessage;
   memcpy(&(outMsg->header), &(inMsg->header), sizeof(bgcios::MessageHeader));
   outMsg->header.type = StartJobAck;
   outMsg->header.length = sizeof(StartJobAckMessage);
   outMsg->header.returnCode = Success;

   // Send StartJobAck message.
   return sendToDataChannel(&outMsg->header); 
}

int
SimStdioController::terminate(void)
{
   // Get pointer to Terminate message available from inbound buffer.
   bgcios::iosctl::TerminateMessage *inMsg = (bgcios::iosctl::TerminateMessage *)_inboundMessage;

   // Set flags to stop processing messages.
   _done = true;
   _terminated = true;

   // Build TerminateAck message in outbound buffer.
   bgcios::iosctl::TerminateAckMessage *outMsg = (bgcios::iosctl::TerminateAckMessage *)_outboundMessage;
   memcpy(&(outMsg->header), &(inMsg->header), sizeof(MessageHeader));
   outMsg->header.type = bgcios::iosctl::TerminateAck;
   outMsg->header.length = sizeof(bgcios::iosctl::TerminateAckMessage);
   outMsg->header.returnCode = bgcios::Success;

   // Send TerminateAck message.
   return sendToCommandChannel(_iosdCmdChannelPath, outMsg);
}

int
SimStdioController::changeConfig(void)
{
   // Get pointer to inbound ChangeConfig message.
   ChangeConfigMessage *inMsg = (ChangeConfigMessage *)_inboundMessage;

   // Build ChangeConfigAck message in outbound buffer.
   ChangeConfigAckMessage *outMsg = (ChangeConfigAckMessage *)_outboundMessage;
   memcpy(&(outMsg->header), &(inMsg->header), sizeof(MessageHeader));
   outMsg->header.type = ChangeConfigAck;
   outMsg->header.length = sizeof(ChangeConfigAckMessage);
   outMsg->header.returnCode = Success;

   // update logging levels
   setLoggingLevel( "ibm.cios", inMsg->commonTraceLevel );
   setLoggingLevel( "ibm.cios.stdiod", inMsg->stdiodTraceLevel );

   // Send ack
   return sendToDataChannel(&outMsg->header); 
}

int
SimStdioController::authenticate(InetSocketPtr channel)
{
   // Get pointer to inbound Authenticate message.
   AuthenticateMessage *inMsg = (AuthenticateMessage *)_inboundMessage;

   LOG_CIOS_TRACE_MSG( "plain: " << inMsg->plainData );
   std::ostringstream os;
   BOOST_FOREACH( const unsigned char i, inMsg->encryptedData ) {
       os << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned>(i);
   }
   LOG_CIOS_TRACE_MSG( "encrypted: " << os.str() );

   // Build AuthenticateAckMessage message in outbound buffer.
   AuthenticateAckMessage *outMsg = (AuthenticateAckMessage *)_outboundMessage;
   memcpy(&(outMsg->header), &(inMsg->header), sizeof(MessageHeader));
   outMsg->header.type = AuthenticateAck;
   outMsg->header.length = sizeof(AuthenticateAckMessage);
   outMsg->header.returnCode = Success;

   // Send ack
   return sendToDataChannel(&outMsg->header, channel); 
}

int
SimStdioController::reconnect(void)
{
   // Get pointer to inbound Reconnect message.
   ReconnectMessage *inMsg = (ReconnectMessage *)_inboundMessage;

   // Build ReconnectAck message in outbound buffer.
   ReconnectAckMessage *outMsg = (ReconnectAckMessage *)_outboundMessage;
   memcpy(&(outMsg->header), &(inMsg->header), sizeof(MessageHeader));
   outMsg->header.type = ReconnectAck;
   outMsg->header.length = sizeof(ReconnectAckMessage);
   outMsg->header.returnCode = Success;

   // Send ReconnectAck message on data channel.
   LOG_CIOS_DEBUG_MSG("Job " << inMsg->header.jobId << ": ReconnectAck message sent on data channel");
   return sendToDataChannel(outMsg);
}
