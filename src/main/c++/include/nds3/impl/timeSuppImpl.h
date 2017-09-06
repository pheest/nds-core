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
			readerDouble_t PV_Time_Reader);


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
     * @brief Returns the timestamp at start.
     *
     * This value is set when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

protected:

    /**
     * @brief Delegate function that retrieves the start time.
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_startTimestampFunction;

    /**
     * @brief Start time. Retrieved via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_clkSrc_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_clkSrc_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_clkFreq_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_clkFreq_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_clkMult_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_clkMult_RBVPV;

    std::shared_ptr<PVDelegateInImpl<double> > m_SyncStatus_PV;

    std::shared_ptr<PVDelegateInImpl<double> > m_SecsSinceSync_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_MaxSchFTEs_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_PendingFTEs_PV;

    std::shared_ptr<PVDelegateInImpl<std::vector<std::int32_t>> > m_FTElevels_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_AbortAllFTEs_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_AbortAllFTEs_RBVPV;

    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_refTimeBase_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_Time_PV;

};

template<typename T>
class TimeStampSuppImpl: public NodeImpl
{
public:
	TimeStampSuppImpl( const std::string& name,
			writerInt32_t PV_EnableTimeStamp_Writer,
			readerInt32_t PV_EnableTimeStamp_Reader,
			writerDouble_t PV_TimeStampEdge_Writer,
			readerDouble_t PV_TimeStampEdge_Reader);

protected:
    // PVs
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableTimeStamp_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableTimeStamp_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_TimeStampEdge_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_TimeStampEdge_RBVPV;

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

