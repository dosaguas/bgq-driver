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

/*!
 * \file bgsched/Block.h
 * \brief Block class definition.
 */

#ifndef BGSCHED_BLOCK_H_
#define BGSCHED_BLOCK_H_

#include <bgsched/Dimension.h>
#include <bgsched/EnumWrapper.h>
#include <bgsched/IOLink.h>
#include <bgsched/Job.h>
#include <bgsched/Midplane.h>
#include <bgsched/types.h>

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <map>
#include <vector>

namespace bgsched {

/*!
 * \brief Represents a block of any size.
 *
 * \image html block_state.png
 */
class Block : boost::noncopyable
{
public:

    /*!
     * \brief Each dimension can be connected as either a Torus or a Mesh.
     */
    struct Connectivity {
        enum Value {
            Torus,  //!< Torus connectivity
            Mesh    //!< Mesh connectivity
        };
    };

    typedef boost::shared_ptr<Block> Ptr;                  //!< Pointer type.
    typedef boost::shared_ptr<const Block> ConstPtr;       //!< Const pointer type.

    typedef std::vector<Ptr> Ptrs;                         //!< Collection of pointers.
    typedef std::vector<ConstPtr> ConstPtrs;               //!< Collection of const pointers.

    typedef std::vector<std::string> Midplanes;            //!< Collection of midplane location strings.
    typedef std::vector<std::string> NodeBoards;           //!< Collection of node board location strings.
    typedef std::vector<std::string> PassthroughMidplanes; //!< Collection of pass-through midplane location strings.
    typedef std::map<bgsched::Dimension, Connectivity::Value> DimensionConnectivity; //!< Connectivity in each dimension.

    /*!
     * \brief Block status.
     */
    enum Status {
        Allocated = 0,  //!< Block is allocated
        Booting,        //!< Block is booting
        Free,           //!< Block is free
        Initialized,    //!< Block is initialized
        Terminating     //!< Block is terminating
    };

    /*!
     * \brief Block action.
     */
    struct Action {
        enum Value {
            None = 0,       //!< No block action
            Boot,           //!< Boot block action
            Free            //!< Free block action
        };
    };

    /*!
     * \brief Create a large block object. This method creates an in-memory block but does not add it
     * to the database. To add the block to the database use the Block::add method.
     *
     * If the dimension connectivity is a Mesh, the Mesh will start at the first midplane location given.
     *
     * If the block consists of all the machine midplanes (full block) or a single midplane then the
     * connectivity must be Torus for all dimensions. Any dimension with a single midplane must also be Torus.
     * Any dimension that is equal to the size of the machine in that dimension must also be Torus.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidMidplanes - if one or more midplanes are invalid
     * - bgsched::InputErrors::InvalidConnectivity - if connectivity setting is not valid
     * - bgsched::InputErrors::BlockNotCreated - if block can not be created from input arguments
     *
     * \return Pointer to compute block object.
     */
    static Block::Ptr create(
            const Midplanes& midplanes,                         //!< [in] Midplane(s)
            const PassthroughMidplanes& passthroughMidplanes,   //!< [in] Pass-through midplane(s) (if any)
            const DimensionConnectivity& dimensionConnectivity  //!< [in] Connectivity (Torus/Mesh) for each dimension (A B C D)
    );

    /*!
     * \brief Create a small block object based on node board locations. This method creates an in-memory block but does not add it
     * to the database. To add the block to the database use the Block::add method.
     *
     * Note: An I/O link must exist on at least one of the node boards in the block.
     *
     * \throws bgsched::InputException with value:
     * - bgsched::InputErrors::InvalidNodeBoards - if one or more node boards are invalid
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::BlockCreateError - if error retrieving I/O links
     *
     * \return Pointer to compute block object.
     */
    static Block::Ptr create(
            const NodeBoards& nodeBoards  //!< [in] Vector of node board locations
    );

    /*!
     * \brief Create a small block object using starting node board location and number of node boards. This method creates an
     * in-memory block but does not add it to the database. To add the block to the database use the Block::add() method.
     *
     * Note: An I/O link must exist on at least one of the node boards in the block.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidNodeBoards - if node board location is invalid
     * - bgsched::InputErrors::InvalidNodeBoardCount - if number of node boards is invalid (valid range is 1 to 8)
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::BlockCreateError - if error retrieving I/O links
     *
     * \return Pointer to compute block object.
     */
    static Block::Ptr create(
            const std::string& nodeBoardStartLocation,  //!< [in] Node board starting location
            const uint8_t nodeBoardCount                //!< [in] Number of node boards to include from starting location
    );

