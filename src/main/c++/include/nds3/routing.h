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
     * @param timestampDelegate the function that returns the exact starting time
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
     * @return the clock source value
     */
    size_t getClkSrc();
    /**
     * @brief Retrieve the clock destination
     *
     * @return the clock destination value
     */
    size_t getClkDst();
    /**
     * @brief Retrieve the terminal source
     *
     * @return the terminal source value
     */
    size_t getTermSrc();
    /**
     * @brief Retrieve the terminal destination
     *
     * @return the terminal destination value
     */
    size_t getTermDst();
    /**
     * @brief Retrieve the terminal sync mode
     * @return the terminal sync mode value
     */
    size_t getTermSyncSet();
    /**
     * @brief Retrieve the terminal invert mode
     *
     * @return the terminal invert mode value
     */
    size_t getTermInvertSet();


    /** TODO Some setters are commented. Should they exist
     *
     */

    /**
     * @brief Makes the clock connection and sets the value of the m_ClkSetStatus_PV and
     * m_ClkSetCode_PV
     *
     */
//    void setClkSet(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_ClkSetStatus_PV.
     *
     */
    void setClkSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_ClkSetCode_PV.
     *
     */
    void setClkSetCode(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_XX_PV.
     *
     */
//    void setClkDstRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Update the value of the m_ClkSrcRead_PV.
     *
     */
    void setClkSrcRead(const timespec& timestamp, const std::int32_t& value);


    /**
     * @brief Makes the terminal connection and sets the value of the m_TermSetStatus_PV and
     * m_TermSetCode_PV
     *
     */
//    void setTermSet(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSetStatus_PV.
     *
     */
    void setTermSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_TermSetCode_PV.
     *
     */
    void setTermSetCode(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Update the value of the m_TermSrcRead_PV, m_TermSyncRead_PV and m_TermInvertRead_PV.
     *
     */
//    void setTermDstRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSrcRead_PV.
     *
     */
    void setTermSrcRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermSyncRead_PV.
     *
     */
    void setTermSyncRead(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_TermInvertRead_PV.
     *
     */
    void setTermInvertRead(const timespec& timestamp, const std::int32_t& value);


};

}
#endif // NDSROUTING_H

