/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSROUTING_H
#define NDSROUTING_H

/**
 * @file routing.h
 * @brief Defines the nds::routing node, which provides basic services for routing
 * clocks and synchronization signals
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

/**
 *
 * This is a node that supplies routing PV and few control
 * PV that specifies how the connection should be performed.
 *
 * It also provides a state machine that allows to start/stop the node.
 *
 * The user of a routing class must declare few delegate functions that
 *  specify the actions to perform when connecting sources and destinations.
 *
 */

template <typename T>
class NDS3_API Routing: public Node
{
public:
    /**
     * @brief Initializes an empty routing node.
     *
     * You must assign a valid routing node before calling initialize().
     */
    Routing();

    /**
     * @brief Copies a routing reference from another object.
     *
     * @param right a routing holder from which the reference to
     *        the routing object implementation is copied
     */
    Routing(const Routing<T>& right);

    Routing& operator=(const Routing<T>& right);

    /**
     * @brief Constructs the routing node.
     *
     */
    Routing(const std::string& name,                		///< The node's name
                    stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on
                    stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off
                    stateChange_t startFunction,            ///< Delegate function that performs the actions to start the node
                    stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the node
                    stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state
                    allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true
					writerInt32_t PV_ClkSet_Writer,			///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_ClkDstRead_Writer,		///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_TermSet_Writer,		///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_TermDstRead_Writer		///< Delegate function setter/getter to interact to the Low Level Driver API
    );


    // TODO Is it necessary this delegate function?
    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the node starts.
     *
     * @param timestampDelegate function that returns the exact starting time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup timing
     * @brief Returns the timestamp at start
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the routing node started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Retrieve the clock source
     *
     * @return clock source value
     */
    size_t getClkSrc();
    /**
     * @brief Retrieve the clock destination
     *
     * @return clock destination value
     */
    size_t getClkDst();
    /**
     * @brief Retrieve the terminal source
     *
     * @return terminal source value
     */
    size_t getTermSrc();
    /**
     * @brief Retrieve the terminal destination
     *
     * @return terminal destination value
     */
    size_t getTermDst();
    /**
     * @brief Retrieve the terminal sync mode
     * @return terminal sync mode value
     */
    size_t getTermSyncSet();
    /**
     * @brief Retrieve the terminal invert mode
     *
     * @return terminal invert mode value
     */
    size_t getTermInvertSet();


    /**
     * @brief Sets the value of the m_ClkSetStatus_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the clock setting
     */
    void setClkSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_ClkSetCode_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error clock setting
     */
    void setClkSetCode(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Update the value of the m_ClkSrcRead_PV.
     *
     * @param timestamp timestamp for the value
     * @param value clock source to read
     */
    void setClkSrcRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSetStatus_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the terminal setting
     */
    void setTermSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_TermSetCode_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error terminal setting
     */
    void setTermSetCode(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSrcRead_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Terminal source to be read
     */
    void setTermSrcRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSyncRead_PV.
     *
     * @param timestamp timestamp for the value
     * @param value terminal sync
     */
    void setTermSyncRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermInvertRead_PV.
     *
     * @param timestamp timestamp for the value
     * @param value Terminal Invert
     */
    void setTermInvertRead(const timespec& timestamp, const std::int32_t& value);


};

}
#endif // NDSROUTING_H

