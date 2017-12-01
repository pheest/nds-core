/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */

#include "nds3/definitions.h"
#include "nds3/impl/StreamingImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{


template<typename T>
StreamingImpl<T>::StreamingImpl( const std::string& name,
										 size_t maxElements,
										 stateChange_t switchOnFunction,
										 stateChange_t switchOffFunction,
										 stateChange_t startFunction,
										 stateChange_t stopFunction,
										 stateChange_t recoverFunction,
										 allowChange_t allowStateChangeFunction,
										 writerInt32_t PV_BufferSize_Writer,
										 writerInt32_t PV_StreamingDataFormat_Writer,
									     writerInt32_t PV_StreamingType_Writer):
    NodeImpl(name, nodeType_t::dataSourceChannel),
	m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
	m_DataIn_PV.reset(new PVVariableInImpl<T>("DataIn"));
	m_DataIn_PV->setMaxElements(maxElements);
	m_DataIn_PV->setDescription("Data input");
	m_DataIn_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataIn_PV);

	m_DataOut_PV.reset(new PVVariableInImpl<T>("DataOut"));
	m_DataOut_PV->setMaxElements(maxElements);
	m_DataOut_PV->setDescription("data Output");
	m_DataOut_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataOut_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_Decimation_PV);

	m_BufferSize_PV.reset(new PVDelegateOutImpl<std::int32_t>("BufferSize",PV_BufferSize_Writer));
	m_BufferSize_PV->setDescription("Size of the streaming buffer");
	m_BufferSize_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_BufferSize_PV);

	m_BufferSize_RBVPV.reset(new PVVariableInImpl<std::int32_t>("BufferSize_RBV"));
	m_BufferSize_RBVPV->setDescription("Size of the streaming buffer ReadBack");
	m_BufferSize_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_BufferSize_RBVPV);

    enumerationStrings_t DataFormatEnumeratorStrings;
    DataFormatEnumeratorStrings.push_back("Binary");
    DataFormatEnumeratorStrings.push_back("ASCII");

	m_StreamingDataFormat_PV.reset(new PVDelegateOutImpl<std::int32_t>("StreamingDataFormat",PV_StreamingDataFormat_Writer));
	m_StreamingDataFormat_PV->setDescription("Streaming Data Format: Binary or ASCII");
	m_StreamingDataFormat_PV->setScanType(scanType_t::passive, 0);
	m_StreamingDataFormat_PV->setEnumeration(DataFormatEnumeratorStrings);
	addChild(m_StreamingDataFormat_PV);

	m_StreamingDataFormat_RBVPV.reset(new PVVariableInImpl<std::int32_t>("StreamingDataFormat_RBV"));
	m_StreamingDataFormat_RBVPV->setDescription("Streaming Data Format: Binary or ASCII");
	m_StreamingDataFormat_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_StreamingDataFormat_RBVPV->setEnumeration(DataFormatEnumeratorStrings);
	addChild(m_StreamingDataFormat_RBVPV);

    enumerationStrings_t StreamingTypeEnumeratorStrings;
    StreamingTypeEnumeratorStrings.push_back("Continuous");
    StreamingTypeEnumeratorStrings.push_back("On-demand");

	m_StreamingType_PV.reset(new PVDelegateOutImpl<std::int32_t>("StreamingType",PV_StreamingType_Writer));
	m_StreamingType_PV->setDescription("Streaming Type: Continuous or On-demand");
	m_StreamingType_PV->setScanType(scanType_t::passive, 0);
	m_StreamingType_PV->setEnumeration(StreamingTypeEnumeratorStrings);
	addChild(m_StreamingType_PV);

	m_StreamingType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("StreamingType_RBV"));
	m_StreamingType_RBVPV->setDescription("Streaming Type: Continuous or On-demand ReadBack");
	m_StreamingType_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_StreamingType_RBVPV);



    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&StreamingImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);

}



template<typename T>
timespec StreamingImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void StreamingImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void StreamingImpl<T>::push(const timespec& timestamp, const T& data)
{
	m_DataOut_PV->push(timestamp, data); //TODO: Implement push for m_DataIn_PV
}

template<typename T>
size_t StreamingImpl<T>::getMaxElements()
{
    return m_DataOut_PV->getMaxElements();
}

template<typename T>
void StreamingImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_DataIn_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_DataOut_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_OnStartDelegate();
}

template<typename T>
size_t StreamingImpl<T>::getBufferSize()
{
	std::int32_t BufferSize;
	timespec timestamp;
	m_BufferSize_RBVPV->read(&timestamp, &BufferSize);
	return (std::int32_t)BufferSize;
}

template<typename T>
size_t StreamingImpl<T>::getStreamingType()
{
	std::int32_t StreamingType;
	timespec timestamp;
	m_StreamingType_RBVPV->read(&timestamp, &StreamingType);
	return (std::int32_t)StreamingType;
}

template<typename T>
size_t StreamingImpl<T>::getStreamingDataFormat()
{
	std::int32_t StreamingDataFormat;
	timespec timestamp;
	m_StreamingDataFormat_RBVPV->read(&timestamp, &StreamingDataFormat);
	return (std::int32_t)StreamingDataFormat;
}

template<typename T>
void StreamingImpl<T>::setBufferSize(const timespec& timestamp, const std::int32_t& value)
{
	m_BufferSize_RBVPV->setValue(timestamp, value);
	m_BufferSize_RBVPV->push(timestamp, value);
}

template<typename T>
void StreamingImpl<T>::setStreamingType(const timespec& timestamp, const std::int32_t& value)
{
	m_StreamingType_RBVPV->setValue(timestamp, value);
	m_StreamingType_RBVPV->push(timestamp, value);
}

template<typename T>
void StreamingImpl<T>::setStreamingDataFormat(const timespec& timestamp, const std::int32_t& value)
{
	m_StreamingDataFormat_RBVPV->setValue(timestamp, value);
	m_StreamingDataFormat_RBVPV->push(timestamp, value);
}

template class StreamingImpl<std::int32_t>;
template class StreamingImpl<double>;
template class StreamingImpl<std::vector<std::int8_t> >;
template class StreamingImpl<std::vector<std::uint8_t> >;
template class StreamingImpl<std::vector<std::int32_t> >;
template class StreamingImpl<std::vector<double> >;

}
