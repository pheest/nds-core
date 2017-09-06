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
TimeSuppImpl<T>::TimeSuppImpl(
        const std::string& name,
		writerInt32_t PV_clkSrc_Writer,
		readerInt32_t PV_clkSrc_Reader,
		writerDouble_t PV_clkFreq_Writer,
		readerDouble_t PV_clkFreq_Reader,
		writerDouble_t PV_clkMult_Writer,
		readerDouble_t PV_clkMult_Reader,
		readerDouble_t PV_SyncStatus_Reader,
		readerDouble_t PV_SecsSinceSync_Reader,
		readerInt32_t PV_MaxSchFTEs_Reader,
		readerInt32_t PV_PendingFTEs_Reader,
		readerVectorInt32_t  PV_FTElevels_Reader,
		size_t MaxElements,
		writerInt32_t PV_AbortAllFTEs_Writer,
		readerInt32_t PV_AbortAllFTEs_Reader,
		readerInt32_t PV_refTimeBase_Reader,
		readerDouble_t PV_Time_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
	m_clkSrc_PV.reset(new PVDelegateOutImpl<std::int32_t>("clkSrc",PV_clkSrc_Writer));
	m_clkSrc_PV->setDescription("Clock Source");
	addChild(m_clkSrc_PV);

	m_clkSrc_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("clkSrc_RBV",PV_clkSrc_Reader));
	m_clkSrc_RBVPV->setDescription("Clock Source ReadBack");
	m_clkSrc_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_clkSrc_RBVPV);

	m_clkFreq_PV.reset(new PVDelegateOutImpl<double>("clkFreq",PV_clkFreq_Writer));
	m_clkFreq_PV->setDescription("Clock Freq");
	addChild(m_clkFreq_PV);

	m_clkFreq_RBVPV.reset(new PVDelegateInImpl<double>("clkFreq_RBV",PV_clkFreq_Reader));
	m_clkFreq_RBVPV->setDescription("Clock Freq ReadBack");
	m_clkFreq_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_clkFreq_RBVPV);

	m_clkMult_PV.reset(new PVDelegateOutImpl<double>("clkMult",PV_clkMult_Writer));
	m_clkMult_PV->setDescription("Clock Multiplier");
	addChild(m_clkMult_PV);

	m_clkMult_RBVPV.reset(new PVDelegateInImpl<double>("clkMult_RBV",PV_clkMult_Reader));
	m_clkMult_RBVPV->setDescription("Clock Multiplier ReadBack");
	m_clkMult_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_clkMult_RBVPV);

	m_SyncStatus_PV.reset(new PVDelegateInImpl<double>("SyncStatus",PV_SyncStatus_Reader));
	m_SyncStatus_PV->setDescription("Sync Status");
	m_SyncStatus_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SyncStatus_PV);

	m_SecsSinceSync_PV.reset(new PVDelegateInImpl<double>("SecsSinceSync",PV_SecsSinceSync_Reader));
	m_SecsSinceSync_PV->setDescription("Seconds Since last Sync");
	m_SecsSinceSync_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SecsSinceSync_PV);

	m_MaxSchFTEs_PV.reset(new PVDelegateInImpl<std::int32_t>("MaxSchFTEs",PV_MaxSchFTEs_Reader));
	m_MaxSchFTEs_PV->setDescription("Max Scheduled FTEs");
	m_MaxSchFTEs_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_MaxSchFTEs_PV);

	m_PendingFTEs_PV.reset(new PVDelegateInImpl<std::int32_t>("PendingFTEs",PV_PendingFTEs_Reader));
	m_PendingFTEs_PV->setDescription("Pending Number of FTEs");
	m_PendingFTEs_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_PendingFTEs_PV);

	m_FTElevels_PV.reset(new PVDelegateInImpl<std::vector<std::int32_t>>("FTElevels",PV_FTElevels_Reader));
	m_FTElevels_PV->setDescription("FTE Levels");
	m_FTElevels_PV->setScanType(scanType_t::interrupt, 0);
	m_FTElevels_PV->setMaxElements(MaxElements);
	addChild(m_FTElevels_PV);

	m_AbortAllFTEs_PV.reset(new PVDelegateOutImpl<std::int32_t>("AbortAllFTEs",PV_AbortAllFTEs_Writer));
	m_AbortAllFTEs_PV->setDescription("Abort All FTEs");
	addChild(m_AbortAllFTEs_PV);

	m_AbortAllFTEs_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("AbortAllFTEs_RBV",PV_AbortAllFTEs_Reader));
	m_AbortAllFTEs_RBVPV->setDescription("Abort All FTEs");
	m_AbortAllFTEs_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_AbortAllFTEs_RBVPV);

	m_refTimeBase_PV.reset(new PVDelegateInImpl<std::int32_t>("refTimeBase",PV_refTimeBase_Reader));
	m_refTimeBase_PV->setDescription("Reference Time Base");
	m_refTimeBase_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_refTimeBase_PV);

	m_Time_PV.reset(new PVDelegateInImpl<double>("Time",PV_Time_Reader));
	m_Time_PV->setDescription("Retrieve Time");
	m_Time_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Time_PV);

}

