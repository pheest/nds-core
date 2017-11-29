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
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_DataIn_PV.reset(new PVVariableInImpl<T>("DataIn"));
    m_DataIn_PV->setMaxElements(maxElements);
    m_DataIn_PV->setDescription("digital input");
    m_DataIn_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataIn_PV);

	// Add the children PVs
    m_DataOut_PV.reset(new PVVariableOutImpl<T>("DataOut"));
    m_DataOut_PV->setMaxElements(maxElements);
    m_DataOut_PV->setDescription("digital output");
    m_DataOut_PV->setScanType(scanType_t::passive, 0);
    addChild(m_DataOut_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_Decimation_PV);

    m_VoltLevelHigh_PV.reset(new PVDelegateOutImpl<std::int32_t>("VoltLevelHigh",PV_voltLevelHigh_Writer));
    m_VoltLevelHigh_PV->setDescription("Volt Level High ");
    m_VoltLevelHigh_PV->setScanType(scanType_t::passive, 0);
	addChild(m_VoltLevelHigh_PV);

	m_VoltLevelHigh_RBVPV.reset(new PVVariableInImpl<std::int32_t>("VoltLevelHigh_RBV"));
	m_VoltLevelHigh_RBVPV->setDescription("Volt Level High ReadBack");
	m_VoltLevelHigh_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_VoltLevelHigh_RBVPV);

    m_VoltLevelLow_PV.reset(new PVDelegateOutImpl<std::int32_t>("VoltLevelLow",PV_voltLevelLow_Writer));
    m_VoltLevelLow_PV->setDescription("Volt Level Low");
    m_VoltLevelLow_PV->setScanType(scanType_t::passive, 0);
	addChild(m_VoltLevelLow_PV);

	m_VoltLevelLow_RBVPV.reset(new PVVariableInImpl<std::int32_t>("VoltLevelLow_RBV"));
	m_VoltLevelLow_RBVPV->setDescription("Volt Level Low Readback");
	m_VoltLevelLow_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_VoltLevelLow_RBVPV);

    //add enumeration for sampling mode
    enumerationStrings_t channelDirEnumeratorStrings;
    channelDirEnumeratorStrings.push_back("In");
    channelDirEnumeratorStrings.push_back("Out");

    m_ChannelDir_PV.reset(new PVDelegateOutImpl<std::int32_t>("ChannelDir",PV_ChannelDir_Writer));
    m_ChannelDir_PV->setDescription("Channel Direction: In/Out");
    m_ChannelDir_PV->setScanType(scanType_t::passive, 0);
    m_ChannelDir_PV->setEnumeration(channelDirEnumeratorStrings);
    addChild(m_ChannelDir_PV);

    m_ChannelDir_RBVPV.reset(new PVVariableInImpl<std::int32_t>("ChannelDir_RBV"));
    m_ChannelDir_RBVPV->setDescription("Channel Direction: In/Out Readback");
    m_ChannelDir_RBVPV->setScanType(scanType_t::interrupt, 0);
    m_ChannelDir_RBVPV->setEnumeration(channelDirEnumeratorStrings);
    addChild(m_ChannelDir_RBVPV);

	// Add the children PVs
	m_NumberOfPushedDataBlocks.reset(new PVVariableInImpl<std::int32_t>("NumberOfPushedDataBlocks"));
	m_NumberOfPushedDataBlocks->setDescription("Number Of Pushed Data Blocks");
	m_NumberOfPushedDataBlocks->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumberOfPushedDataBlocks);

    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DigitalIOImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);
}


template<typename T>
size_t DigitalIOImpl<T>::getMaxElements()
{
    return m_DataIn_PV->getMaxElements();
}

template<typename T>
timespec DigitalIOImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void DigitalIOImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void DigitalIOImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_DataIn_PV->push(timestamp, data);
}

template<typename T>
void DigitalIOImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_DataIn_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_OnStartDelegate();
}

template<typename T>
size_t DigitalIOImpl<T>::getVoltLevelHigh()
{
	std::int32_t voltLevelHigh;
	timespec timestamp;
	m_VoltLevelHigh_RBVPV->read(&timestamp, &voltLevelHigh);
	return (std::int32_t)voltLevelHigh;
}

template<typename T>
size_t DigitalIOImpl<T>::getVoltLevelLow()
{
	std::int32_t voltLevelLow;
	timespec timestamp;
	m_VoltLevelLow_RBVPV->read(&timestamp, &voltLevelLow);
	return (std::int32_t)voltLevelLow;
}

template<typename T>
size_t DigitalIOImpl<T>::getChannelDir()
{
	std::int32_t channelDir;
	timespec timestamp;
	m_ChannelDir_RBVPV->read(&timestamp, &channelDir);
	return (std::int32_t)channelDir;
}

template<typename T>
void DigitalIOImpl<T>::setVoltLevelHigh(const timespec& timestamp, const std::int32_t& value)
{
	m_VoltLevelHigh_RBVPV->setValue(timestamp, value);
	m_VoltLevelHigh_RBVPV->push(timestamp, value);
}

template<typename T>
void DigitalIOImpl<T>::setVoltLevelLow(const timespec& timestamp, const std::int32_t& value)
{
	m_VoltLevelLow_RBVPV->setValue(timestamp, value);
	m_VoltLevelLow_RBVPV->push(timestamp, value);

}

template<typename T>
void DigitalIOImpl<T>::setChannelDir(const timespec& timestamp, const std::int32_t& value)
{
	m_ChannelDir_RBVPV->setValue(timestamp, value);
	m_ChannelDir_RBVPV->push(timestamp, value);
}

template<typename T>
void DigitalIOImpl<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
	m_NumberOfPushedDataBlocks->setValue(timestamp, value);
	m_NumberOfPushedDataBlocks->push(timestamp, value);
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
