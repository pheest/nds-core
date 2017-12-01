/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESUPPIMPL_H
#define NDSTIMESUPPIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class TimeSuppImpl: public NodeImpl
{
public:
	TimeSuppImpl( const std::string& name,
			size_t maxElements,
	        stateChange_t switchOnFunction,
	        stateChange_t switchOffFunction,
	        stateChange_t startFunction,
	        stateChange_t stopFunction,
	        stateChange_t recoverFunction,
	        allowChange_t allowStateChangeFunction,
			writerInt32_t PV_clkSrc_Writer,
			writerDouble_t PV_clkFreq_Writer,
			writerInt32_t PV_clkMult_Writer,
			writerInt32_t PV_MaxSchFTEs_Writer,
			writerInt32_t PV_AbortAllFTEs_Writer,
			readerTime_t PV_Time_Reader);


    /**
     * @brief Specifies the function to call to get the start timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();

    /**
     * @ingroup
     * @brief Push data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data thread in order to push the data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Returns the timestamp at start.
     *
     * This value is set when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;
    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();
    /**
     * @brief Retrieve the array of FTEs times
     *
     * @return the array of FTEs times
     */
    std::vector<timespec> getDataFTEs();
    /**
     * @brief Retrieve the Clock Source
     *
     * @return the Clock source
     */
    size_t getClkSrc();
    /**
     * @brief Retrieve the Clock frequency
     *
     * @return the  Clock frequency value
     */
    size_t getClkFreq();
    /**
     * @brief Retrieve the Clock multiplier
     *
     * @return the  Clock multiplier value
     */
    size_t getClkMult();
    /**
     * @brief Retrieve the Clock frequency
     *
     * @return the Clock frequency value
     */
    size_t getSyncStatus();
    /**
     * @brief Retrieve the seconds since last Sync
     *
     * @return the seconds since last Sync
     */
    size_t getSecsSinceSync();
    /**
     * @brief Retrieve the max scheduled FTEs
     *
     * @return the Max scheduled FTEs value
     */
    size_t getMaxSchFTEs();
    /**
     * @brief Retrieve the number of pending FTEs
     *
     * @return the number of pending FTEs value
     */
    size_t getPendingFTEs();
    /**
     * @brief Retrieve the FTE levels
     *
     * @return the FTEs levels
     */
    std::vector<std::int32_t> getFTEsLevels();
    /**
     * @brief Retrieve the status of Abort all FTEs
     *
     * @return the AbortAllFTEs value
     */
    size_t getAbortAllFTEs();
    /**
     * @brief Retrieve the Reference base time
     *
     * @return the Reference base time value
     */
    timespec getRefTimeBase();
    /**
     * @brief Sets the array of the FTEs times
     *
     */
    void setDataFTEs(const timespec& timestamp, const std::vector<timespec>& value);
    /**
     * @brief Retrieve the Clock Source
     *
     */
    void setClkSrc(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Clock frequency
     *
     */
    void setClkFreq(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the Clock multiplier
     *
     */
    void setClkMult(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Clock frequency
     *
     */
    void setSyncStatus(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the seconds since last Sync
     *
     */
    void setSecsSinceSync(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the max scheduled FTEs
     *
     */
    void setMaxSchFTEs(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the number of pending FTEs
     *
     */
    void setPendingFTEs(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the FTE levels
     *
     */
    void setFTEsLevels(const timespec& timestamp, const std::vector<std::int32_t>& value);
    /**
     * @brief Sets the value of the status of Abort all FTEs
     *
     */
    void setAbortAllFTEs(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the Reference base time
     *
     */
    void setRefTimeBase(const timespec& timestamp, const timespec& value);


protected:
    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_OnStartDelegate;
    /**
     * @brief Delegate function that retrieves the start time.
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Start time. Retrieved via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;

    // PVs

    std::shared_ptr<PVVariableOutImpl<std::vector<timespec> >> m_DataFTEs_PV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Decimation_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ClkSrc_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ClkSrc_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_ClkFreq_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_ClkFreq_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ClkMult_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ClkMult_RBVPV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SyncStatus_PV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SecsSinceSync_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_MaxSchFTEs_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_MaxSchFTEs_RBVPV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PendingFTEs_PV;

    std::shared_ptr<PVVariableOutImpl<std::vector<std::int32_t>> > m_FTEsLevels_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_AbortAllFTEs_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_AbortAllFTEs_RBVPV;

    std::shared_ptr<PVVariableInImpl<timespec> > m_RefTimeBase_PV;

    std::shared_ptr<PVDelegateInImpl<timespec> > m_Time_PV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;

};

template<typename T>
class TimeStampSuppImpl: public NodeImpl
{
public:
	TimeStampSuppImpl( const std::string& name,
			size_t maxElements,
	        stateChange_t switchOnFunction,
	        stateChange_t switchOffFunction,
	        stateChange_t startFunction,
	        stateChange_t stopFunction,
	        stateChange_t recoverFunction,
	        allowChange_t allowStateChangeFunction,
			writerInt32_t PV_EnableTimeStamp_Writer,
			readerInt32_t PV_EnableTimeStamp_Reader,
			writerDouble_t PV_TimeStampEdge_Writer,
			readerDouble_t PV_TimeStampEdge_Reader);

	 /**
	     * @brief Specifies the function to call to get the start timestamp.
	     *
	     * The function is called only once at each start and its result
	     * is stored in a local variable that can be retrieved with getStartTimestamp().
	     *
	     * If this function is not called then getTimestamp() is used to get the start time.
	     *
	     * @param timestampDelegate the function to call to get the start time
	     */
	    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

	    /**
	     * @brief Retrieve the maximum number of elements that can be stored in the
	     *        pushed array. This number is set in the constructor.
	     *
	     * @return the maximum number of elements that can be stored in the pushed array
	     */
	    size_t getMaxElements();

	    /**
	     * @ingroup
	     * @brief Push data to the control system.
	     *
	     * Usually your device implementation will call this function from the
	     *  data thread in order to push the data.
	     *
	     * @param timestamp the timestamp for the data
	     * @param data      the data to push to the control system
	     */
	    void push(const timespec& timestamp, const T& data);

	    /**
	     * @brief Returns the timestamp at start.
	     *
	     * This value is set when the state switches to running.
	     * If a timing plugin is active then the timestamp is taken from the plugin.
	     *
	     * @return the time when started.
	     */
	    timespec getStartTimestamp() const;
	    /**
	     * @brief Called by the state machine. Store the current timestamp and then calls the
	     *        delegated onStart function.
	     */
	    void onStart();
	    /**
	     * @brief Retrieve the timestamp source
	     *
	     * @return the timestamp source
	     */
	    size_t getTimeStampSrc();
	    /**
	     * @brief Retrieve the status of the timestamp node Enable/Disable
	     *
	     * @return the the status of the timestamp node Enable/Disable
	     */
	    size_t getEnableTimeStamp();
	    /**
	     * @brief Retrieve the timestamp edge
	     *
	     * @return the timestamp edge
	     */
	    size_t getTimeStampEdge();
	    /**
	     * @brief Sets the value of the timestamp source
	     *
	     */
	    void setTimeStampSrc(const timespec& timestamp, const std::int32_t& value);
	    /**
	     * @brief Sets the status of the timestamp node to Enable/Disable
	     *
	     */
	    void setEnableTimeStamp(const timespec& timestamp, const std::int32_t& value);
	    /**
	     * @brief Sets the value of the timestamp edge
	     *
	     */
	    void setTimeStampEdge(const timespec& timestamp, const std::int32_t& value);

protected:
	    /**
	     * @brief In the state machine we set the start function to onStart(), so we
	     *        remember here what to call from onStart().
	     */
	    stateChange_t m_OnStartDelegate;
	    /**
	     * @brief Delegate function that retrieves the start time.
	     *
	     * By default points to BaseImpl::getTimestamp().
	     *
	     * Use setStartTimestampDelegate() to change the delegate function.
	     */
	    getTimestampPlugin_t m_StartTimestampFunction;

	    /**
	     * @brief Start time. Retrieved via the delegate
	     *        function declared in  m_startTimestampFunction.
	     */
	    timespec m_StartTime;


	    // PVs
	    std::shared_ptr<PVVariableInImpl<std::vector<timespec>> > m_DataTimeStamps_PV;

	    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Decimation_PV;

	    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TimeStampSrc_PV;
	    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TimeStampSrc_RBVPV;

	    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableTimeStamp_PV;
	    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_EnableTimeStamp_RBVPV;

	    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TimeStampEdge_PV;
	    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TimeStampEdge_RBVPV;

	    std::shared_ptr<StateMachineImpl> m_StateMachine;


};

template<typename T>
class TriggerSuppImpl: public NodeImpl
{
public:
	TriggerSuppImpl( const std::string& name,
			writerDouble_t PV_DAQStartABSTime_Writer,
			readerDouble_t PV_DAQStartABSTime_Reader,
			writerDouble_t PV_DAQStartTimeDelay_Writer,
			readerDouble_t PV_DAQStartTimeDelay_Reader,
			writerInt32_t PV_TriggPeriodType_Writer,
			readerInt32_t PV_TriggPeriodType_Reader,
			writerInt32_t PV_EnableSWTrigg_Writer,
			readerInt32_t PV_EnableSWTrigg_Reader,
			writerDouble_t PV_TriggPeriod_Writer,
			readerDouble_t PV_TriggPeriod_Reader,
			writerInt32_t PV_TriggEventType_Writer,
			readerInt32_t PV_TriggEventType_Reader,
			writerInt32_t PV_LevelTrigg_Writer,
			readerInt32_t PV_LevelTrigg_Reader,
			writerInt32_t PV_EdgeTrigg_Writer,
			readerInt32_t PV_EdgeTrigg_Reader,
			writerInt32_t PV_CombineTrigg_Writer,
			readerInt32_t PV_CombineTrigg_Reader,
			writerInt32_t PV_TriggDelay_Writer,
			readerInt32_t PV_TriggDelay_Reader,
			writerInt32_t PV_PreTrigg_Writer,
			readerInt32_t PV_PreTrigg_Reader,
			writerDouble_t PV_SamplesToACQwhenTrigg_Writer,
			readerDouble_t PV_SamplesToACQwhenTrigg_Reader,
			writerDouble_t PV_SecondsToACQwhenTrigg_Writer,
			readerDouble_t PV_SecondsToACQwhenTrigg_Reader);

protected:
    // PVs
    std::shared_ptr<PVDelegateOutImpl<double> > m_DAQStartABSTime_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DAQStartABSTime_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_DAQStartTimeDelay_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DAQStartTimeDelay_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TriggPeriodType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TriggPeriodType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableSWTrigg_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableSWTrigg_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_TriggPeriod_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_TriggPeriod_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TriggEventType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TriggEventType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_LevelTrigg_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_LevelTrigg_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EdgeTrigg_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EdgeTrigg_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_CombineTrigg_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_CombineTrigg_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TriggDelay_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TriggDelay_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_PreTrigg_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_PreTrigg_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_SamplesToACQwhenTrigg_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_SamplesToACQwhenTrigg_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_SecondsToACQwhenTrigg_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_SecondsToACQwhenTrigg_RBVPV;

};

}
#endif // NDSTIMESUPPIMPL_H