    /*!
     *  \brief An I/O block can be booted when some I/O nodes are not available. This method will check if the minimum I/O is
     *  available for the compute block to boot. For small blocks at least one I/O link must be "available" and have a
     *  connected (booted) I/O node. For large blocks each midplane must have at least two "available" I/O links with
     *  connected (booted) I/O nodes. The I/O links rule is based on the requirement of at most a 1:256 compute to I/O ratio.
     *
     *  Users of this method should note that even though unconnected I/O nodes were discovered the compute block may still be
     *  booted as long as all midplanes pass the I/O requirements check.
     *
     *  The block must exist in the database.
     *
     * \note This method was added in V1R1M1.
     * \ingroup V1R1M1
     *
     * * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs accessing the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found
     *
     * \throws bgsched::InternalException with value:
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     *
     * \return A vector of unconnected I/O nodes for the block, or an empty vector if all I/O nodes are connected.
     * Also returns a vector of midplanes that fail to meet the minimum I/O requirements to successfully boot the
     * compute block. An empty vector is returned if all midplanes meet the minimum I/O requirements.
     */
    static void checkIO(
            const std::string& blockName,                       //!< [in]  Compute block name
            std::vector<std::string>* unconnectedIONodes,       //!< [out] Unconnected I/O node locations
            std::vector<std::string>* midplanesFailingIORules   //!< [out] Midplanes failing I/O requirements
            );

    /*!
     *  \brief Returns true if all of the connected (and available) I/O nodes for the compute block are booted.
     *  If false the unconnected I/O nodes are returned.
     *
     *  The block must exist in the database.
     *
     * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs accessing the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found
     *
     * \throws bgsched::InternalException with value:
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     *
     * \return True if all of the connected (and available) I/O nodes for the compute block are booted.
     */
    static bool isIOConnected(
            const std::string& blockName,                 //!< [in]  Compute block name
            std::vector<std::string>* unconnectedIONodes  //!< [out] Unconnected I/O node locations
            );

    /*!
     * \brief Check I/O links for a compute block. The compute block must exist in the database.
     *
     * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs accessing the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found
     *
     * \throws bgsched::InternalException with values:
     * - bgsched::InternalErrors::XMLParseError - if internal XML format is bad
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     *
     * \return All (both available and unavailable) I/O links for a compute block. If an empty vector is returned
     * this implies that no I/O links exist for the compute block.
     */
    static IOLink::ConstPtrs checkIOLinks(
            const std::string& blockName       //!< [in] Compute block name
            );

    /*!
     * \brief Check "available" I/O links for a compute block. The compute block must exist in the database. An I/O link
     * is considered "available" if both the I/O link hardware state is available and the destination I/O node is also
     * available.
     *
     * \note This method was added in V1R1M1.
     * \ingroup V1R1M1
     *
     * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs accessing the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found
     *
     * \throws bgsched::InternalException with values:
     * - bgsched::InternalErrors::XMLParseError - if internal XML format is bad
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     *
     * \return Available I/O links for a compute block. If an empty vector is returned this implies that no I/O links
     * are "available" for the compute block.
     */
    static IOLink::ConstPtrs checkAvailableIOLinks(
            const std::string& blockName       //!< [in] Compute block name
            );

    /*!
     * \brief Check I/O links for a block and return a summary of the total number of available I/O
     * links and the total number of unavailable I/O links. An I/O link is considered "available" if both the
     * the I/O link hardware resource is available and the destination I/O node is also available. This interface
     * provides a faster interface for finding the number of available I/O links for a block than checkIOLinks().
     * If detailed I/O link information is needed for a block than checkIOLinks() should be used instead.
     *
     * The block must exist in the database.
     *
     * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs accessing the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found
     *
     * \throws bgsched::InternalException with values:
     * - bgsched::InternalErrors::XMLParseError - if internal XML format is bad
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     */
    static void checkIOLinksSummary(
            const std::string& blockName,       //!< [in]  Compute block name
            uint32_t *availableIOLinks,         //!< [out] Count of available I/O links
            uint32_t *unavailableIOLinks        //!< [out] Count of unavailable I/O links
            );

