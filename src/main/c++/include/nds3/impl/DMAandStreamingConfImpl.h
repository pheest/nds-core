/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Modified by GMV & UPM
 */

#ifndef NDSDMAANDSTREAMINGCONFIMPL_H
#define NDSDMAANDSTREAMINGCONFIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;
//TODO ALBB modify this

template<typename T>
class DMASupportImpl: public NodeImpl
{
public:
	DMASupportImpl(const std::string& name,
					size_t maxElements,
					readerDouble_t PV_BufferSize_Reader,
					writerInt32_t PV_EnableDMA_Writer,
					readerInt32_t PV_EnableDMA_Reader,
					readerInt32_t PV_NumDMAChannels_Reader,
					readerInt32_t PV_DMAFrameType_Reader,
					readerInt32_t PV_DMASampleSize_Reader,
					readerInt32_t PV_DMASamplingRate_Reader);

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
     * @brief Generation start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs
    std::shared_ptr<PVVariableInImpl<T> > m_dataPV;
    std::shared_ptr<PVDelegateInImpl<double> > m_BufferSize_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableDMA_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableDMA_RBVPV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_NumDMAChannels_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_DMAFrameType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_DMASampleSize_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_DMASamplingRate_PV;



};

template<typename T>
class StreamingConfImpl: public NodeImpl
{
public:
	StreamingConfImpl(const std::string& name,
					size_t maxElements,
					readerInt32_t PV_StreamingDataFormat_Reader,
					writerInt32_t PV_StreamingType_Writer,
					readerInt32_t PV_StreamingType_Reader);

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

    void push(const timespec& timestamp, const T& data);

        /**
     * @brief Returns the timestamp at start .
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;


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
     * @brief Generation start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs
    std::shared_ptr<PVVariableInImpl<T> > m_dataPV;
    std::shared_ptr<PVDelegateInImpl<double> > m_BufferSize_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_StreamingType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_StreamingType_RBVPV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_StreamingDataFormat_PV;

};

}
#endif // NDSDMAANDSTREAMINGCONFIMPL_H

