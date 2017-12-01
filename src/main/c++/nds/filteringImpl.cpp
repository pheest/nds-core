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
#include "nds3/impl/filteringImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
FilteringImpl<T>::FilteringImpl( const std::string& name,
										   size_t maxElements,
										   stateChange_t switchOnFunction,
										   stateChange_t switchOffFunction,
										   stateChange_t startFunction,
										   stateChange_t stopFunction,
										   stateChange_t recoverFunction,
										   allowChange_t allowStateChangeFunction,
										   writerInt32_t PV_EnableFilter_Writer,
										   writerInt32_t PV_FilterType_Writer,
										   writerVectorInt32_t PV_FilterParams_Writer
										  ):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_DataIn_PV.reset(new PVVariableInImpl<T>("DataIn"));
    m_DataIn_PV->setMaxElements(maxElements);
    m_DataIn_PV->setDescription("Data received in the filtering node");
    m_DataIn_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataIn_PV);

    m_DataOut_PV.reset(new PVVariableInImpl<T>("DataOut"));
    m_DataOut_PV->setMaxElements(maxElements);
    m_DataOut_PV->setDescription("Data produced by the filtering node");
    m_DataOut_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataOut_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_Decimation_PV);

	//add enumeration
	enumerationStrings_t enableFiterEnumeratorStrings;
	enableFiterEnumeratorStrings.push_back("On");
	enableFiterEnumeratorStrings.push_back("Off");

	m_EnableFilter_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableFilter",PV_EnableFilter_Writer));
	m_EnableFilter_PV->setDescription("Enable/Disable filter operation");
	m_EnableFilter_PV->setEnumeration(enableFiterEnumeratorStrings);
	m_EnableFilter_PV->setScanType(scanType_t::passive, 0);
	addChild(m_EnableFilter_PV);

	m_EnableFilter_RBVPV.reset(new PVVariableInImpl<std::int32_t>("EnableFilter_RBV"));
	m_EnableFilter_RBVPV->setDescription("Enable/Disable filter operation ReadBack");
	m_EnableFilter_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_EnableFilter_RBVPV->setEnumeration(enableFiterEnumeratorStrings);
	addChild(m_EnableFilter_RBVPV);

	enumerationStrings_t fiterTypeEnumeratorStrings;
	fiterTypeEnumeratorStrings.push_back("SW");
	fiterTypeEnumeratorStrings.push_back("Polynom");
	fiterTypeEnumeratorStrings.push_back("LPass");
	fiterTypeEnumeratorStrings.push_back("HPass");

	m_FilterType_PV.reset(new PVDelegateOutImpl<std::int32_t>("FilterType",PV_FilterType_Writer));
	m_FilterType_PV->setDescription("Filter types: SW, Polynomial, LowPass, HighPass");
	m_FilterType_PV->setEnumeration(fiterTypeEnumeratorStrings);
	m_FilterType_PV->setScanType(scanType_t::passive, 0);
	addChild(m_FilterType_PV);


	m_FilterType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("FilterType_RBV"));
	m_FilterType_RBVPV->setDescription("Filter types: SW, Polynomial, LowPass, HighPass");
	m_FilterType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_FilterType_RBVPV->setEnumeration(fiterTypeEnumeratorStrings);
	addChild(m_FilterType_RBVPV);


	m_FilterParams_PV.reset(new PVDelegateOutImpl<std::vector<std::int32_t> >("FilterParams",PV_FilterParams_Writer));
	m_FilterParams_PV->setDescription("Filter Polynomial Coefficients");
	m_FilterParams_PV->setEnumeration(fiterTypeEnumeratorStrings);
	m_FilterParams_PV->setScanType(scanType_t::passive, 0);
	m_FilterParams_PV->setMaxElements(100);//TODO Define max number of coefficients
	addChild(m_FilterParams_PV);

	m_FilterParams_RBVPV.reset(new PVVariableInImpl<std::vector<std::int32_t> >("FilterParams_RBV"));
	m_FilterParams_RBVPV->setDescription("Filter Polynomial Coefficients Readback");
	m_FilterParams_RBVPV->setEnumeration(fiterTypeEnumeratorStrings);
	m_FilterParams_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_FilterParams_RBVPV->setMaxElements(100);//TODO Define max number of coefficients
	addChild(m_FilterParams_RBVPV);





    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&FilteringImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);

}

template<typename T>
timespec FilteringImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void FilteringImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void FilteringImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_DataOut_PV->push(timestamp, data);
}

template<typename T>
size_t FilteringImpl<T>::getMaxElements()
{
    return m_DataOut_PV->getMaxElements();
}

template<typename T>
void FilteringImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_DataOut_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_DataIn_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_OnStartDelegate();
}

template<typename T>
size_t FilteringImpl<T>::getEnableFilter()
{
	std::int32_t EnableFilter;
	timespec timestamp;
	m_EnableFilter_RBVPV->read(&timestamp, &EnableFilter);
	return (std::int32_t)EnableFilter;
}

template<typename T>
size_t FilteringImpl<T>::getFilterType()
{
	std::int32_t FilterType;
	timespec timestamp;
	m_FilterType_RBVPV->read(&timestamp, &FilterType);
	return (std::int32_t)FilterType;
}

template<typename T>
std::vector<std::int32_t> FilteringImpl<T>::getFilterParams()
{
	std::vector<std::int32_t> FilterParams;
	timespec timestamp;
	m_FilterParams_RBVPV->read(&timestamp, &FilterParams);
	return (std::vector<std::int32_t>)FilterParams;
}

template<typename T>
void FilteringImpl<T>::setEnableFilter(const timespec& timestamp, const std::int32_t& value)
{
	m_EnableFilter_RBVPV->setValue(timestamp, value);
	m_EnableFilter_RBVPV->push(timestamp, value);
}

template<typename T>
void FilteringImpl<T>::setFilterType(const timespec& timestamp, const std::int32_t& value)
{
	m_FilterType_RBVPV->setValue(timestamp, value);
	m_FilterType_RBVPV->push(timestamp, value);
}

template<typename T>
void FilteringImpl<T>::setFilterParams(const timespec& timestamp, const std::vector<std::int32_t>& value)
{
	m_FilterParams_RBVPV->setValue(timestamp, value);
	m_FilterParams_RBVPV->push(timestamp, value);
}

template class FilteringImpl<std::int32_t>;
template class FilteringImpl<double>;
template class FilteringImpl<std::vector<std::int8_t> >;
template class FilteringImpl<std::vector<std::uint8_t> >;
template class FilteringImpl<std::vector<std::int32_t> >;
template class FilteringImpl<std::vector<double> >;

}
