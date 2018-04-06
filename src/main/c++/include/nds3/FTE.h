/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSFTE_H
#define NDSFTE_H

/**
 * @file FTE.h
 * @brief Defines the nds::FTE node, which provides basic services for Future Time Event scheduling
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

/**
 * This is a node that supplies FTE scheduling tools. Set, suppress and change FTEs.
 *
 * It also provides a state machine that allows to start/stop the node.
 *
 * The user of FTE class must declare few delegate functions that specify
 * the actions to perform when the FTE node's state changes.
 *
 * In particular, the transition from the state off to on should get
 * the hardware parameters.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::string
 *
 */
template <typename T>
class NDS3_API FTE: public Node
{
public:
    /**
     * @brief Initializes an empty FTE node.
     *
     * You must assign a valid FTE node before calling initialize().
     */
	FTE();

    /**
     * @brief Copies a FTE reference from another object.
     *
     * @param right a FTE holder from which the reference to
     *        the object implementation is copied
     */
	FTE(const FTE<T>& right);

	FTE& operator=(const FTE<T>& right);

    /**
     * @brief Constructs the FTE node.
     *
     */
	FTE(const std::string& name,
            stateChange_t switchOnFunction,          	///< Delegate function that performs the actions to switch the node on
            stateChange_t switchOffFunction,         	///< Delegate function that performs the actions to switch the node off
            stateChange_t startFunction,             	///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
            stateChange_t stopFunction,              	///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
            stateChange_t recoverFunction,           	///< Delegate function to execute to recover from an error state
            allowChange_t allowStateChangeFunction,  	///< Delegate function that can deny a state change. Usually just returns true
			writerInt32_t PV_Set_Writer,               	///< Delegate function setter/getter to interact to the Low Level Driver API
			writerInt32_t PV_Suppress_Writer,          	///< Delegate function setter/getter to interact to the Low Level Driver API
			writerInt32_t PV_ChgPeriod_Writer,         	///< Delegate function setter/getter to interact to the Low Level Driver API
			writerInt32_t PV_PendingValue_Writer);   	///< Delegate function setter/getter to interact to the Low Level Driver API




    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param timestampDelegate function that returns the exact starting time
     *
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup timing
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Set functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Terminal
     *
     * @return TerminalSet value
     */
    std::int32_t getTerminalSet();
    /**
     * @brief Retrieve the Mode (Single, Pulse, Clk, InmLVL)
     *
     * @return ModeSet value
     */
    std::int32_t getModeSet();
    /**
     * @brief Retrieve the StartTime
     *
     * @return SignalRefSet value
     */
	timespec getStartTimeSet();
    /**
     * @brief Retrieve the StopTime
     *
     * @return StopTimeSet value
     */
	timespec getStopTimeSet();
    /**
     * @brief Retrieve the Level
     *
     * @return LevelSet value
     */
	std::int32_t getLevelSet();
    /**
     * @brief Retrieve the Period in Nanoseconds
     *
     * @return PeriodNsecSet value
     */
	std::int32_t getPeriodNsecSet();
    /**
     * @brief Retrieve the SignalRef
     *
     * @return DutyCycleSet value
     */
	std::int32_t getDutyCycleSet();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Suppress functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Terminal Suppress
     *
     * @return TerminalSuppress value
     */
    std::int32_t getTerminalSuppress();
    /**
     * @brief Retrieve the Mode Suppress (FTE/Clock)
     *
     * @return ModeSuppress value
     */
    std::int32_t getModeSuppress();
    /**
     * @brief Retrieve the AllSuppress signal (Suppess All/Suppress One)
     *
     * @return AllSuppress value
     */
	std::int32_t getAllSuppress();
    /**
     * @brief Retrieve the Start Time of FTE to be suppressed
     *
     * @return StartTimeSuppress value
     */
	timespec getStartTimeSuppress();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Change Period functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Terminal to change the period
     *
     * @return TerminalChgPeriod value
     */
    std::int32_t getTerminalChgPeriod();
    /**
     * @brief Retrieve the Period to change the period
     *
     * @return PeriodChgPeriod value
     */
    std::int32_t getPeriodChgPeriod();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Set functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_SetStatus_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the operation
     */
	void setSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_SetCode_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error setting
     */
	void setSetCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Suppress functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_SuppressStatus_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the operation
     */
	void setSuppressStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_SuppressCode_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error suppressing
     */
	void setSuppressCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Change Period functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_ChgPeriodStatus_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the operation
     */
	void setChgPeriodStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_ChgPeriodCode_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error changing clock period
     */
	void setChgPeriodCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setter of Pending functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_PendingValue_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value number of pending FTEs
     */
	void setPendingValue(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_PendingStatus_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Status of the operation
     */
	void setPendingStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_PendingCode_PV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Code of the success/error retrieving pending FTEs
     */
	void setPendingCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setter of Maximum functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_Maximum_RBVPV and pushes it to the control system.
     *
     * @param timestamp timestamp for the value
     * @param value Maximum FTEs that can be scheduled. Size of the FTE FIFO.
     */
	void setMaximum(const timespec& timestamp, const std::int32_t& value);

};

}
#endif // NDSFTE_H