template<typename T>
TimeStampSuppImpl<T>::TimeStampSuppImpl(
		 const std::string& name,
		writerInt32_t PV_EnableTimeStamp_Writer,
		readerInt32_t PV_EnableTimeStamp_Reader,
		writerDouble_t PV_TimeStampEdge_Writer,
		readerDouble_t PV_TimeStampEdge_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel)
{
	// Add the children PVs
    m_EnableTimeStamp_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableTimeStamp",PV_EnableTimeStamp_Writer));
    m_EnableTimeStamp_PV->setDescription("Enable TimeStamp");
	addChild(m_EnableTimeStamp_PV);

	m_EnableTimeStamp_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableTimeStamp_RBV",PV_EnableTimeStamp_Reader));
	m_EnableTimeStamp_RBVPV->setDescription("Enable TimeStamp ReadBack");
	m_EnableTimeStamp_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_EnableTimeStamp_RBVPV);

	m_TimeStampEdge_PV.reset(new PVDelegateOutImpl<double>("TimeStampEdge",PV_TimeStampEdge_Writer));
	m_TimeStampEdge_PV->setDescription("TimeStamp Edge");
	addChild(m_TimeStampEdge_PV);

	m_TimeStampEdge_RBVPV.reset(new PVDelegateInImpl<double>("TimeStampEdge_RBV",PV_TimeStampEdge_Reader));
	m_TimeStampEdge_RBVPV->setDescription("TimeStamp Edge ReadBack");
	m_TimeStampEdge_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TimeStampEdge_RBVPV);

}

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

    m_EnableSWTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggPeriodType",PV_EnableSWTrigg_Writer));
    m_EnableSWTrigg_PV->setDescription("Enable SW Trigg");
	addChild(m_EnableSWTrigg_PV);

	m_EnableSWTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggPeriodType_RBV",PV_EnableSWTrigg_Reader));
	m_EnableSWTrigg_RBVPV->setDescription("Enable SW Trigg ReadBack");
	m_EnableSWTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_EnableSWTrigg_RBVPV);

    m_TriggPeriod_PV.reset(new PVDelegateOutImpl<double>("TriggPeriod",PV_TriggPeriod_Writer));
    m_TriggPeriod_PV->setDescription("Trigger Period");
	addChild(m_TriggPeriod_PV);

	m_TriggPeriod_RBVPV.reset(new PVDelegateInImpl<double>("TriggPeriod_RBV",PV_TriggPeriod_Reader));
	m_TriggPeriod_RBVPV->setDescription("Trigg Period ReadBack");
	m_TriggPeriod_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggPeriod_RBVPV);

    m_TriggEventType_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggPeriod",PV_TriggEventType_Writer));
    m_TriggEventType_PV->setDescription("Trigger Event Type");
	addChild(m_TriggEventType_PV);

	m_TriggEventType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggPeriod_RBV",PV_TriggEventType_Reader));
	m_TriggEventType_RBVPV->setDescription("Trigg Event Type ReadBack");
	m_TriggEventType_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggEventType_RBVPV);

    m_LevelTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("LevelTrigg",PV_LevelTrigg_Writer));
    m_LevelTrigg_PV->setDescription("Trigger Level");
	addChild(m_LevelTrigg_PV);

	m_LevelTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("LevelTrigg_RBV",PV_LevelTrigg_Reader));
	m_LevelTrigg_RBVPV->setDescription("Trigg Level ReadBack");
	m_LevelTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_LevelTrigg_RBVPV);

    m_EdgeTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("EdgeTrigg",PV_EdgeTrigg_Writer));
    m_EdgeTrigg_PV->setDescription("Trigger Edge");
	addChild(m_EdgeTrigg_PV);

	m_EdgeTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EdgeTrigg_RBV",PV_EdgeTrigg_Reader));
	m_EdgeTrigg_RBVPV->setDescription("Trigg Edge ReadBack");
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
    m_TriggDelay_PV->setDescription("Combine Trigger ");
	addChild(m_TriggDelay_PV);

	m_TriggDelay_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggDelay_RBV",PV_TriggDelay_Reader));
	m_TriggDelay_RBVPV->setDescription("Combine Trigger ReadBack");
	m_TriggDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggDelay_RBVPV);

    m_PreTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggDelay",PV_PreTrigg_Writer));
    m_PreTrigg_PV->setDescription("PreTrigger ");
	addChild(m_PreTrigg_PV);

	m_PreTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("PreTrigg_RBV",PV_PreTrigg_Reader));
	m_PreTrigg_RBVPV->setDescription("PreTrigger ReadBack");
	m_PreTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_PreTrigg_RBVPV);

    m_SamplesToACQwhenTrigg_PV.reset(new PVDelegateOutImpl<double>("SamplesToACQwhenTrigg",PV_SamplesToACQwhenTrigg_Writer));
    m_SamplesToACQwhenTrigg_PV->setDescription("Samples To ACQ when Trigg");
	addChild(m_SamplesToACQwhenTrigg_PV);

	m_SamplesToACQwhenTrigg_RBVPV.reset(new PVDelegateInImpl<double>("SamplesToACQwhenTrigg_RBV",PV_SamplesToACQwhenTrigg_Reader));
	m_SamplesToACQwhenTrigg_RBVPV->setDescription("Samples To ACQ when Trigg ReadBack");
	m_SamplesToACQwhenTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SamplesToACQwhenTrigg_RBVPV);

    m_SecondsToACQwhenTrigg_PV.reset(new PVDelegateOutImpl<double>("SecondsToACQwhenTrigg",PV_SecondsToACQwhenTrigg_Writer));
    m_SecondsToACQwhenTrigg_PV->setDescription("Samples To ACQ when Trigg");
	addChild(m_SecondsToACQwhenTrigg_PV);

	m_SecondsToACQwhenTrigg_RBVPV.reset(new PVDelegateInImpl<double>("SecondsToACQwhenTrigg_RBV",PV_SecondsToACQwhenTrigg_Reader));
	m_SecondsToACQwhenTrigg_RBVPV->setDescription("Samples To ACQ when Trigg ReadBack");
	m_SecondsToACQwhenTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SecondsToACQwhenTrigg_RBVPV);


}


