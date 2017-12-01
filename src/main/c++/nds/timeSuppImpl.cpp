/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/definitions.h"
#include "nds3/impl/timeSuppImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
TimeSuppImpl<T>::TimeSuppImpl(	const std::string& name,
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
								readerTime_t PV_Time_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
	m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
	m_Time_PV.reset(new PVDelegateInImpl<T>("Time",PV_Time_Reader));
	m_Time_PV->setDescription("Time provided by the timing board");
	m_Time_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Time_PV);

    m_DataFTEs_PV.reset(new PVVariableOutImpl<std::vector<T>>("DataFTEs"));
	m_DataFTEs_PV->setDescription("DataFTEs");
	m_DataFTEs_PV->setScanType(scanType_t::passive, 0);
	m_DataFTEs_PV->setMaxElements(maxElements);
    addChild(m_DataFTEs_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_Decimation_PV);

	m_ClkSrc_PV.reset(new PVDelegateOutImpl<std::int32_t>("ClkSrc",PV_clkSrc_Writer));
	m_ClkSrc_PV->setDescription("Clock Source");
	m_ClkSrc_PV->setScanType(scanType_t::passive, 0);
	addChild(m_ClkSrc_PV);

	m_ClkSrc_RBVPV.reset(new PVVariableInImpl<std::int32_t>("clkSrc_RBV"));
	m_ClkSrc_RBVPV->setDescription("Clock Source ReadBack");
	m_ClkSrc_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ClkSrc_RBVPV);

	m_ClkFreq_PV.reset(new PVDelegateOutImpl<double>("ClkFreq",PV_clkFreq_Writer));
	m_ClkFreq_PV->setDescription("Clock Frequency");
	m_ClkFreq_PV->setScanType(scanType_t::passive, 0);
	addChild(m_ClkFreq_PV);

	m_ClkFreq_RBVPV.reset(new PVVariableInImpl<double>("ClkFreq_RBV"));
	m_ClkFreq_RBVPV->setDescription("Clock Freq ReadBack");
	m_ClkFreq_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ClkFreq_RBVPV);

	m_ClkMult_PV.reset(new PVDelegateOutImpl<std::int32_t>("ClkMult",PV_clkMult_Writer));
	m_ClkMult_PV->setDescription("Clock Multiplier");
	m_ClkMult_PV->setScanType(scanType_t::passive, 0);
	addChild(m_ClkMult_PV);

	m_ClkMult_RBVPV.reset(new PVVariableInImpl<std::int32_t>("clkMult_RBV"));
	m_ClkMult_RBVPV->setDescription("Clock Multiplier ReadBack");
	m_ClkMult_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ClkMult_RBVPV);

	m_SyncStatus_PV.reset(new PVVariableInImpl<std::int32_t>("SyncStatus"));
	m_SyncStatus_PV->setDescription("Sync Status");
	m_SyncStatus_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SyncStatus_PV);

	m_SecsSinceSync_PV.reset(new PVVariableInImpl<std::int32_t>("SecsSinceSync"));
	m_SecsSinceSync_PV->setDescription("Seconds Since last Sync");
	m_SecsSinceSync_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SecsSinceSync_PV);

	m_MaxSchFTEs_PV.reset(new PVDelegateOutImpl<std::int32_t>("MaxSchFTEs",PV_MaxSchFTEs_Writer));
	m_MaxSchFTEs_PV->setDescription("Max Scheduled FTEs");
	m_MaxSchFTEs_PV->setScanType(scanType_t::passive, 0);
	addChild(m_MaxSchFTEs_PV);

	m_MaxSchFTEs_RBVPV.reset(new PVVariableInImpl<std::int32_t>("MaxSchFTEs_RBV"));
	m_MaxSchFTEs_RBVPV->setDescription("Max Scheduled FTEs Readback");
	m_MaxSchFTEs_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_MaxSchFTEs_RBVPV);

	m_PendingFTEs_PV.reset(new PVVariableInImpl<std::int32_t>("PendingFTEs"));
	m_PendingFTEs_PV->setDescription("Pending Number of FTEs");
	m_PendingFTEs_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_PendingFTEs_PV);

	m_FTEsLevels_PV.reset(new PVVariableOutImpl<std::vector<std::int32_t>>("FTEslevels"));
	m_FTEsLevels_PV->setDescription("FTEs Levels");
	m_FTEsLevels_PV->setScanType(scanType_t::passive, 0);
	m_FTEsLevels_PV->setMaxElements(maxElements);
	addChild(m_FTEsLevels_PV);

	m_AbortAllFTEs_PV.reset(new PVDelegateOutImpl<std::int32_t>("AbortAllFTEs",PV_AbortAllFTEs_Writer));
	m_AbortAllFTEs_PV->setDescription("Abort All FTEs");
	m_AbortAllFTEs_PV->setScanType(scanType_t::passive, 0);
	addChild(m_AbortAllFTEs_PV);

	m_AbortAllFTEs_RBVPV.reset(new PVVariableInImpl<std::int32_t>("AbortAllFTEs_RBV"));
	m_AbortAllFTEs_RBVPV->setDescription("Abort All FTEs Readback");
	m_AbortAllFTEs_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_AbortAllFTEs_RBVPV);

	m_RefTimeBase_PV.reset(new PVVariableInImpl<timespec>("refTimeBase"));
	m_RefTimeBase_PV->setDescription("Reference Time Base");
	m_RefTimeBase_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_RefTimeBase_PV);

    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&TimeSuppImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);
}

