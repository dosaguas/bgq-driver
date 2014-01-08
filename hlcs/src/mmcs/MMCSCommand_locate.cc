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
 * \file MMCSCommand_locate.cc
 * \brief The locate command shows the addressing information for nodes and idos in a block.
 */

#include "BlockControllerBllInfo.h"
#include "BlockControllerIconInfo.h"
#include "BlockControllerNodeInfo.h"
#include "BlockControllerTarget.h"
#include "CNBlockController.h"
#include "IOBlockController.h"
#include "ConsoleController.h"
#include "MMCSCommandReply.h"
#include "MMCSCommand_locate.h"
#include "MMCSProperties.h"

#include <control/include/bgqconfig/BGQPersonality.h>
#include <control/include/bgqconfig/BGQWirePort.h>
#include <control/include/bgqconfig/BGQTopology.h>

#define NO_COORD (-1)


using namespace std;

// Helper method to dump location info for a single node.
static void
dumpLocationInfo(BlockPtr pBlock,
                 const char *msg,
                 ostream &ostr,
                 BCNodeInfo *nodeInfo,
                 bool bRASFormat,
                 bool ioBlock)
{
    if (!nodeInfo->_iopos.trainOnly()) {

        ostr << msg <<  "{" << nodeInfo->_locateId << "}\t";
        ostr << "<"
             << setw(2) << (uint16_t)nodeInfo->personality().Network_Config.Acoord << ","
             << setw(2) << (uint16_t)nodeInfo->personality().Network_Config.Bcoord << ","
             << setw(2) << (uint16_t)nodeInfo->personality().Network_Config.Ccoord << ","
             << setw(2) << (uint16_t)nodeInfo->personality().Network_Config.Dcoord << ","
             << setw(2) << (uint16_t)nodeInfo->personality().Network_Config.Ecoord << ">\t";


        if ( bRASFormat || ioBlock ) {
            ostr << "location: " << setw(16) << left << nodeInfo->location();
        } else {
            ostr << "in " << setw(6) << left << nodeInfo->midplanePos()
                 << " board "     << BGQTopology::nodeCardNameFromPos(nodeInfo->_pos.nodeCard())
                 << " card "      << BGQTopology::processorCardNameFromJtagPort(nodeInfo->_pos.jtagPort());
        }

        if (nodeInfo->_open)
            ostr << "\t target: open";
        else
            ostr << "\t target: closed";
    }
}

// Cheap func to increment a coord.  Makes code below just a little more readable.
static int
next_coord(unsigned start, unsigned size, bool torus)
{
    if (++start == size) {
	if (torus)
	    return 0;
    else
	    return NO_COORD;
    }
    return start;
}

// Cheap func to decrement a coord.  Makes code below just a little more readable.
static int
prev_coord(unsigned start, unsigned size, bool torus)
{
    if (start == 0) {
	if (torus)
	    return size-1;
	else
	    return NO_COORD;
    }
    return --start;
}

/*!
** build() - MMCSCommand factory
** This is invoked at MMCS startup when MMCS builds its list of commands
** @return an MMCSCommand object for this specific command
*/
MMCSCommand_locate*
MMCSCommand_locate::build()
{
    MMCSCommandAttributes commandAttributes;
    commandAttributes.requiresBlock(true);             // does require a BlockController object
    commandAttributes.requiresConnection(false);       // does not require  mc_server connections
    commandAttributes.requiresTarget(true);            // does require a BlockControllerTarget object
    commandAttributes.mmcsServerCommand(true);
    commandAttributes.mmcsLiteCommand(true);
    MMCSCommandAttributes::AuthPair blockread(hlcs::security::Object::Block, hlcs::security::Action::Read);
    commandAttributes.addAuthPair(blockread);
    return new MMCSCommand_locate("locate", "[<target>] locate [neighbors [verbose]] [ras_format] [summary]", commandAttributes);
}