    /*!
     * \brief Initiate the booting of a block.
     *
     * The block must exist in the database.
     *
     * The block must have a status of Free or an exception is thrown.
     * The boot process is done asynchronously so the status of the block
     * must be checked to determine if the block was successfully booted.
     *
     * The block owner will be set to the user calling the method.
     *
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::BlockBootError - if block status is not Free or block action already pending or hardware in use already
     *
     * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs accessing the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found
     *
     * \throws bgsched::InternalException with value:
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     */
    static void initiateBoot(
            const std::string& blockName       //!< [in] Compute block name
    );

    /*!
     * \brief Initiate the freeing of a block.
     *
     * The block must exist in the database.
     *
     * The freeing of the block is done asynchronously so the status of the block
     * must be checked to determine if the block was successfully freed.
     *
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::BlockFreeError - if block has block action already pending
     *
     * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs accessing the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found
     *
     * \throws bgsched::InternalException with value:
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     */
    static void initiateFree(
            const std::string& blockName       //!< [in] Compute block name
    );

    /*!
     * \brief Remove block from database.
     *
     * A block can only be deleted if its status is Free.
     *
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::InvalidBlockState - if block status is not Free
     *
     * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs accessing the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found
     *
     * \throws bgsched::InternalException with value:
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     */
    static void remove(
            const std::string& blockName  //!< [in] Compute block name
    );

    /*!
     * \brief Add user permission to run jobs on block.
     *
     * The block must exist in the database.
     *
     * This method will add the specified user so they have permission to run jobs on the block.
     *
     * Note: Attempting to add a user who is already authorized to the block does not cause any
     * exception to be thrown.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if block name is invalid
     * - bgsched::InputErrors::InvalidUser - if the user name is invalid
     *
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::AuthorityError - if error adding user to block
     */
    static void addUser(
            const std::string& blockName, //!< [in] Compute block name
            const std::string& user       //!< [in] User to add permissions to compute block
    );

    /*!
     * \brief Remove user permission to run jobs on block.
     *
     * The block must exist in the database.
     *
     * This method will remove the specified user so they no longer have permission to run jobs
     * on the block.
     *
     * Note: Attempting to remove a user who was not authorized to the block does not cause any
     * exception to be thrown.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if block name is invalid
     * - bgsched::InputErrors::InvalidUser - if the user name is invalid
     *
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::AuthorityError - if error removing user from block
     */
    static void removeUser(
            const std::string& blockName, //!< [in] Compute block name
            const std::string& user       //!< [in] User to remove permissions from compute block
    );

    /*!
     * \brief Returns true if specified user has permissions to run jobs on block.
     *
     * The block must exist in the database.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if block name is invalid
     * - bgsched::InputErrors::InvalidUser - if the user name is invalid
     *
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::AuthorityError - if error retrieving compute block permissions
     *
     * \return True if specified user has permissions to run jobs on block.
     */
    static bool isAuthorized(
            const std::string& blockName, //!< [in] Compute block name
            const std::string& user       //!< [in] User to check permissions to compute block
    );

    /*!
     * \brief Get list of users with permission to run jobs on block.
     *
     * The block must exist in the database.
     *
     * This method will return all users who have "Execute" permission to the block.
     *
     * \throws bgsched::InputException with value:
     * - bgsched::InputErrors::InvalidBlockName - if block name is invalid
     *
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::AuthorityError - if error retrieving compute block permissions
     */
    static std::vector<std::string> getUsers(
            const std::string& blockName  //!< [in] Compute block name
    );

    /*!
     * \brief Add block to database.
     *
     * This method will add the block to the database. If the owner string is empty the user id
     * of the caller will be used as the block owner.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::BlockNotAdded - if the block was not added to the database
     * - bgsched::InputErrors::InvalidMidplanes - if large block and no midplanes found
     *
     * \throws bgsched::RuntimeException with value:
     * - bgsched::RuntimeErrors::BlockAddError - if error setting block owner
     *
     * \throws bgsched::DatabaseException with value:
     * - bgsched::DatabaseErrors::OperationFailed - if a database operation failed
     */
    void add(
            const std::string& owner  //!< [in] Compute block owner
    );

