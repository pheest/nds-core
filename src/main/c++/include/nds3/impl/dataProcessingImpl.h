/*
 * Nominal Device Support v.3 (NDS3)
 *
 *
 * by GMV & UPM
 */

#ifndef NDSDATAPROCESSINGIMPL_H
#define NDSDATAPROCESSINGIMPL_H

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
class DataProcessingImpl: public NodeImpl
{
public:
    DataProcessingImpl( const std::string& name,
						size_t maxElements,
						stateChange_t switchOnFunction,
						stateChange_t switchOffFunction,
						stateChange_t startFunction,
						stateChange_t stopFunction,
						stateChange_t recoverFunction,
						allowChange_t allowStateChangeFunction,
						writerInt32_t PV_EnableFilter_Writer,
						readerInt32_t PV_EnableFilter_Reader,
						writerInt32_t PV_FilterType_Writer,
						readerInt32_t PV_FilterType_Reader,
						writerVectorInt32_t PV_FilterParams_Writer,
						readerVectorInt32_t PV_FilterParams_Reader,
						size_t 		maxFFTElements,
						writerInt32_t PV_EnableFFT_Writer,
						readerInt32_t PV_EnableFFT_Reader,
						writerInt32_t PV_EnableSwFFT_Writer,
						readerInt32_t PV_EnableSwFFT_Reader,
						writerInt32_t PV_FFTwindowType_Writer,
						readerInt32_t PV_FFTwindowType_Reader,
						writerInt32_t PV_FFTOverlap_Writer,
						readerInt32_t PV_FFTOverlap_Reader,
						writerInt32_t PV_FFTFrameSize_Writer,
						readerInt32_t PV_FFTFrameSize_Reader,
						writerInt32_t PV_FFTSmooth_Writer,
						readerInt32_t PV_FFTSmooth_Reader,
						writerInt32_t PV_EnableDecimation_Writer,
						readerInt32_t PV_EnableDecimation_Reader,
						writerInt32_t PV_DecimationType_Writer,
						readerInt32_t PV_DecimationType_Reader,
						writerInt32_t PV_DecimationOffset_Writer,
						readerInt32_t PV_DecimationOffset_Reader,
						writerInt32_t PV_RAW2Eng_Writer,
						readerInt32_t PV_RAW2Eng_Reader);



    /**
     * @brief Specifies the function to call to get the starting timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    void push(const timespec& timestamp, const T& data);


    /**
     * @brief Returns the timestamp at start.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;
    size_t getMaxElements();

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();


protected:

    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_onStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_startTimestampFunction;

    /**
     * @brief Acquisition start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs
    std::shared_ptr<PVVariableInImpl<T> > m_dataPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_enableFilter_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_enableFilter_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_FilterType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_FilterType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::vector<std::int32_t> > > m_FilterParams_PV;
    std::shared_ptr<PVDelegateInImpl<std::vector<std::int32_t> > > m_FilterParams_RBVPV;

    std::shared_ptr<PVVariableInImpl<T> > m_FFTdataPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_enableFFT_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_enableFFT_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_enableSwFFT_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_enableSwFFT_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_FFTWindowType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_FFTWindowType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_FFTFrameOverlap_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_FFTFrameOverlap_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_FFTFrameSize_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_FFTFrameSize_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_FFTSmoothFactor_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_FFTSmoothFactor_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_enableDecimation_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_enableDecimation_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_DecimationType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_DecimationType_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_DecimationFactor_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DecimationFactor_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_DecimationOffset_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_DecimationOffset_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_enableRaw2EngConversion_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_enableRaw2EngConversion_RBVPV;

    std::shared_ptr<StateMachineImpl> m_stateMachine;


};

}
#endif // NDSDATAPROCESSINGIMPL_H