template<typename T>
timespec TimeSuppImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void TimeSuppImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}


template class TimeSuppImpl<std::int32_t>;
template class TimeSuppImpl<double>;
template class TimeSuppImpl<std::vector<std::int8_t> >;
template class TimeSuppImpl<std::vector<std::uint8_t> >;
template class TimeSuppImpl<std::vector<std::int32_t> >;
template class TimeSuppImpl<std::vector<double> >;
template class TimeSuppImpl<std::string >;


template class TimeStampSuppImpl<std::int32_t>;
template class TimeStampSuppImpl<double>;
template class TimeStampSuppImpl<std::vector<std::int8_t> >;
template class TimeStampSuppImpl<std::vector<std::uint8_t> >;
template class TimeStampSuppImpl<std::vector<std::int32_t> >;
template class TimeStampSuppImpl<std::vector<double> >;
template class TimeStampSuppImpl<std::string >;

template class TriggerSuppImpl<std::int32_t>;
template class TriggerSuppImpl<double>;
template class TriggerSuppImpl<std::vector<std::int8_t> >;
template class TriggerSuppImpl<std::vector<std::uint8_t> >;
template class TriggerSuppImpl<std::vector<std::int32_t> >;
template class TriggerSuppImpl<std::vector<double> >;
template class TriggerSuppImpl<std::string >;

}
