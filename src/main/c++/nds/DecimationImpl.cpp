/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 *
 */

#include "nds3/definitions.h"
#include "nds3/impl/DecimationImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
DecimationImpl<T>::DecimationImpl( const std::string& name,
										   size_t maxElements,
										   stateChange_t switchOnFunction,
										   stateChange_t switchOffFunction,
										   stateChange_t startFunction,
										   stateChange_t stopFunction,
										   stateChange_t recoverFunction,
										   allowChange_t allowStateChangeFunction,
										   writerInt32_t PV_DecimationEnable_Writer,
										   writerInt32_t PV_DecimationType_Writer,
										   writerInt32_t PV_DecimationFactor_Writer,
										   writerInt32_t PV_DecimationOffset_Writer
										  ):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_DataIn_PV.reset(new PVVariableInImpl<T>("DataIn"));
    m_DataIn_PV->setMaxElements(maxElements);
    m_DataIn_PV->setDescription("Data received in the Decimation node");
    m_DataIn_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataIn_PV);

    m_DataOut_PV.reset(new PVVariableInImpl<T>("DataOut"));
    m_DataOut_PV->setMaxElements(maxElements);
    m_DataOut_PV->setDescription("Data produced by the Decimation node");
    m_DataOut_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataOut_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_Decimation_PV);

	//add enumeration
	enumerationStrings_t DecimationenableEnumeratorStrings;
	DecimationenableEnumeratorStrings.push_back("On");
	DecimationenableEnumeratorStrings.push_back("Off");

	m_DecimationEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("DecimationEnable",PV_DecimationEnable_Writer));
	m_DecimationEnable_PV->setDescription("Enable/Disable the Decimation operation");
	m_DecimationEnable_PV->setEnumeration(DecimationenableEnumeratorStrings);
	m_DecimationEnable_PV->setScanType(scanType_t::passive, 0);
	addChild(m_DecimationEnable_PV);

	m_DecimationEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("DecimationEnable_RBV"));
	m_DecimationEnable_RBVPV->setDescription("Enable/Disable Decimation operation ReadBack");
	m_DecimationEnable_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_DecimationEnable_RBVPV->setEnumeration(DecimationenableEnumeratorStrings);
	addChild(m_DecimationEnable_RBVPV);

	enumerationStrings_t DecimationTypeEnumeratorStrings;
	DecimationTypeEnumeratorStrings.push_back("TBD");
	DecimationTypeEnumeratorStrings.push_back("TBD");

	m_DecimationType_PV.reset(new PVDelegateOutImpl<std::int32_t>("DecimationType",PV_DecimationType_Writer));
	m_DecimationType_PV->setDescription("Decimation types");
	m_DecimationType_PV->setEnumeration(DecimationTypeEnumeratorStrings);
	m_DecimationType_PV->setScanType(scanType_t::passive, 0);
	addChild(m_DecimationType_PV);


	m_DecimationType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("DecimationType_RBV"));
	m_DecimationType_RBVPV->setDescription("Decimation types");
	m_DecimationType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_DecimationType_RBVPV->setEnumeration(DecimationTypeEnumeratorStrings);
	addChild(m_DecimationType_RBVPV);

	m_DecimationFactor_PV.reset(new PVDelegateOutImpl<std::int32_t>("DecimationFactor",PV_DecimationFactor_Writer));
	m_DecimationFactor_PV->setDescription("Decimation factor for input and output channels");
	m_DecimationFactor_PV->setScanType(scanType_t::passive, 0);
	addChild(m_DecimationFactor_PV);

	m_DecimationFactor_RBVPV.reset(new PVVariableInImpl<std::int32_t>("DecimationFactor_RBV"));
	m_DecimationFactor_RBVPV->setDescription("Decimation factor for input and output channels ReadBack");
	m_DecimationFactor_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DecimationFactor_RBVPV);

	m_DecimationOffset_PV.reset(new PVDelegateOutImpl<std::int32_t>("DecimationOffset",PV_DecimationOffset_Writer));
	m_DecimationOffset_PV->setDescription("Index of the first sample that is not decimated");
	m_DecimationOffset_PV->setScanType(scanType_t::passive, 0);
	addChild(m_DecimationOffset_PV);

	m_DecimationOffset_RBVPV.reset(new PVVariableInImpl<std::int32_t>("DecimationOffset_RBV"));
	m_DecimationOffset_RBVPV->setDescription("Index of the first sample that is not decimated ReadBack");
	m_DecimationOffset_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DecimationOffset_RBVPV);

    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DecimationImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);

}

template<typename T>
timespec DecimationImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void DecimationImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void DecimationImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_DataOut_PV->push(timestamp, data);
}

template<typename T>
size_t DecimationImpl<T>::getMaxElements()
{
    return m_DataOut_PV->getMaxElements();
}

template<typename T>
void DecimationImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_DataOut_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_DataIn_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_OnStartDelegate();
}

template<typename T>
size_t DecimationImpl<T>::getDecimationEnable()
{
	std::int32_t DecimationEnable;
	timespec timestamp;
	m_DecimationEnable_RBVPV->read(&timestamp, &DecimationEnable);
	return (std::int32_t)DecimationEnable;
}

template<typename T>
size_t DecimationImpl<T>::getDecimationType()
{
	std::int32_t DecimationType;
	timespec timestamp;
	m_DecimationType_RBVPV->read(&timestamp, &DecimationType);
	return (std::int32_t)DecimationType;
}

template<typename T>
size_t DecimationImpl<T>::getDecimationFactor()
{
	std::int32_t DecimationFactor;
	timespec timestamp;
	m_DecimationFactor_RBVPV->read(&timestamp, &DecimationFactor);
	return (std::int32_t)DecimationFactor;
}

template<typename T>
size_t DecimationImpl<T>::getDecimationOffset()
{
	std::int32_t DecimationOffset;
	timespec timestamp;
	m_DecimationOffset_RBVPV->read(&timestamp, &DecimationOffset);
	return (std::int32_t)DecimationOffset;
}

template<typename T>
void DecimationImpl<T>::setDecimationEnable(const timespec& timestamp, const std::int32_t& value)
{
	m_DecimationEnable_RBVPV->setValue(timestamp, value);
	m_DecimationEnable_RBVPV->push(timestamp, value);
}

template<typename T>
void DecimationImpl<T>::setDecimationType(const timespec& timestamp, const std::int32_t& value)
{
	m_DecimationType_RBVPV->setValue(timestamp, value);
	m_DecimationType_RBVPV->push(timestamp, value);
}

template<typename T>
void DecimationImpl<T>::setDecimationFactor(const timespec& timestamp, const std::int32_t& value)
{
	m_DecimationFactor_RBVPV->setValue(timestamp, value);
	m_DecimationFactor_RBVPV->push(timestamp, value);
}

template<typename T>
void DecimationImpl<T>::setDecimationOffset(const timespec& timestamp, const std::int32_t& value)
{
	m_DecimationOffset_RBVPV->setValue(timestamp, value);
	m_DecimationOffset_RBVPV->push(timestamp, value);
}

template class DecimationImpl<std::int32_t>;
template class DecimationImpl<double>;
template class DecimationImpl<std::vector<std::int8_t> >;
template class DecimationImpl<std::vector<std::uint8_t> >;
template class DecimationImpl<std::vector<std::int32_t> >;
template class DecimationImpl<std::vector<double> >;

}
