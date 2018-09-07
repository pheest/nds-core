/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESUPP_H
#define NDSTIMESUPP_H

/**
 * @file TriggerAndClk.h
 * @brief TBD
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"
#include "nds3/routing.h"



namespace nds
{

template <typename T>
class NDS3_API TriggerAndClk: public Node
{
public:
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
	TriggerAndClk();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
	TriggerAndClk(const TriggerAndClk<T>& right);

	TriggerAndClk& operator=(const TriggerAndClk<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
	TriggerAndClk(const std::string& name,
			stateChange_t switchOnFunction,
			stateChange_t switchOffFunction,
			stateChange_t startFunction,
			stateChange_t stopFunction,
			stateChange_t recoverFunction,
			allowChange_t allowStateChangeFunction,
			writerInt32_t PV_SetSW_Writer,
			writerInt32_t PV_LoadTrigConf_Writer,
			writerInt32_t PV_ResetTrigConf_Writer,
			writerInt32_t PV_PLLSyncSET_Writer,
			writerInt32_t PV_EnableDisablePLL_Writer,
			nds::Routing<std::string> routingNode);

	/**
	 * @brief Routing node
	 */
	nds::Routing<std::string> m_Routing;


    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the waveform Generation starts.
     *
     * @param
     *
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup timing
     * @brief Returns the timestamp at the moment of the start of the generation.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the generation started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

    /**
     * @brief Retrieve the HW Block set by the control system (AI,DI,AO,DO).
     *
     * @return HWBlock value
     */
    std::int32_t getHWBlock();

    /**
     * @brief Retrieve the time to delay the DAQ after trigger.
     *
     * @return DAQStartTimeDelay value
     */
    std::int32_t getDAQStartTimeDelay();

    /**
     * @brief Retrieve the Trigger Period.
     *
     * @return Trigger Period value
     */
    std::int32_t getTriggPeriod();

    /**
     * @brief Retrieve the level configuration for the trigger (HIGH or LOW).
     *
     * @return Level value
     */
    std::int32_t getLevel();

    /**
     * @brief Retrieve the detection edge for the trigger (RISING or FALLING).
     *
     * @return Edge value
     */
    std::int32_t getEdge();

    /**
     * @brief Retrieve the selected mode for the trigger (LEVEL or EDGE).
     *
     * @return Change value
     */
    std::int32_t getChange();

    /**
     * @brief Retrieve The action (operation) executed when the trigger is received.
     *
     * @return Mode value
     */
    std::int32_t getMode();

    /**
     * @brief Retrieve the number of samples to be acquired BEFORE the trigger condition is met(only if REF-RETRIGERABLE is configured).
     *
     * @return PreTrigSamples value
     */
    std::int32_t getPreTrigSamples();

    /**
     * @brief Retrieve the number of samples to be acquired AFTER the trigger condition is met.
     *
     * @return PostTrigSamples value
     */
    std::int32_t getPostTrigSamples();

    /**
     * @brief Retrieve if the device has been configured as master or slave. Should have a value.
     *
     * @return SyncMode value
     */
    std::int32_t getSyncMode();

    /**
     * @brief Retrieve the PLL Reference Frequency.
     *
     * @return PLLRefFreq value
     */
    std::int32_t getPLLRefFreq();

    /**
     * @brief Retrieve the PLL Reference Divisor.
     *
     * @return PLLRefDiv value
     */
    std::int32_t getPLLRefDiv();

    /**
     * @brief Retrieve the PLL Reference Multiplier.
     *
     * @return PLLRefMult value
     */
    std::int32_t getPLLRefMult();

    /**
     * @brief Retrieve the PLL Reference Divisor.
     *
     * @return PLLRefDiv value
     */
    std::int32_t getPLLRefDivALL();

    /**
    * @brief Sets the LoadTrigConf error status.
    *
    * @param timestamp timestamp for the value
    * @param value Status after the trigger configuration.
    */
    void setTrigLoadStatus(const timespec& timestamp, const std::string& value);

    /**
    * @brief Sets the LoadTrigConf error code.
    *
    * @param timestamp timestamp for the value
    * @param value Code of detected error.
    */
	void setTrigLoadCode(const timespec& timestamp, const std::int32_t& value);

	/**
    * @brief Sets the HW Block set by the control system (AI,DI,AO,DO) Readback Value.
    *
    * @param timestamp timestamp for the value
    * @param value HWBlockRBV
    */
    void setHWBlockRBV(const timespec& timestamp, const std::int32_t& value);
	/**
    * @brief Number ide
    *
    * @param timestamp timestamp for the value
    * @param value DAQStartTimeDelayRBV
    */
    void setDAQStartTimeDelayRBV(const timespec& timestamp, const std::int32_t& value);

	/**
    * @brief Sets the trigger period readback value.
    *
    * @param timestamp timestamp for the value
    * @param value TriggPeriodRBV
    */
    void setTriggPeriodRBV(const timespec& timestamp, const std::int32_t& value);

	/**
    * @brief Sets the level configuration for the trigger (HIGH or LOW) readback value.
    *
    * @param timestamp timestamp for the value
    * @param value LevelRBV
    */
    void setLevelRBV(const timespec& timestamp, const std::int32_t& value);

	/**
    * @brief Sets the detection edge for the trigger (RISING or FALLING) readback value.
    *
    * @param timestamp timestamp for the value
    * @param value EdgeRBV
    */
    void setEdgeRBV(const timespec& timestamp, const std::int32_t& value);

	/**
    * @brief Sets the selected mode for the trigger (LEVEL or EDGE) readback value.
    *
    * @param timestamp timestamp for the value
    * @param value ChangeRBV
    */
    void setChangeRBV(const timespec& timestamp, const std::int32_t& value);

	/**
    * @brief Sets the action (operation) executed when the trigger is receive readback value.
    *
    * @param timestamp timestamp for the value
    * @param value ModeRBV
    */
    void setModeRBV(const timespec& timestamp, const std::int32_t& value);

	/**
    * @brief Sets the the number of samples to be acquired BEFORE the trigger condition is met readback value.
    *
    * @param timestamp timestamp for the value
    * @param value PreTrigSamplesRBV
    */
    void setPreTrigSamplesRBV(const timespec& timestamp, const std::int32_t& value);

	/**
    * @brief Sets the number of samples to be acquired AFTER the trigger condition is met readback value.
    *
    * @param timestamp timestamp for the value
    * @param value PostTrigSamplesRBV
    */
    void setPostTrigSamplesRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets if the device has been configured as master or slave (Should have a value). Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value SyncModeRBV configured in the HW.
     */
	void setSyncModeRBV(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the PLL Reference Frequency Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value RefFreq configured in the HW.
     */
	void setPLLRefFreqRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets the PLL Reference Divisor Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value RefDiv configured in the HW.
     */
	void setPLLRefDivRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets the PLL Reference Multiplier Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value RefMult configured in the HW.
     */
	void setPLLRefMultRBV(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Sets the PLL Reference Divisor Readback Value.
     *
     * @param timestamp timestamp for the value
     * @param value RefDivAll configured in the HW.
     */
	void setPLLRefDivALLRBV(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the PLL Load error status.
    *
    * @param timestamp timestamp for the value
    * @param value Status after the PLL configuration.
    */
    void setPLLLoadStatus(const timespec& timestamp, const std::string& value);

    /**
    * @brief Sets the PLL Load error code.
    *
    * @param timestamp timestamp for the value
    * @param value Code of detected error.
    */
	void setPLLLoadCode(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets the enable or disable PLL readback value.
    *
    * @param timestamp timestamp for the value
    * @param value Code of detected error.
    */
	void setEnableDisablePLLRBV(const timespec& timestamp, const std::int32_t& value);


};

}
#endif // NDSTIMESUPP_H