    /*!
     * \brief Update block in database.
     *
     * This method will update select block fields in the database based on current settings
     * in the Block object. The following fields are updated: description, boot options,
     * micro-loader image, Node configuration and options.
     *
     * Note: The updated node configuration name must exist in the BG/Q database or a bgsched::DatabaseException
     * is thrown.
     *
     * \throws bgsched::DatabaseException with values:
     * - bgsched::DatabaseErrors::DatabaseError - if error occurs updating the database
     * - bgsched::DatabaseErrors::ConnectionError - if error occurs connecting to the database
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid
     * - bgsched::InputErrors::BlockNotFound - if block name was not found or block is not Free
     * - bgsched::InputErrors::InvalidBlockInfo - if options, boot options, micro-loader image or Node configuration is not valid
     * - bgsched::InputErrors::InvalidBlockDescription - if description is not valid
     *
     * \throws bgsched::InternalException with value:
     * - bgsched::InternalErrors::UnexpectedError - if any type of unexpected error occurs
     */
    void update();

    /*!
     * \brief Block object represented as a human readable string value.
     *
     * \return Compute block object represented as a human readable string value.
     */
    std::string toString(
            bool verbose    //!< [in] true for extended block info, false for brief block info
    ) const;

    /*!
     * \brief Returns true if block is large (midplane or larger).
     *
     * \return True if compute block is large (midplane or larger).
     */
    bool isLarge() const;

    /*!
     * \brief Returns true if block is small (smaller than a midplane).
     *
     * \return True if compute block is small (smaller than a midplane).
     */
    bool isSmall() const;

    /*!
     * \brief Get the block dimension size in midplanes. This only returns dimensions for large blocks.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidDimension - if dimension is not A, B, C or D
     * - bgsched::InputErrors::InvalidBlockSize - if small block
     *
     * \return Size in midplanes for the given dimension.
     */
    uint32_t getDimensionSize(
            const Dimension dimension   //!< [in] Dimension, A-D
    ) const;

    /*!
     * \brief Returns true if block is Torus in specified dimension. This only returns information for large blocks.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidDimension - if dimension is not A, B, C or D
     * - bgsched::InputErrors::InvalidBlockSize - if small block
     *
     * \return True if compute block is Torus in specified dimension.
     */
    bool isTorus(
            const Dimension dimension     //!< [in] Dimension, A-D
    ) const;

    /*!
     * \brief Returns true if block is Mesh in specified dimension. This only returns information for large blocks.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidDimension - if dimension is not A, B, C or D
     * - bgsched::InputErrors::InvalidBlockSize - if small block
     *
     * \return True if compute block is Mesh in specified dimension.
     */
    bool isMesh(
            const Dimension dimension     //!< [in] Dimension, A-D
    ) const;

    /*!
     * \brief Get the block name (Id).
     *
     * This is the block's unique name.
     *
     * \return Compute block name.
     */
    const std::string& getName() const;

    /*!
     * \brief Get the block creation identifier.
     *
     * A unique creation identifier is automatically generated when a block is added to
     * persistent storage (database). The id is stored in the block history table and also in the
     * job history table for any job run on the block. The creation identifier associates a job with
     * the exact block it was run on.
     *
     * \return Compute block creation Id.
     */
    BlockCreationId getCreationId() const;

    /*!
     * \brief Get the block description.
     *
     * \return Compute block description.
     */
    const std::string& getDescription() const;

    /*!
     * \brief Get the block owner.
     *
     * \return Compute block owner.
     */
    const std::string& getOwner() const;

    /*!
     * \brief Get the block user.
     *
     * \return Compute block user.
     */
    const std::string& getUser() const;

    /*!
     * \brief Get the block boot options.
     *
     * \return Compute block boot options.
     */
    const std::string& getBootOptions() const;

    /*!
     * \brief Get the block microloader image.
     *
     * \return Compute block microloader image.
     */
    const std::string& getMicroLoaderImage() const;

    /*!
     * \brief Get the block Node configuration.
     *
     * Note: Customized compute block Node configurations are created by System Administrators using the "create_node_config"
     * command from the bg_console. The default Node configuration is named "CNKDefault".
     *
     * \return Compute block Node configuration.
     */
    const std::string& getNodeConfiguration() const;

    /*!
     * \brief Get the block sequence ID.
     *
     * \return Compute block sequence ID.
     */
    SequenceId getSequenceId() const;

    /*!
     * \brief Get the block status.
     *
     * This is the block status, not necessarily the status of the underlying hardware.
     *
     * \return Compute block status.
     */
    EnumWrapper<Status> getStatus() const;