void
MMCSCommand_locate::execute(
        deque<string> args,
        MMCSCommandReply& reply,
        ConsoleController* pController,
        BlockControllerTarget* pTarget
        )
{
    bool bNeighbors = false;
    bool bRASFormat = false;
    bool bVerbose = false;
    bool bSummary = false;

    for (unsigned i = 0; i < args.size(); ++i)
    {
        if (args[i] == "neighbors")  { bNeighbors   = true;    continue; }
        if (args[i] == "ras_format") { bRASFormat   = true;    continue; }
        if (args[i] == "verbose")    { bVerbose     = true;    continue; }
        if (args[i] == "summary")    { bSummary     = true;    continue; }
        reply << FAIL << "unrecognized argument: " << args[i] << DONE;
        return;
    }

    if ( bNeighbors && bSummary ) {
        reply << FAIL << "specify one of neighbors or summary." << DONE;
        return;
    } else if ( bVerbose && !bNeighbors ) {
        reply << FAIL << "specify verbose with neighbors." << DONE;
        return;
    }


    const BlockPtr pBlock = pController->getBlockBaseController();
    const CNBlockPtr compute_block = boost::dynamic_pointer_cast<CNBlockController>( pBlock );
    const IOBlockPtr io_block = boost::dynamic_pointer_cast<IOBlockController>( pBlock );
    const BGQBlockNodeConfig* bnc = pBlock->getBlockNodeConfig();
    bool ioBlock;
    if ( compute_block ) {
        ioBlock = false;
    } else if ( io_block ) {
        ioBlock = true;
    } else {
        reply << FAIL << "block is not compute or I/O" << DONE;
        return;
    }
    reply << OK;

    if ( bSummary ) {
        // summarize the block output
        if ( !ioBlock ) {
            if ( pTarget->getNodes().size() >= 512 ) {
                string mp = "none";
                for (unsigned i = 0; i < pTarget->getNodes().size(); ++i) {
                    BCNodeInfo *nodeInfo = pTarget->getNodes()[i];
                    if (!nodeInfo->_iopos.trainOnly() && mp != string(nodeInfo->midplanePos())) {
                        if ( mp != "none" ) reply << endl;
                        reply << nodeInfo->midplanePos();
                        mp = string(nodeInfo->midplanePos());
                    }
                }
            } else {
                string nb = "none";
                for (unsigned i = 0; i < pTarget->getNodes().size(); ++i) {
                    BCNodeInfo *nodeInfo = pTarget->getNodes()[i];
                    if (!nodeInfo->_iopos.trainOnly() && nb != (string(nodeInfo->midplanePos()) + string(BGQTopology::nodeCardNameFromPos(nodeInfo->_pos.nodeCard())))) {
                        if ( nb != "none" ) reply << endl;
                        reply << nodeInfo->midplanePos() << "-" << BGQTopology::nodeCardNameFromPos(nodeInfo->_pos.nodeCard());
                        nb = string(nodeInfo->midplanePos()) + string(BGQTopology::nodeCardNameFromPos(nodeInfo->_pos.nodeCard()));
                    }
                }
            }
        } else {
            string iob = "none";
            for (unsigned i = 0; i < pTarget->getNodes().size(); ++i) {
                BCNodeInfo *nodeInfo = pTarget->getNodes()[i];
                if ( iob != nodeInfo->ioboardNodeConfig()->posInMachine() ) {
                    if ( iob != "none" ) reply << endl;
                    iob = nodeInfo->ioboardNodeConfig()->posInMachine();
                    reply << iob;
                }
            }
        }

        reply << DONE;
        return;
    }

    for (unsigned i = 0; i < pTarget->getNodes().size(); ++i) {
        BCNodeInfo *nodeInfo = pTarget->getNodes()[i];
        ostringstream ostr;

        if ( !nodeInfo->_iopos.trainOnly() ) {
            dumpLocationInfo(pBlock,"", ostr, nodeInfo, bRASFormat, ioBlock);
            string fromPos(nodeInfo->midplanePos());
            if (bNeighbors && (!ioBlock || !MMCSProperties::getProperty("ioTorus").empty())) {
                int coord;
                char linkInfo[256];
                ostr << '\n';
                BCNodeInfo *nb;
                // For torus just search for nodes in the +/- directions

                // A+
                coord = next_coord(nodeInfo->personality().Network_Config.Acoord,
                                   nodeInfo->personality().Network_Config.Anodes,
                                   nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_A);

                if (coord != NO_COORD) {
                    nb = pBlock->findNodeInfo(coord,
                                              nodeInfo->personality().Network_Config.Bcoord,
                                              nodeInfo->personality().Network_Config.Ccoord,
                                              nodeInfo->personality().Network_Config.Dcoord,
                                              nodeInfo->personality().Network_Config.Ecoord);

                    if (nb) {
                        dumpLocationInfo(pBlock,"    A+ ", ostr, nb, bRASFormat, ioBlock);
                        if (bVerbose) {
                            BGQTopology::neighborInfo(nodeInfo->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                      fromPos,nb->midplanePos(),
                                                      (coord == 0) , "A+", linkInfo);
                            ostr << "\t" << linkInfo;
                        }
                        ostr << '\n';
                    }
                }

                // A-
                coord = prev_coord(nodeInfo->personality().Network_Config.Acoord,
                                   nodeInfo->personality().Network_Config.Anodes,
                                   nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_A);

                if (coord != NO_COORD) {
                    nb = pBlock->findNodeInfo(coord,
                                              nodeInfo->personality().Network_Config.Bcoord,
                                              nodeInfo->personality().Network_Config.Ccoord,
                                              nodeInfo->personality().Network_Config.Dcoord,
                                              nodeInfo->personality().Network_Config.Ecoord);

                    if (nb) {
                        dumpLocationInfo(pBlock,"    A- ", ostr, nb, bRASFormat, ioBlock);
                        if (bVerbose) {
                            BGQTopology::neighborInfo(nb->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                      fromPos,nb->midplanePos(),
                                                      (nodeInfo->personality().Network_Config.Acoord == 0) , "A-", linkInfo);
                            ostr << "\t" << linkInfo;
                        }
                        ostr << '\n';
                    }
                }

                // B+
                coord = next_coord(nodeInfo->personality().Network_Config.Bcoord,
                                   nodeInfo->personality().Network_Config.Bnodes,
                                   nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_B);

                if (coord != NO_COORD) {
                    nb = pBlock->findNodeInfo(nodeInfo->personality().Network_Config.Acoord,
                                              coord,
                                              nodeInfo->personality().Network_Config.Ccoord,
                                              nodeInfo->personality().Network_Config.Dcoord,
                                              nodeInfo->personality().Network_Config.Ecoord);
                    if (nb) {
                        dumpLocationInfo(pBlock,"    B+ ", ostr, nb, bRASFormat, ioBlock);
                        if (bVerbose) {
                            BGQTopology::neighborInfo(nodeInfo->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                      fromPos,nb->midplanePos(),
                                                      (coord == 0) , "B+", linkInfo);
                            ostr << "\t" << linkInfo;
                        }
                        ostr << '\n';
                    }
                }

                // B-
                coord = prev_coord(nodeInfo->personality().Network_Config.Bcoord,
                                   nodeInfo->personality().Network_Config.Bnodes,
                                   nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_B);

                if (coord != NO_COORD) {
                    nb = pBlock->findNodeInfo(nodeInfo->personality().Network_Config.Acoord,
                                              coord,
                                              nodeInfo->personality().Network_Config.Ccoord,
                                              nodeInfo->personality().Network_Config.Dcoord,
                                              nodeInfo->personality().Network_Config.Ecoord);
                    if (nb) {
                        dumpLocationInfo(pBlock,"    B- ", ostr, nb, bRASFormat, ioBlock);
                        if (bVerbose) {
                            BGQTopology::neighborInfo(nb->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                      fromPos,nb->midplanePos(),
                                                      (nodeInfo->personality().Network_Config.Bcoord == 0) , "B-", linkInfo);
                            ostr << "\t" << linkInfo;
                        }
                        ostr << '\n';
                    }
                }

                // C+
                coord = next_coord(nodeInfo->personality().Network_Config.Ccoord,
                                   nodeInfo->personality().Network_Config.Cnodes,
                                   nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_C);

                if (coord != NO_COORD) {
                    nb = pBlock->findNodeInfo(nodeInfo->personality().Network_Config.Acoord,
                                              nodeInfo->personality().Network_Config.Bcoord,
                                              coord,
                                              nodeInfo->personality().Network_Config.Dcoord,
                                              nodeInfo->personality().Network_Config.Ecoord);
                    if (nb) {
                        dumpLocationInfo(pBlock,"    C+ ", ostr, nb, bRASFormat, ioBlock);
                        if (bVerbose) {
                            BGQTopology::neighborInfo(nodeInfo->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                      fromPos,nb->midplanePos(),
                                                      (coord == 0) , "C+", linkInfo);
                            ostr << "\t" << linkInfo;
                        }
                        ostr << '\n';
                    }
                }

                // C-
                coord = prev_coord(nodeInfo->personality().Network_Config.Ccoord,
                                   nodeInfo->personality().Network_Config.Cnodes,
                                   nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_C);

                if (coord != NO_COORD) {
                    nb = pBlock->findNodeInfo(nodeInfo->personality().Network_Config.Acoord,
                                              nodeInfo->personality().Network_Config.Bcoord,
                                              coord,
                                              nodeInfo->personality().Network_Config.Dcoord,
                                              nodeInfo->personality().Network_Config.Ecoord);
                    if (nb) {
                        dumpLocationInfo(pBlock,"    C- ", ostr, nb, bRASFormat, ioBlock);
                        if (bVerbose) {
                            BGQTopology::neighborInfo(nb->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                      fromPos,nb->midplanePos(),
                                                      (nodeInfo->personality().Network_Config.Ccoord == 0) , "C-", linkInfo);
                            ostr << "\t" << linkInfo;
                        }
                        ostr << '\n';
                    }
                }

                if (!ioBlock) {
                    // D+
                    coord = next_coord(nodeInfo->personality().Network_Config.Dcoord,
                                       nodeInfo->personality().Network_Config.Dnodes,
                                       nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_D);

                    if (coord != NO_COORD) {
                        nb = pBlock->findNodeInfo(nodeInfo->personality().Network_Config.Acoord,
                                                  nodeInfo->personality().Network_Config.Bcoord,
                                                  nodeInfo->personality().Network_Config.Ccoord,
                                                  coord,
                                                  nodeInfo->personality().Network_Config.Ecoord);
                        if (nb) {
                            dumpLocationInfo(pBlock,"    D+ ", ostr, nb, bRASFormat, ioBlock);
                            if (bVerbose) {
                                BGQTopology::neighborInfo(nodeInfo->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                          fromPos,nb->midplanePos(),
                                                          (coord == 0) , "D+", linkInfo);
                                ostr << "\t" << linkInfo;
                            }
                            ostr << '\n';
                        }
                    }

                    // D-
                    coord = prev_coord(nodeInfo->personality().Network_Config.Dcoord,
                                       nodeInfo->personality().Network_Config.Dnodes,
                                       nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_D);

                    if (coord != NO_COORD) {
                        nb = pBlock->findNodeInfo(nodeInfo->personality().Network_Config.Acoord,
                                                  nodeInfo->personality().Network_Config.Bcoord,
                                                  nodeInfo->personality().Network_Config.Ccoord,
                                                  coord,
                                                  nodeInfo->personality().Network_Config.Ecoord);
                        if (nb) {
                            dumpLocationInfo(pBlock,"    D- ", ostr, nb, bRASFormat, ioBlock);
                            if (bVerbose) {
                                BGQTopology::neighborInfo(nb->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                          fromPos,nb->midplanePos(),
                                                          (nodeInfo->personality().Network_Config.Dcoord == 0) , "D-", linkInfo);
                                ostr << "\t" << linkInfo;
                            }
                            ostr << '\n';
                        }
                    }

                    // E+
                    coord = next_coord(nodeInfo->personality().Network_Config.Ecoord,
                                       nodeInfo->personality().Network_Config.Enodes,
                                       nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_E);

                    if (coord != NO_COORD) {
                        nb = pBlock->findNodeInfo(nodeInfo->personality().Network_Config.Acoord,
                                                  nodeInfo->personality().Network_Config.Bcoord,
                                                  nodeInfo->personality().Network_Config.Ccoord,
                                                  nodeInfo->personality().Network_Config.Dcoord,
                                                  coord);
                        if (nb) {
                            dumpLocationInfo(pBlock,"    E+ ", ostr, nb, bRASFormat, ioBlock);
                            if (bVerbose) {
                                BGQTopology::neighborInfo(nodeInfo->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                          fromPos,nb->midplanePos(),
                                                          false , "E+", linkInfo);
                                ostr << "\t" << linkInfo;
                            }
                            ostr << '\n';
                        }
                    }

                    // E-
                    coord = prev_coord(nodeInfo->personality().Network_Config.Ecoord,
                                       nodeInfo->personality().Network_Config.Enodes,
                                       nodeInfo->personality().Network_Config.NetFlags & ND_ENABLE_TORUS_DIM_E);

                    if (coord != NO_COORD) {
                        nb = pBlock->findNodeInfo(nodeInfo->personality().Network_Config.Acoord,
                                                  nodeInfo->personality().Network_Config.Bcoord,
                                                  nodeInfo->personality().Network_Config.Ccoord,
                                                  nodeInfo->personality().Network_Config.Dcoord,
                                                  coord);
                        if (nb) {
                            dumpLocationInfo(pBlock,"    E- ", ostr, nb, bRASFormat, ioBlock);
                            if (bVerbose) {
                                BGQTopology::neighborInfo(nb->_pos.jtagPort(), nodeInfo->_pos.nodeCard(), nb->_pos.nodeCard(),
                                                          fromPos,nb->midplanePos(),
                                                          false , "E-", linkInfo);
                                ostr << "\t" << linkInfo;
                            }
                            ostr << '\n';
                        }
                    }

                    // include the connected IO node
                    ostr << "    Connected IO: " << bnc->connectedIONode(nodeInfo->location()) << "\n";
                }
            }
            reply << ostr.str() << '\n';
        }
    }

    // location information on icon cards
    for (unsigned i = 0; i < pTarget->getIcons().size(); ++i)
        {
            BCIconInfo *iconInfo = pTarget->getIcons()[i];
            ostringstream ostr;
            // if this is an io location in a compute block, skip it, since its included only for training
            if (iconInfo->_ioboard == false  || ioBlock) {
                ostr << "{" << iconInfo->_locateId << "}\t";
                if (iconInfo->_ioboard)
                    ostr << setw(10) << left << "<io>" << "\t";
                else if (typeid(*iconInfo) == typeid(BCNodecardInfo))
                    ostr << setw(10) << left << "<nc>" << "\t";
                else if (typeid(*iconInfo) == typeid(BCServicecardInfo))
                    ostr << setw(10) << left << "<sc>" << "\t";
                else if (typeid(*iconInfo) == typeid(BCClockcardInfo))
                    ostr << setw(10) << left << "<cc>" << "\t";
                if (bRASFormat || ioBlock)
                    {
                        ostr << "location: " << setw(16) << left << iconInfo->location();
                    }
                else
                    {
                        ostr << "in " << setw(6) << left << iconInfo->midplanePos();
                        ostr << " board "     << iconInfo->cardName();
                    }
                if (iconInfo->_open)
                    ostr << "\t target: open";
                else
                    ostr << "\t target: closed";
                reply << ostr.str() << '\n';
            }
        }


    // location information on link chips
    for (unsigned i = 0; i < pTarget->getLinkchips().size(); ++i)
        {
            ostringstream ostr;
            BCLinkchipInfo *linkchipInfo = pTarget->getLinkchips()[i];

            ostr << "{" << linkchipInfo->_locateId << "}\t<l>\t";
            if (bRASFormat) {
                ostr << "location: " << setw(16) << left << linkchipInfo->location();
            } else {
                ostr << "in " << setw(6) << left << linkchipInfo->midplanePos()
                     << " chip "  << linkchipInfo->linkchipPos();
            }
            ostr << "\troutes:";
            // 	for (unsigned j = 0; j < linkchipInfo->_chipRoute.size(); ++j)
            // 	{
            // 	    ostr << " " << BGQTopology::linkChipPortNameFromLinkChipPort(linkchipInfo->_chipRoute[j]._fromPort)
            // 		 << "-" << BGQTopology::linkChipPortNameFromLinkChipPort(linkchipInfo->_chipRoute[j]._toPort);
            // 	}
            if (linkchipInfo->_open)
                ostr << "\t target: open";
            else
                ostr << "\t target: closed";
            reply << ostr.str() << '\n';
        }
    reply << DONE;
}

void
MMCSCommand_locate::help(
        deque<string> args,
        MMCSCommandReply& reply
        )
{
    // the first data written to the reply stream should be 'OK' or 'FAIL'
    reply << OK << description()
        << ";lists physical location (midplane, board, card, slot) of all nodes in the selected block. "
        << ";If target node is specified, lists the location of the specified node. "
        << ";Add 'neighbors' to get the + and - neighbors in a,b,c,d,e dimensions, and the connected"
        << ";  I/O node, for a compute block. Add verbose to see how the connection is made."
        << ";Add 'ras_format' to get location information in the format found in RAS events."
        << ";Add 'summary' to get summary board or midplane information instead of node locations."
        << DONE;
}
