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
	m_data_PV.reset(new PVVariableInImpl<T>("Data"));
	m_data_PV->setMaxElements(maxElements);
	m_data_PV->setDescription("Acquired data");
	m_data_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_data_PV);

	m_StreamingDataFormat_PV.reset(new PVDelegateInImpl<std::int32_t>("StreamingDataFormat",PV_StreamingDataFormat_Reader));
	m_StreamingDataFormat_PV->setDescription("Streaming Data Format: Binary or ASCII");
	m_StreamingDataFormat_PV->setScanType(scanType_t::interrupt, 0);
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
	addChild(m_StreamingType_RBVPV);

    m_decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_decimation_PV->setDescription("Decimation");
    m_decimation_PV->setScanType(scanType_t::passive, 0);
    m_decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_decimation_PV);

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
	m_data_PV->push(timestamp, data);
}

template<typename T>
size_t StreamingConfImpl<T>::getMaxElements()
{
    return m_data_PV->getMaxElements();
}

template<typename T>
void StreamingConfImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_data_PV->setDecimation((std::uint32_t)m_decimation_PV->getValue());
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
