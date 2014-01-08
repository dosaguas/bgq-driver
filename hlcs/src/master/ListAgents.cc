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

#include "ListAgents.h"
#include "BinaryController.h"

LOG_DECLARE_FILE( "master" );

void ListAgents::doListAgents(BGMasterClient& client, bool agents_only, bool fancy) {
    LOG_DEBUG_MSG(__FUNCTION__);
    std::map<BGAgentId, std::vector<BinaryControllerPtr>, Id::Comp > agentmap;
    client.get_agents(agentmap);

    LOG_DEBUG_MSG("Sending list_agents command");

    bool firstbin = true;
    bool firstagent = true;
    bool foundbin = false;

    // Loop through the map and print the agents and associated binaries
    for(std::map<BGAgentId, std::vector<BinaryControllerPtr>, Id::Comp >::iterator it = agentmap.begin();
        it != agentmap.end(); ++it) {
        // Get the Id string
        BGAgentId* id = const_cast<BGAgentId*>(&(it->first));
        std::string idstr = *id;

        if(!agents_only) {
            // Now iterate through the binary vector
            typedef std::vector<BinaryControllerPtr> BinVector;
            BinVector vec = it->second;
            if(vec.size() == 0) {
                std::cout << "No binaries running on agent " << id->get_host().uhn() << "." << std::endl;
            }
            for(BinVector::iterator bin_it = vec.begin();
                bin_it != vec.end(); ++bin_it) {
                foundbin = true;
                if(fancy) {
                    if(firstbin) {
                        std::cout << std::endl;
                        std::cout << std::left << std::setw(17) << "Agent Host" << std::setw(22) << "Binary ID (IP:PID)" << std::setw(17) << "Alias name" << std::setw(13) << "Status" << std::setw(11) << "User" << std::setw(12) << "Start Time" << std::endl;
                        std::cout << std::left << std::setw(17) << "----------" << std::setw(22) << "------------------" << std::setw(17) << "----------" << std::setw(13) << "------" << std::setw(11) << "----" << std::setw(12) << "----------" << std::endl;
                        firstbin = false;
                    }
                    std::cout << std::left << std::setw(17) << id->get_host().uhn().substr(0,16) << std::setw(22) << (*bin_it)->get_binid().str().substr(0,21)  << std::setw(17) << (*bin_it)->get_alias_name().substr(0,16) << std::setw(13) << BinaryController::status_to_string((*bin_it)->get_status()) << std::setw(11) << (*bin_it)->get_user().substr(0,10) << std::setw(11) << (*bin_it)->get_start_time() << std::endl;
                } else {
                    std::cout << idstr << "|" << (*bin_it)->get_binid().str() << ":" << (*bin_it)->get_alias_name()
                              << "|" << BinaryController::status_to_string((*bin_it)->get_status()) << "|"
                              << (*bin_it)->get_user() << "|" << (*bin_it)->get_start_time() << std::endl;
                }
            }
        }
    }

    // Now just the agents
    for(std::map<BGAgentId, std::vector<BinaryControllerPtr>, Id::Comp >::iterator it = agentmap.begin();
        it != agentmap.end(); ++it) {
        BGAgentId* id = const_cast<BGAgentId*>(&(it->first));
        std::string idstr = *id;
        if(!fancy) {
            if(!foundbin)
                std::cout << id->get_host().fqhn() << " " << idstr << std::endl;
        } else {
            if(firstagent) {
                std::cout << std::endl;
                std::cout << std::left << std::setw(36) << "Agent Host" << std::setw(22) << "Agent ID (IP:Port)" << std::endl;
                std::cout << std::left << std::setw(36) << "----------" << std::setw(22) << "------------------" << std::endl;
                firstagent = false;
            }
            std::cout << std::left << std::setw(36) << id->get_host().fqhn().substr(0,35) << std::setw(22) << idstr.substr(0,21) << std::endl;
        }
    }
}