    /*!
     * \brief Get the block action.
     *
     * \note This method was added in V1R1M1.
     * \ingroup V1R1M1
     *
     * \return Compute block action.
     */
    EnumWrapper<Action::Value> getAction() const;

    /*!
     * \brief Get the number of compute nodes in the block.
     *
     * \return Number of compute nodes in the compute block.
     */
    uint32_t getComputeNodeCount() const;

    /*!
     * \brief Get the block options.
     *
     * \return Compute block options.
     */
    const std::string& getOptions() const;

    /*!
     * \brief Get the job IDs associated with the block.
     *
     * \return Job IDs associated with the compute block.
     */
    std::vector<Job::Id> getJobIds() const;

    /*!
     * \brief Get the midplane locations this block uses.
     *
     * The returned midplanes don't include the pass-through midplanes for a large block. Use getPassthroughMidplanes()
     * to obtain any pass-through midplanes.
     *
     * The returned vector may be empty if this block was queried from core::getBlocks and extended information wasn't requested.
     *
     * \return Midplane locations the compute block uses.
     */
    Midplanes getMidplanes() const;

    /*!
     * \brief Get the node board locations this block uses.
     *
     * There are two cases where this will be an empty vector:
     * - The block is a midplane or larger.
     * - The block was created using core::getBlocks and extended information wasn't requested.
     *
     * \return Node board locations the compute block uses.
     */
    NodeBoards getNodeBoards() const;

    /*!
     * \brief Get the midplane locations that this block uses for pass-through.
     *
     * \return Midplane locations the compute block uses for pass-through.
     */
    PassthroughMidplanes getPassthroughMidplanes() const;

    /*!
     * \brief Set the compute block name (Id).
     *
     * This is a user-defined name limited to 32 characters. The name must be unique.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockName - if the block name is not valid (empty or exceeds 32 characters)
     */
    void setName(
            const std::string& name       //!< [in] Compute block name
    );

    /*!
     * \brief Set the block description.
     *
     * This is a user-defined description limited to 1024 characters, e.g. "Joe's test block"
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockDescription - if the block description is not valid (empty or exceeds 1024 characters)
     */
    void setDescription(
            const std::string& description      //!< [in] Description for compute block
    );

    /*!
     * \brief Set the block options.
     *
     * This is optional, if not set, the default options will be used. Options are limited to 16 characters.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockOptions - if the block options are not valid (empty or exceeds 16 characters)
     */
    void setOptions(
            const std::string& options          //!< [in] Compute block options
    );

    /*!
     * \brief Set the block boot options.
     *
     * This is optional, if not set, the default boot options will be used.
     * Boot options are limited to 256 characters.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockBootOptions - if the block boot options are not valid (empty or exceeds 256 characters)
     */
    void setBootOptions(
            const std::string& bootOptions      //!< [in] Compute block boot options
    );

    /*!
     * \brief Set the block micro-loader image.
     *
     * This is optional, if not set, the default image will be used.
     * The micro-loader image file name is limited to 256 characters.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockMicroLoaderImage - if the block micro-loader image file name is not valid (empty or exceeds 256 characters)
     */
    void setMicroLoaderImage(
            const std::string& image            //!< [in] Path to the compute block micro-loader image
    );

    /*!
     * \brief Set the block Node configuration.
     *
     * This is optional, if not set, the default Node configuration "CNKDefault" will be used.
     *
     * The Node configuration name is limited to 32 characters.
     *
     * Note: Customized compute block Node configurations are created by System Administrators using the "create_node_config" command
     * from the bg_console. The Node configuration name specified must exist in the database.
     *
     * \throws bgsched::InputException with values:
     * - bgsched::InputErrors::InvalidBlockNodeConfiguration - if the block Node configuration is not valid (empty or exceeds 32 characters)
     */
    void setNodeConfiguration(
            const std::string& nodeConfig       //!< [in] Node configuration name for the compute block
    );

    /*!
     * \brief Implementation type.
     */
    class Impl;

    /*!
     * \brief Pointer to implementation type.
     */
    typedef boost::shared_ptr<Impl> Pimpl;

    /*!
     * \brief Get pointer to implementation.
     *
     * \return Pointer to implementation.
     */
    Pimpl getPimpl() const;

    /*!
     * \brief
     */
    explicit Block(
            Pimpl impl      //!< [in] Pointer to implementation
    );

protected:

    Pimpl _impl;

};

} // namespace bgsched

#endif