template<typename T>
timespec TimeSuppImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void TimeSuppImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void TimeSuppImpl<T>::push(const timespec& timestamp, const T& data)
{
	m_Time_PV->push(timestamp, data);
}

template<typename T>
size_t TimeSuppImpl<T>::getMaxElements()
{
    return m_DataFTEs_PV->getMaxElements();
}

template<typename T>
void TimeSuppImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_Time_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_OnStartDelegate();
}

template<typename T>
std::vector<timespec> TimeSuppImpl<T>::getDataFTEs()
{
	std::vector<timespec> DataFTEs;
	timespec timestamp;
	m_DataFTEs_PV->read(&timestamp, &DataFTEs);
	return (std::vector<timespec>)DataFTEs;
}
template<typename T>
size_t TimeSuppImpl<T>::getClkSrc()
{
	std::int32_t ClkSrc;
	timespec timestamp;
	m_ClkSrc_RBVPV->read(&timestamp, &ClkSrc);
	return (std::int32_t)ClkSrc;
}
template<typename T>
size_t TimeSuppImpl<T>::getClkFreq()
{
	double ClkFreq;
	timespec timestamp;
	m_ClkFreq_RBVPV->read(&timestamp, &ClkFreq);
	return (double)ClkFreq;
}
template<typename T>
size_t TimeSuppImpl<T>::getClkMult()
{
	std::int32_t ClkMult;
	timespec timestamp;
	m_ClkMult_RBVPV->read(&timestamp, &ClkMult);
	return (std::int32_t)ClkMult;
}
template<typename T>
size_t TimeSuppImpl<T>::getSyncStatus()
{
	std::int32_t SyncStatus;
	timespec timestamp;
	m_SyncStatus_PV->read(&timestamp, &SyncStatus);
	return (std::int32_t)SyncStatus;
}
template<typename T>
size_t TimeSuppImpl<T>::getSecsSinceSync()
{
	std::int32_t SecsSinceSync;
	timespec timestamp;
	m_SecsSinceSync_PV->read(&timestamp, &SecsSinceSync);
	return (std::int32_t)SecsSinceSync;
}
template<typename T>
size_t TimeSuppImpl<T>::getMaxSchFTEs()
{
	std::int32_t MaxSchFTEs;
	timespec timestamp;
	m_MaxSchFTEs_RBVPV->read(&timestamp, &MaxSchFTEs);
	return (std::int32_t)MaxSchFTEs;
}
template<typename T>
size_t TimeSuppImpl<T>::getPendingFTEs()
{
	std::int32_t PendingFTEs;
	timespec timestamp;
	m_PendingFTEs_PV->read(&timestamp, &PendingFTEs);
	return (std::int32_t)PendingFTEs;
}
template<typename T>
std::vector<std::int32_t> TimeSuppImpl<T>::getFTEsLevels()
{
	std::vector<std::int32_t> FTEsLevels;
	timespec timestamp;
	m_FTEsLevels_PV->read(&timestamp, &FTEsLevels);
	return (std::vector<std::int32_t>)FTEsLevels;
}
template<typename T>
size_t TimeSuppImpl<T>::getAbortAllFTEs()
{
	std::int32_t AbortAllFTEs;
	timespec timestamp;
	m_AbortAllFTEs_RBVPV->read(&timestamp, &AbortAllFTEs);
	return (std::int32_t)AbortAllFTEs;
}
template<typename T>
timespec TimeSuppImpl<T>::getRefTimeBase()
{
	timespec RefTimeBase;
	timespec timestamp;
	m_RefTimeBase_PV->read(&timestamp, &RefTimeBase);
	return (timespec)RefTimeBase;
}
template<typename T>
void TimeSuppImpl<T>::setDataFTEs(const timespec& timestamp, const std::vector<timespec>& value)
{
	//m_DataFTEs_PV->setValue(timestamp, value);
	//m_DataFTEs_PV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setClkSrc(const timespec& timestamp, const std::int32_t& value)
{
	m_ClkSrc_RBVPV->setValue(timestamp, value);
	m_ClkSrc_RBVPV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setClkFreq(const timespec& timestamp, const double& value)
{
	m_ClkFreq_RBVPV->setValue(timestamp, value);
	m_ClkFreq_RBVPV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setClkMult(const timespec& timestamp, const std::int32_t& value)
{
	m_ClkMult_RBVPV->setValue(timestamp, value);
	m_ClkMult_RBVPV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setSyncStatus(const timespec& timestamp, const std::int32_t& value)
{
	m_SyncStatus_PV->setValue(timestamp, value);
	m_SyncStatus_PV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setSecsSinceSync(const timespec& timestamp, const std::int32_t& value)
{
	m_SecsSinceSync_PV->setValue(timestamp, value);
	m_SecsSinceSync_PV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setMaxSchFTEs(const timespec& timestamp, const std::int32_t& value)
{
	m_MaxSchFTEs_RBVPV->setValue(timestamp, value);
	m_MaxSchFTEs_RBVPV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setPendingFTEs(const timespec& timestamp, const std::int32_t& value)
{
	m_PendingFTEs_PV->setValue(timestamp, value);
	m_PendingFTEs_PV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setFTEsLevels(const timespec& timestamp, const std::vector<std::int32_t>& value)
{
	//m_FTEsLevels_PV->setValue(timestamp, value);
	//m_FTEsLevels_PV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setAbortAllFTEs(const timespec& timestamp, const std::int32_t& value)
{
	m_AbortAllFTEs_RBVPV->setValue(timestamp, value);
	m_AbortAllFTEs_RBVPV->push(timestamp, value);
}
template<typename T>
void TimeSuppImpl<T>::setRefTimeBase(const timespec& timestamp, const timespec& value)
{
	m_RefTimeBase_PV->setValue(timestamp, value);
	m_RefTimeBase_PV->push(timestamp, value);
}

template class TimeSuppImpl<timespec> ;


template<typename T>
TimeStampSuppImpl<T>::TimeStampSuppImpl( 	const std::string& name,
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
											readerDouble_t PV_TimeStampEdge_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
	m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
	m_DataTimeStamps_PV.reset(new PVVariableInImpl<T>("DataTimeStamps"));
	m_DataTimeStamps_PV->setMaxElements(maxElements);
	m_DataTimeStamps_PV->setDescription("Data timestamps acquired");
	m_DataTimeStamps_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataTimeStamps_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_Decimation_PV);

    m_TimeStampSrc_PV.reset(new PVDelegateOutImpl<std::int32_t>("TimeStampSrc",PV_TimeStampEdge_Writer));
    m_TimeStampSrc_PV->setDescription("TimeStamp Source");
    m_TimeStampSrc_PV->setScanType(scanType_t::passive, 0);
	addChild(m_TimeStampSrc_PV);

	m_TimeStampSrc_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TimeStampSrc_RBV"));
	m_TimeStampSrc_RBVPV->setDescription("TimeStamp Source ReadBack");
	m_TimeStampSrc_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TimeStampSrc_RBVPV);

	m_EnableTimeStamp_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableTimeStamp",PV_EnableTimeStamp_Writer));
    m_EnableTimeStamp_PV->setDescription("Enable TimeStamp");
    m_EnableTimeStamp_PV->setScanType(scanType_t::passive, 0);
	addChild(m_EnableTimeStamp_PV);

	m_EnableTimeStamp_RBVPV.reset(new PVVariableInImpl<std::int32_t>("EnableTimeStamp_RBV"));
	m_EnableTimeStamp_RBVPV->setDescription("Enable TimeStamp ReadBack");
	m_EnableTimeStamp_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_EnableTimeStamp_RBVPV);

	m_TimeStampEdge_PV.reset(new PVDelegateOutImpl<std::int32_t>("TimeStampEdge",PV_TimeStampEdge_Writer));
	m_TimeStampEdge_PV->setDescription("TimeStamp Edge");
	m_TimeStampEdge_PV->setScanType(scanType_t::passive, 0);
	addChild(m_TimeStampEdge_PV);

	m_TimeStampEdge_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TimeStampEdge_RBV"));
	m_TimeStampEdge_RBVPV->setDescription("TimeStamp Edge ReadBack");
	m_TimeStampEdge_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TimeStampEdge_RBVPV);

	// Add state machine
	m_StateMachine.reset(new StateMachineImpl(true,
			switchOnFunction,
			switchOffFunction,
			std::bind(&TimeStampSuppImpl::onStart, this),
			stopFunction,
			recoverFunction,
			allowStateChangeFunction));
	addChild(m_StateMachine);

}

template<typename T>
timespec TimeStampSuppImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void TimeStampSuppImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void TimeStampSuppImpl<T>::push(const timespec& timestamp, const T& data)
{
	m_DataTimeStamps_PV->push(timestamp, data);
}

template<typename T>
size_t TimeStampSuppImpl<T>::getMaxElements()
{
    return m_DataTimeStamps_PV->getMaxElements();
}

template<typename T>
void TimeStampSuppImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_DataTimeStamps_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_OnStartDelegate();
}

template<typename T>
size_t TimeStampSuppImpl<T>::getTimeStampSrc()
{
	std::int32_t TimeStampSrc;
	timespec timestamp;
	m_TimeStampSrc_RBVPV->read(&timestamp, &TimeStampSrc);
	return (std::int32_t)TimeStampSrc;
}

template<typename T>
size_t TimeStampSuppImpl<T>::getEnableTimeStamp()
{
	std::int32_t EnableTimeStamp;
	timespec timestamp;
	m_EnableTimeStamp_RBVPV->read(&timestamp, &EnableTimeStamp);
	return (std::int32_t)EnableTimeStamp;
}

template<typename T>
size_t TimeStampSuppImpl<T>::getTimeStampEdge()
{
	std::int32_t TimeStampEdge;
	timespec timestamp;
	m_TimeStampEdge_RBVPV->read(&timestamp, &TimeStampEdge);
	return (std::int32_t)TimeStampEdge;
}

template<typename T>
void TimeStampSuppImpl<T>::setTimeStampSrc(const timespec& timestamp, const std::int32_t& value)
{
	m_TimeStampSrc_RBVPV->setValue(timestamp, value);
	m_TimeStampSrc_RBVPV->push(timestamp, value);
}

template<typename T>
void TimeStampSuppImpl<T>::setEnableTimeStamp(const timespec& timestamp, const std::int32_t& value)
{
	m_EnableTimeStamp_RBVPV->setValue(timestamp, value);
	m_EnableTimeStamp_RBVPV->push(timestamp, value);
}

template<typename T>
void TimeStampSuppImpl<T>::setTimeStampEdge(const timespec& timestamp, const std::int32_t& value)
{
	m_TimeStampEdge_RBVPV->setValue(timestamp, value);
	m_TimeStampEdge_RBVPV->push(timestamp, value);
}
template class TimeStampSuppImpl<std::vector<timespec>>;

template<typename T>
TriggerSuppImpl<T>::TriggerSuppImpl(
		 const std::string& name,
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
			readerDouble_t PV_SecondsToACQwhenTrigg_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel)
{
	// Add the children PVs
    m_DAQStartABSTime_PV.reset(new PVDelegateOutImpl<double>("DAQStartABSTime",PV_DAQStartABSTime_Writer));
    m_DAQStartABSTime_PV->setDescription("Start DAQ after ABS Time");
	addChild(m_DAQStartABSTime_PV);

	m_DAQStartABSTime_RBVPV.reset(new PVDelegateInImpl<double>("DAQStartABSTime_RBV",PV_DAQStartABSTime_Reader));
	m_DAQStartABSTime_RBVPV->setDescription("Start DAQ after ABS Time ReadBack");
	m_DAQStartABSTime_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DAQStartABSTime_RBVPV);

    m_DAQStartTimeDelay_PV.reset(new PVDelegateOutImpl<double>("DAQStartTimeDelay",PV_DAQStartTimeDelay_Writer));
    m_DAQStartTimeDelay_PV->setDescription("Start DAQ after Time Delay");
	addChild(m_DAQStartTimeDelay_PV);

	m_DAQStartTimeDelay_RBVPV.reset(new PVDelegateInImpl<double>("DAQStartTimeDelay_RBV",PV_DAQStartTimeDelay_Reader));
	m_DAQStartTimeDelay_RBVPV->setDescription("Start DAQ after Time Delay ReadBack");
	m_DAQStartTimeDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DAQStartTimeDelay_RBVPV);

    m_TriggPeriodType_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggPeriodType",PV_TriggPeriodType_Writer));
    m_TriggPeriodType_PV->setDescription("Trigger Period Type");
	addChild(m_TriggPeriodType_PV);

	m_TriggPeriodType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggPeriodType_RBV",PV_TriggPeriodType_Reader));
	m_TriggPeriodType_RBVPV->setDescription("Trigger period Type ReadBack");
	m_TriggPeriodType_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggPeriodType_RBVPV);

    m_EnableSWTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableSWTrigg",PV_EnableSWTrigg_Writer));
    m_EnableSWTrigg_PV->setDescription("Enable SW Trigger");
	addChild(m_EnableSWTrigg_PV);

	m_EnableSWTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableSWTrigg_RBV",PV_EnableSWTrigg_Reader));
	m_EnableSWTrigg_RBVPV->setDescription("Enable SW Trigger ReadBack");
	m_EnableSWTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_EnableSWTrigg_RBVPV);

    m_TriggPeriod_PV.reset(new PVDelegateOutImpl<double>("TriggPeriod",PV_TriggPeriod_Writer));
    m_TriggPeriod_PV->setDescription("Trigger Period");
	addChild(m_TriggPeriod_PV);

	m_TriggPeriod_RBVPV.reset(new PVDelegateInImpl<double>("TriggPeriod_RBV",PV_TriggPeriod_Reader));
	m_TriggPeriod_RBVPV->setDescription("Trigger Period ReadBack");
	m_TriggPeriod_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggPeriod_RBVPV);

    m_TriggEventType_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggEventType",PV_TriggEventType_Writer));
    m_TriggEventType_PV->setDescription("Trigger Event Type");
	addChild(m_TriggEventType_PV);

	m_TriggEventType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggEventType_RBV",PV_TriggEventType_Reader));
	m_TriggEventType_RBVPV->setDescription("Trigg Event Type ReadBack");
	m_TriggEventType_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggEventType_RBVPV);

    m_LevelTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("LevelTrigg",PV_LevelTrigg_Writer));
    m_LevelTrigg_PV->setDescription("Trigger Level");
	addChild(m_LevelTrigg_PV);

	m_LevelTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("LevelTrigg_RBV",PV_LevelTrigg_Reader));
	m_LevelTrigg_RBVPV->setDescription("Trigger Level ReadBack");
	m_LevelTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_LevelTrigg_RBVPV);

    m_EdgeTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("EdgeTrigg",PV_EdgeTrigg_Writer));
    m_EdgeTrigg_PV->setDescription("Trigger Edge");
	addChild(m_EdgeTrigg_PV);

	m_EdgeTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EdgeTrigg_RBV",PV_EdgeTrigg_Reader));
	m_EdgeTrigg_RBVPV->setDescription("Trigger Edge ReadBack");
	m_EdgeTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_EdgeTrigg_RBVPV);

    m_CombineTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("CombineTrigg",PV_CombineTrigg_Writer));
    m_CombineTrigg_PV->setDescription("Combine Trigger ");
	addChild(m_CombineTrigg_PV);

	m_CombineTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("CombineTrigg_RBV",PV_CombineTrigg_Reader));
	m_CombineTrigg_RBVPV->setDescription("Combine Trigger ReadBack");
	m_CombineTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_CombineTrigg_RBVPV);

    m_TriggDelay_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggDelay",PV_TriggDelay_Writer));
    m_TriggDelay_PV->setDescription("Trigger Delay ");
	addChild(m_TriggDelay_PV);

	m_TriggDelay_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggDelay_RBV",PV_TriggDelay_Reader));
	m_TriggDelay_RBVPV->setDescription("Trigger Delay ReadBack");
	m_TriggDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggDelay_RBVPV);

    m_PreTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("PreTrigg",PV_PreTrigg_Writer));
    m_PreTrigg_PV->setDescription("PreTrigger ");
	addChild(m_PreTrigg_PV);

	m_PreTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("PreTrigg_RBV",PV_PreTrigg_Reader));
	m_PreTrigg_RBVPV->setDescription("PreTrigger ReadBack");
	m_PreTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_PreTrigg_RBVPV);

    m_SamplesToACQwhenTrigg_PV.reset(new PVDelegateOutImpl<double>("SamplesToACQwhenTrigg",PV_SamplesToACQwhenTrigg_Writer));
    m_SamplesToACQwhenTrigg_PV->setDescription("Samples To ACQ when Trigger");
	addChild(m_SamplesToACQwhenTrigg_PV);

	m_SamplesToACQwhenTrigg_RBVPV.reset(new PVDelegateInImpl<double>("SamplesToACQwhenTrigg_RBV",PV_SamplesToACQwhenTrigg_Reader));
	m_SamplesToACQwhenTrigg_RBVPV->setDescription("Samples To ACQ when Trigger ReadBack");
	m_SamplesToACQwhenTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SamplesToACQwhenTrigg_RBVPV);

    m_SecondsToACQwhenTrigg_PV.reset(new PVDelegateOutImpl<double>("SecondsToACQwhenTrigg",PV_SecondsToACQwhenTrigg_Writer));
    m_SecondsToACQwhenTrigg_PV->setDescription("Samples To ACQ when Trigger");
	addChild(m_SecondsToACQwhenTrigg_PV);

	m_SecondsToACQwhenTrigg_RBVPV.reset(new PVDelegateInImpl<double>("SecondsToACQwhenTrigg_RBV",PV_SecondsToACQwhenTrigg_Reader));
	m_SecondsToACQwhenTrigg_RBVPV->setDescription("Samples To ACQ when Trigger ReadBack");
	m_SecondsToACQwhenTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SecondsToACQwhenTrigg_RBVPV);


}

template class TriggerSuppImpl<std::vector<timespec>>;


}
