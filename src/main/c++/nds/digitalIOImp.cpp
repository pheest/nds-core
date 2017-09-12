/*
 * Nominal Device Support v.3 (NDS3)
 *
 * By GMV & UPM
 */

#include "nds3/definitions.h"
#include "nds3/impl/digitalIOImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

template<typename T>
DigitalIOImpl<T>::DigitalIOImpl( const std::string& name,
								 size_t maxElements,
								 stateChange_t switchOnFunction,
								 stateChange_t switchOffFunction,
								 stateChange_t startFunction,
								 stateChange_t stopFunction,
								 stateChange_t recoverFunction,
								 allowChange_t allowStateChangeFunction,
								 writerInt32_t PV_voltLevelHigh_Writer,
								 readerInt32_t PV_voltLevelHigh_Reader,
								 writerInt32_t PV_voltLevelLow_Writer,
								 readerInt32_t PV_voltLevelLow_Reader,
								 writerInt32_t PV_ChannelDir_Writer,
								 readerInt32_t PV_ChannelDir_Reader):
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

    m_voltLevelHigh_PV.reset(new PVDelegateOutImpl<std::int32_t>("voltLevelHigh",PV_voltLevelHigh_Writer));
    m_voltLevelHigh_PV->setDescription("Gain of the Channel");
    m_voltLevelHigh_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_voltLevelHigh_PV);

	m_voltLevelHigh_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("voltLevelHigh_RBV",PV_voltLevelHigh_Reader));
	m_voltLevelHigh_RBVPV->setDescription("Gain of the Channel ReadBack");
	m_voltLevelHigh_RBVPV-> setScanType(scanType_t::passive,0);
	addChild(m_voltLevelHigh_RBVPV);

    m_voltLevelLow_PV.reset(new PVDelegateOutImpl<std::int32_t>("voltLevelLow",PV_voltLevelLow_Writer));
    m_voltLevelLow_PV->setDescription("Gain of the Channel");
    m_voltLevelLow_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_voltLevelLow_PV);

	m_voltLevelLow_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("voltLevelLow_RBV",PV_voltLevelLow_Reader));
	m_voltLevelLow_RBVPV->setDescription("Gain of the Channel");
	m_voltLevelLow_RBVPV-> setScanType(scanType_t::passive,0);
	addChild(m_voltLevelLow_RBVPV);

    //add enumeration for sampling mode
    enumerationStrings_t channelDirEnumeratorStrings;
    channelDirEnumeratorStrings.push_back("In");
    channelDirEnumeratorStrings.push_back("Out");

    m_channelDir_PV.reset(new PVDelegateOutImpl<std::int32_t>("channelDir",PV_ChannelDir_Writer));
    m_channelDir_PV->setDescription("Channel Direction: In/Out");
    m_channelDir_PV->setEnumeration(channelDirEnumeratorStrings);
    m_channelDir_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_channelDir_PV);

    m_channelDir_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("channelDir_RBV",PV_ChannelDir_Reader));
    m_channelDir_RBVPV->setDescription("Channel Direction: In/Out");
    m_channelDir_RBVPV->setScanType(scanType_t::passive, 0);
    m_channelDir_RBVPV->setEnumeration(channelDirEnumeratorStrings);
    addChild(m_channelDir_RBVPV);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DigitalIOImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);
}

template<typename T>
timespec DigitalIOImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void DigitalIOImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void DigitalIOImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_dataPV->push(timestamp, data);
}

template<typename T>
void DigitalIOImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}


template class DigitalIOImpl<std::int32_t>;
template class DigitalIOImpl<double>;
template class DigitalIOImpl<std::vector<std::int8_t> >;
template class DigitalIOImpl<std::vector<std::uint8_t> >;
template class DigitalIOImpl<std::vector<std::int32_t> >;
template class DigitalIOImpl<std::vector<double> >;
template class DigitalIOImpl<std::string >;


}
