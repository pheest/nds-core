/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */

#include "nds3/definitions.h"
#include "nds3/impl/DMAandStreamingConfImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
DMASupportImpl<T>::DMASupportImpl( const std::string& name,
								   size_t maxElements,
								   stateChange_t switchOnFunction,
								   stateChange_t switchOffFunction,
								   stateChange_t startFunction,
								   stateChange_t stopFunction,
								   stateChange_t recoverFunction,
								   allowChange_t allowStateChangeFunction,
								   readerDouble_t PV_BufferSize_Reader,
								   writerInt32_t PV_EnableDMA_Writer,
								   readerInt32_t PV_EnableDMA_Reader,
								   readerInt32_t PV_NumDMAChannels_Reader,
								   readerInt32_t PV_DMAFrameType_Reader,
								   readerInt32_t PV_DMASampleSize_Reader,
								   readerInt32_t PV_DMASamplingRate_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
	m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_dataPV.reset(new PVVariableInImpl<T>("Data"));
    m_dataPV->setMaxElements(maxElements);
    m_dataPV->setDescription("Acquired data");
    m_dataPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_dataPV);

	m_BufferSize_PV.reset(new PVDelegateInImpl<double>("BufferSize",PV_BufferSize_Reader));
	m_BufferSize_PV->setDescription("Internal Buffer Filter Size");
	m_BufferSize_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_BufferSize_PV);

	m_EnableDMA_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableDMA",PV_EnableDMA_Writer));
	m_EnableDMA_PV->setDescription("Enable DMA");
	m_EnableDMA_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_EnableDMA_PV);

	m_EnableDMA_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableDMA_RBV",PV_EnableDMA_Reader));
	m_EnableDMA_RBVPV->setDescription("Enable DMA ReadBack");
	m_EnableDMA_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_EnableDMA_RBVPV);

	m_NumDMAChannels_PV.reset(new PVDelegateInImpl<std::int32_t>("NumDMAChannels",PV_NumDMAChannels_Reader));
	m_NumDMAChannels_PV->setDescription("Number of DMA Channels");
	m_NumDMAChannels_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumDMAChannels_PV);

	m_DMAFrameType_PV.reset(new PVDelegateInImpl<std::int32_t>("DMAFrameType",PV_DMAFrameType_Reader));
	m_DMAFrameType_PV->setDescription("DMA Frame Type");
	m_DMAFrameType_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DMAFrameType_PV);

	m_DMASampleSize_PV.reset(new PVDelegateInImpl<std::int32_t>("DMASampleSize",PV_DMASampleSize_Reader));
	m_DMASampleSize_PV->setDescription("DMA Sample Size");
	m_DMASampleSize_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DMASampleSize_PV);

	m_DMASamplingRate_PV.reset(new PVDelegateInImpl<std::int32_t>("DMASamplingRate",PV_DMASamplingRate_Reader));
	m_DMASamplingRate_PV->setDescription("DMA Sampling Rate");
	m_DMASamplingRate_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DMASamplingRate_PV);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DMASupportImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);

}

template<typename T>
timespec DMASupportImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void DMASupportImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void DMASupportImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_dataPV->push(timestamp, data);
}

template<typename T>
size_t DMASupportImpl<T>::getMaxElements()
{
    return m_dataPV->getMaxElements();
}

template<typename T>
void DMASupportImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}

template class DMASupportImpl<std::int32_t>;
template class DMASupportImpl<double>;
template class DMASupportImpl<std::vector<std::int8_t> >;
template class DMASupportImpl<std::vector<std::uint8_t> >;
template class DMASupportImpl<std::vector<std::int32_t> >;
template class DMASupportImpl<std::vector<double> >;
template class DMASupportImpl<std::string >;



template<typename T>
StreamingConfImpl<T>::StreamingConfImpl( const std::string& name,
										 size_t maxElements,
										 stateChange_t switchOnFunction,
										 stateChange_t switchOffFunction,
										 stateChange_t startFunction,
										 stateChange_t stopFunction,
										 stateChange_t recoverFunction,
										 allowChange_t allowStateChangeFunction,
										 readerInt32_t PV_StreamingDataFormat_Reader,
										 writerInt32_t PV_StreamingType_Writer,
										 readerInt32_t PV_StreamingType_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
	m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_dataPV.reset(new PVVariableInImpl<T>("Data"));
    m_dataPV->setMaxElements(maxElements);
    m_dataPV->setDescription("Acquired data");
    m_dataPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_dataPV);

	m_StreamingDataFormat_PV.reset(new PVDelegateInImpl<std::int32_t>("StreamingDataFormat",PV_StreamingDataFormat_Reader));
	m_StreamingDataFormat_PV->setDescription("Streaming Data Format: Binary or ASCII");
	m_StreamingDataFormat_PV->setScanType(scanType_t::interrupt, 0);
	m_StreamingDataFormat_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_StreamingDataFormat_PV);

    enumerationStrings_t DataFormatEnumeratorStrings;
    DataFormatEnumeratorStrings.push_back("Binary");
    DataFormatEnumeratorStrings.push_back("ASCII");

	m_StreamingType_PV.reset(new PVDelegateOutImpl<std::int32_t>("StreamingType",PV_StreamingType_Writer));
	m_StreamingType_PV->setDescription("Streaming Type: Continuous or On Demand");
	m_StreamingType_PV->write(getTimestamp(), (std::int32_t)1);
	m_StreamingType_PV->setEnumeration(DataFormatEnumeratorStrings);
	addChild(m_StreamingType_PV);

	m_StreamingType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("StreamingType_RBV",PV_StreamingType_Reader));
	m_StreamingType_RBVPV->setDescription("Streaming Type ReadBack");
	m_StreamingType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_StreamingType_RBVPV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_StreamingType_RBVPV);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&StreamingConfImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);

}



template<typename T>
timespec StreamingConfImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void StreamingConfImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void StreamingConfImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_dataPV->push(timestamp, data);
}

template<typename T>
size_t StreamingConfImpl<T>::getMaxElements()
{
    return m_dataPV->getMaxElements();
}

template<typename T>
void StreamingConfImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}

template class StreamingConfImpl<std::int32_t>;
template class StreamingConfImpl<double>;
template class StreamingConfImpl<std::vector<std::int8_t> >;
template class StreamingConfImpl<std::vector<std::uint8_t> >;
template class StreamingConfImpl<std::vector<std::int32_t> >;
template class StreamingConfImpl<std::vector<double> >;
template class StreamingConfImpl<std::string >;

}
