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
								 writerInt32_t PV_voltLevelLow_Writer,
								 writerInt32_t PV_ChannelDir_Writer):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_dataInPV.reset(new PVVariableInImpl<T>("dataIn"));
    m_dataInPV->setMaxElements(maxElements);
    m_dataInPV->setDescription("digital input");
    m_dataInPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_dataInPV);

	// Add the children PVs
    m_dataOutPV.reset(new PVVariableOutImpl<T>("dataOut"));
    m_dataOutPV->setMaxElements(maxElements);
    m_dataOutPV->setDescription("digital output");
    m_dataOutPV->setScanType(scanType_t::passive, 0);
    addChild(m_dataOutPV);

    m_voltLevelHigh_PV.reset(new PVDelegateOutImpl<std::int32_t>("voltLevelHigh",PV_voltLevelHigh_Writer));
    m_voltLevelHigh_PV->setDescription("Gain of the Channel");
	addChild(m_voltLevelHigh_PV);

	m_voltLevelHigh_RBVPV.reset(new PVVariableInImpl<std::int32_t>("voltLevelHigh_RBV"));
	m_voltLevelHigh_RBVPV->setDescription("Gain of the Channel ReadBack");
	m_voltLevelHigh_RBVPV-> setScanType(scanType_t::passive,0);
	addChild(m_voltLevelHigh_RBVPV);

    m_voltLevelLow_PV.reset(new PVDelegateOutImpl<std::int32_t>("voltLevelLow",PV_voltLevelLow_Writer));
    m_voltLevelLow_PV->setDescription("Gain of the Channel");
	addChild(m_voltLevelLow_PV);

	m_voltLevelLow_RBVPV.reset(new PVVariableInImpl<std::int32_t>("voltLevelLow_RBV"));
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
    addChild(m_channelDir_PV);

    m_channelDir_RBVPV.reset(new PVVariableInImpl<std::int32_t>("channelDir_RBV"));
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
    m_dataInPV->push(timestamp, data);
}

template<typename T>
void DigitalIOImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}


/*
 * TODO: Major modifications must be done to include this new data types.
 */
//template class DigitalIOImpl<bool>;
//template class DigitalIOImpl<std::uint8_t>;
//template class DigitalIOImpl<std::uint16_t>;
//template class DigitalIOImpl<std::uint32_t>;

//template class DigitalIOImpl<std::vector<bool>>;
template class DigitalIOImpl<std::vector<std::uint8_t>>;
//template class DigitalIOImpl<std::vector<std::uint16_t>>;
//template class DigitalIOImpl<std::vector<std::uint32_t>>;


}
