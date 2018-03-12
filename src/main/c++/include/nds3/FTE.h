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
 * the actions to perform when the acquisition node's state changes.
 *
 * In particular, the transition from the state off to on should get
 * the hardware parameters.
 *
 * @tparam T  the PV data type. //TODO:template??
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
     * @ingroup
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param
     *
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup
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
     * @return the TerminalSet value
     */
    std::int32_t getTerminalSet();
    /**
     * @brief Retrieve the Mode (Single, Pulse, Clk, InmLVL)
     *
     * @return the ModeSet value
     */
    std::int32_t getModeSet();
    /**
     * @brief Retrieve the StartTime
     *
     * @return the SignalRefSet value
     */
	timespec getStartTimeSet();
    /**
     * @brief Retrieve the StopTime
     *
     * @return the StopTimeSet value
     */
	timespec getStopTimeSet();
    /**
     * @brief Retrieve the Level
     *
     * @return the LevelSet value
     */
	std::int32_t getLevelSet();
    /**
     * @brief Retrieve the Period in Nanoseconds
     *
     * @return the PeriodNsecSet value
     */
	std::int32_t getPeriodNsecSet();
    /**
     * @brief Retrieve the SignalRef
     *
     * @return the DutyCycleSet value
     */
	std::int32_t getDutyCycleSet();
    /**
     * @brief Retrieve the SignalRef
     *
     * @return the Set value
     */
	std::int32_t getSet();
    /**
     * @brief Retrieve the SignalRef
     *
     * @return the SetStatus value
     */
	std::string getSetStatus();
    /**
     * @brief Retrieve the SignalRef
     *
     * @return the SetCode value
     */
	std::int32_t getSetCode();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Suppress functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Terminal Suppress
     *
     * @return the TerminalSuppress value
     */
    std::int32_t getTerminalSuppress();
    /**
     * @brief Retrieve the Mode Suppress (FTE/Clock)
     *
     * @return the ModeSuppress value
     */
    std::int32_t getModeSuppress();
    /**
     * @brief Retrieve the AllSuppress signal (Suppess All/Suppress One)
     *
     * @return the AllSuppress value
     */
	std::int32_t getAllSuppress();
    /**
     * @brief Retrieve the Start Time of FTE to be suppressed
     *
     * @return the StartTimeSuppress value
     */
	timespec getStartTimeSuppress();
    /**
     * @brief Retrieve the Suppress signal
     *
     * @return the Suppress value
     */
	std::int32_t getSuppress();
    /**
     * @brief Retrieve the Suppress Status signal
     *
     * @return the SuppressStatus value
     */
	std::string getSuppressStatus();
    /**
     * @brief Retrieve the Suppress Code signal
     *
     * @return the Code value
     */
	std::int32_t getSuppressCode();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Change Period functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Terminal to change the period
     *
     * @return the TerminalChgPeriod value
     */
    std::int32_t getTerminalChgPeriod();
    /**
     * @brief Retrieve the Period to change the period
     *
     * @return the PeriodChgPeriod value
     */
    std::int32_t getPeriodChgPeriod();
    /**
     * @brief Retrieve the ChgPeriod signal
     *
     * @return the ChgPeriod value
     */
	std::int32_t getChgPeriod();
    /**
     * @brief Retrieve the ChgPeriodStatus
     *
     * @return the ChgPeriodStatus value
     */
	std::string getChgPeriodStatus();
    /**
     * @brief Retrieve the ChgPeriodCode
     *
     * @return the ChgPeriodCode value
     */
	std::int32_t getChgPeriodCode();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getters of Pending functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the TerminalPending
     *
     * @return the TerminalPending value
     */
	std::int32_t getTerminalPending();
    /**
     * @brief Retrieve the number of FTEs pending in the TerminalPending set
     *
     * @return the PendingValue value
     */
	std::int32_t getPendingValue();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Getter of Maximum functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Retrieve the Maximum number FTEs that can be scheduled. (Size of the FTE FIFO)
     *
     * @return the Maximum value
     */
	std::int32_t getMaximum();

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Set functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_Set_RBVPV.
     *
     */
	void setSet(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_SetStatus_PV.
     *
     */
	void setSetStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_SetCode_PV.
     *
     */
	void setSetCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Suppress functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_Suppress_RBVPV.
     *
     */
	void setSuppress(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_SuppressStatus_PV.
     *
     */
	void setSuppressStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_SuppressCode_PV.
     *
     */
	void setSuppressCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setters of Change Period functionality
    //////////////////////////////////////////////////////////////////////////////////////////
	void setChgPeriod(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_ChgPeriodStatus_PV.
     *
     */
	void setChgPeriodStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the m_ChgPeriodCode_PV.
     *
     */
	void setChgPeriodCode(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setter of Pending functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_PendingValue_PV.
     *
     */
	void setPendingValue(const timespec& timestamp, const std::int32_t& value);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Setter of Maximum functionality
    //////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Sets the value of the m_Maximum_RBVPV.
     *
     */
	void setMaximum(const timespec& timestamp, const std::int32_t& value);

};

}
#endif // NDSFTE_H

