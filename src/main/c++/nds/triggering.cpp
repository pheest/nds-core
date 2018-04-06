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


///////////////////////////////////////// TRIGGER SUPPORT /////////////////////////////////////////

template <typename T>
Triggering<T>::Triggering(): Node()
{
}

template <typename T>
Triggering<T>::Triggering(
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
			Node(std::shared_ptr<TriggeringImpl<T> >(new TriggeringImpl<T>(name,
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
Triggering<T>::Triggering(const Triggering<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
Triggering<T>& Triggering<T>::operator=(const Triggering<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template class Triggering<std::vector<timespec>>;

///////////////////////////////////////// END TRIGGER SUPPORT /////////////////////////////////////////


}
