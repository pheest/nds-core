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
class TriggeringImpl: public NodeImpl
{
public:
	TriggeringImpl( const std::string& name,
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

