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
    Node(std::shared_ptr<TimeSuppImpl<T> >(new TimeSuppImpl<T>(	name,
															PV_clkSrc_Writer,
															PV_clkSrc_Reader,
															PV_clkFreq_Writer,
															PV_clkFreq_Reader,
															PV_clkMult_Writer,
															PV_clkMult_Reader,
															PV_SyncStatus_Reader,
															PV_SecsSinceSync_Reader,
															PV_MaxSchFTEs_Reader,
															PV_PendingFTEs_Reader,
															PV_FTElevels_Reader,
															MaxElements,
															PV_AbortAllFTEs_Writer,
															PV_AbortAllFTEs_Reader,
															PV_refTimeBase_Reader,
															PV_Time_Reader)))
{
}


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
		writerInt32_t PV_EnableTimeStamp_Writer,
		readerInt32_t PV_EnableTimeStamp_Reader,
		writerDouble_t PV_TimeStampEdge_Writer,
		readerDouble_t PV_TimeStampEdge_Reader):
    Node(std::shared_ptr<TimeStampSuppImpl<T> >(new TimeStampSuppImpl<T>(name,
																		PV_EnableTimeStamp_Writer,
																		PV_EnableTimeStamp_Reader,
																		 PV_TimeStampEdge_Writer,
																		 PV_TimeStampEdge_Reader)))
{
}


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
timespec TimeSupp<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<TimeSuppImpl<T> >(m_pImplementation)->getStartTimestamp();
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



template class TimeSupp<std::int32_t>;
template class TimeSupp<double>;
template class TimeSupp<std::vector<std::int8_t> >;
template class TimeSupp<std::vector<std::uint8_t> >;
template class TimeSupp<std::vector<std::int32_t> >;
template class TimeSupp<std::vector<double> >;
template class TimeSupp<std::string >;

template class TimeStampSupp<std::int32_t>;
template class TimeStampSupp<double>;
template class TimeStampSupp<std::vector<std::int8_t> >;
template class TimeStampSupp<std::vector<std::uint8_t> >;
template class TimeStampSupp<std::vector<std::int32_t> >;
template class TimeStampSupp<std::vector<double> >;
template class TimeStampSupp<std::string >;

}
