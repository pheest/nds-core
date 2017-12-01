/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/timeSupp.h"
#include "nds3/impl/timeSuppImpl.h"

namespace nds
{

///////////////////////////////////////// TIME SUPPORT /////////////////////////////////////////

template <typename T>
TimeSupp<T>::TimeSupp(): Node()
{
}

/**
 * @brief Constructs the data acquisition node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
TimeSupp<T>::TimeSupp(
        const std::string& name,
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
						Node(std::shared_ptr<TimeSuppImpl<T> >(new TimeSuppImpl<T>(	name,
																maxElements,
																switchOnFunction,
																switchOffFunction,
																startFunction,
																stopFunction,
																recoverFunction,
																allowStateChangeFunction,
																PV_clkSrc_Writer,
																PV_clkFreq_Writer,
																PV_clkMult_Writer,
																PV_MaxSchFTEs_Writer,
																PV_AbortAllFTEs_Writer,
																PV_Time_Reader)))
{
}

template <typename T>
TimeSupp<T>::TimeSupp(const TimeSupp<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
TimeSupp<T>& TimeSupp<T>::operator=(const TimeSupp<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void TimeSupp<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void TimeSupp<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t TimeSupp<T>::getMaxElements()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec TimeSupp<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
std::vector<timespec> TimeSupp<T>::getDataFTEs()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getDataFTEs();
}
template <typename T>
size_t TimeSupp<T>::getClkSrc()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getClkSrc();
}
template <typename T>
size_t TimeSupp<T>::getClkFreq()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getClkFreq();
}
template <typename T>
size_t TimeSupp<T>::getClkMult()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getClkMult();
}
template <typename T>
size_t TimeSupp<T>::getSyncStatus()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getSyncStatus();
}
template <typename T>
size_t TimeSupp<T>::getSecsSinceSync()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getSecsSinceSync();
}
template <typename T>
size_t TimeSupp<T>::getMaxSchFTEs()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getMaxSchFTEs();
}
template <typename T>
size_t TimeSupp<T>::getPendingFTEs()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getPendingFTEs();
}
template <typename T>
std::vector<std::int32_t> TimeSupp<T>::getFTEsLevels()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getFTEsLevels();
}
template <typename T>
size_t TimeSupp<T>::getAbortAllFTEs()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getAbortAllFTEs();
}
template <typename T>
timespec TimeSupp<T>::getRefTimeBase()
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getRefTimeBase();
}

template <typename T>
void TimeSupp<T>::setDataFTEs(const timespec& timestamp, const std::vector<timespec>& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setDataFTEs(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setClkSrc(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setClkSrc(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setClkFreq(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setClkFreq(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setClkMult(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setClkMult(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setSyncStatus(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setSyncStatus(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setSecsSinceSync(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setSecsSinceSync(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setMaxSchFTEs(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setMaxSchFTEs(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setPendingFTEs(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setPendingFTEs(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setFTEsLevels(const timespec& timestamp, const std::vector<std::int32_t>& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setFTEsLevels(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setAbortAllFTEs(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setAbortAllFTEs(timestamp, value);
}
template <typename T>
void TimeSupp<T>::setRefTimeBase(const timespec& timestamp, const timespec& value)
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->setRefTimeBase(timestamp, value);
}

template class TimeSupp<timespec>;

///////////////////////////////////////// END TIME SUPPORT /////////////////////////////////////////

///////////////////////////////////////// TIMESTAMP SUPPORT /////////////////////////////////////////

template <typename T>
TimeStampSupp<T>::TimeStampSupp(): Node()
{
}

/**
 * @brief Constructs the node.
 *
 * @param name        the node name
 *
 */
template <typename T>
TimeStampSupp<T>::TimeStampSupp(
		 const std::string& name,
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
    Node(std::shared_ptr<TimeStampSuppImpl<T> >(new TimeStampSuppImpl<T>(name,
																	maxElements,
																	switchOnFunction,
																	switchOffFunction,
																	startFunction,
																	stopFunction,
																	recoverFunction,
																	allowStateChangeFunction,
																		PV_EnableTimeStamp_Writer,
																		PV_EnableTimeStamp_Reader,
																		 PV_TimeStampEdge_Writer,
																		 PV_TimeStampEdge_Reader)))
{
}

template <typename T>
TimeStampSupp<T>::TimeStampSupp(const TimeStampSupp<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
TimeStampSupp<T>& TimeStampSupp<T>::operator=(const TimeStampSupp<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void TimeStampSupp<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void TimeStampSupp<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t TimeStampSupp<T>::getMaxElements()
{
    return std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec TimeStampSupp<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
size_t TimeStampSupp<T>::getTimeStampSrc()
{
    return std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->getTimeStampSrc();
}

template <typename T>
size_t TimeStampSupp<T>::getEnableTimeStamp()
{
    return std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->getEnableTimeStamp();
}

template <typename T>
size_t TimeStampSupp<T>::getTimeStampEdge()
{
    return std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->getTimeStampEdge();
}

template <typename T>
void TimeStampSupp<T>::setTimeStampSrc(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->setTimeStampSrc(timestamp, value);
}

template <typename T>
void TimeStampSupp<T>::setEnableTimeStamp(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->setEnableTimeStamp(timestamp, value);
}

template <typename T>
void TimeStampSupp<T>::setTimeStampEdge(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<TimeStampSuppImpl<T> >(m_pImplementation)->setTimeStampEdge(timestamp, value);
}

template class TimeStampSupp<std::vector<timespec>>;

///////////////////////////////////////// END TIMESTAMP SUPPORT /////////////////////////////////////////

///////////////////////////////////////// TRIGGER SUPPORT /////////////////////////////////////////

template <typename T>
TriggerSup<T>::TriggerSup(): Node()
{
}

template <typename T>
TriggerSup<T>::TriggerSup(
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
			Node(std::shared_ptr<TriggerSuppImpl<T> >(new TriggerSuppImpl<T>(name,
								PV_DAQStartABSTime_Writer,
								PV_DAQStartABSTime_Reader,
								PV_DAQStartTimeDelay_Writer,
								PV_DAQStartTimeDelay_Reader,
								PV_TriggPeriodType_Writer,
								PV_TriggPeriodType_Reader,
								PV_EnableSWTrigg_Writer,
								PV_EnableSWTrigg_Reader,
								PV_TriggPeriod_Writer,
								PV_TriggPeriod_Reader,
								PV_TriggEventType_Writer,
								PV_TriggEventType_Reader,
								PV_LevelTrigg_Writer,
								PV_LevelTrigg_Reader,
								PV_EdgeTrigg_Writer,
								PV_EdgeTrigg_Reader,
								PV_CombineTrigg_Writer,
								PV_CombineTrigg_Reader,
								PV_TriggDelay_Writer,
								PV_TriggDelay_Reader,
								PV_PreTrigg_Writer,
								PV_PreTrigg_Reader,
								PV_SamplesToACQwhenTrigg_Writer,
								PV_SamplesToACQwhenTrigg_Reader,
								PV_SecondsToACQwhenTrigg_Writer,
								PV_SecondsToACQwhenTrigg_Reader)))
{
}

template <typename T>
TriggerSup<T>::TriggerSup(const TriggerSup<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
TriggerSup<T>& TriggerSup<T>::operator=(const TriggerSup<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template class TriggerSup<std::vector<timespec>>;

///////////////////////////////////////// END TRIGGER SUPPORT /////////////////////////////////////////


}
