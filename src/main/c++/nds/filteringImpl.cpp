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
										   readerInt32_t PV_EnableFilter_Reader,
										   writerInt32_t PV_FilterType_Writer,
										   readerInt32_t PV_FilterType_Reader,
										   writerVectorInt32_t PV_FilterParams_Writer,
										   readerVectorInt32_t PV_FilterParams_Reader
										  ):
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

    m_decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_decimation_PV->setDescription("Decimation");
    m_decimation_PV->setScanType(scanType_t::passive, 0);
    m_decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_decimation_PV);

	//add enumeration
	enumerationStrings_t enableFiterEnumeratorStrings;
	enableFiterEnumeratorStrings.push_back("On");
	enableFiterEnumeratorStrings.push_back("Off");

	m_enableFilter_PV.reset(new PVDelegateOutImpl<std::int32_t>("enableFilter",PV_EnableFilter_Writer));
	m_enableFilter_PV->setDescription("Enable filter");
	m_enableFilter_PV->setEnumeration(enableFiterEnumeratorStrings);
	m_enableFilter_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_enableFilter_PV);

	m_enableFilter_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("enableFilter_RBV",PV_EnableFilter_Reader));
	m_enableFilter_RBVPV->setDescription("Enable filter ReadBack");
	m_enableFilter_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_enableFilter_RBVPV->setEnumeration(enableFiterEnumeratorStrings);
	addChild(m_enableFilter_RBVPV);

	enumerationStrings_t fiterTypeEnumeratorStrings;
	fiterTypeEnumeratorStrings.push_back("SW");
	fiterTypeEnumeratorStrings.push_back("Polynom");
	fiterTypeEnumeratorStrings.push_back("LPass");
	fiterTypeEnumeratorStrings.push_back("HPass");

	m_FilterType_PV.reset(new PVDelegateOutImpl<std::int32_t>("filterType",PV_FilterType_Writer));
	m_FilterType_PV->setDescription("Filter types: SW, Polynomial, LowPass, HighPass");
	m_FilterType_PV->setEnumeration(fiterTypeEnumeratorStrings);
	m_FilterType_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_FilterType_PV);


	m_FilterType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("filterType_RBV",PV_FilterType_Reader));
	m_FilterType_RBVPV->setDescription("Filter types: SW, Polynomial, LowPass, HighPass");
	m_FilterType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_FilterType_RBVPV->setEnumeration(fiterTypeEnumeratorStrings);
	addChild(m_FilterType_RBVPV);


	m_FilterParams_PV.reset(new PVDelegateOutImpl<std::vector<std::int32_t> >("filterParams",PV_FilterParams_Writer));
	m_FilterParams_PV->setDescription("Filter Parameters");
	m_FilterParams_PV->setEnumeration(fiterTypeEnumeratorStrings);
	m_FilterParams_PV->setMaxElements(100);//TODO TBD
	addChild(m_FilterParams_PV);

	m_FilterParams_RBVPV.reset(new PVDelegateInImpl<std::vector<std::int32_t> >("filterParams_RBV",PV_FilterParams_Reader));
	m_FilterParams_RBVPV->setDescription("Filter Parameters");
	m_FilterParams_RBVPV->setScanType(scanType_t::passive, 0);
	m_FilterParams_RBVPV->setEnumeration(fiterTypeEnumeratorStrings);
	addChild(m_FilterParams_RBVPV);





    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&FilteringImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);

}

template<typename T>
timespec FilteringImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void FilteringImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void FilteringImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_data_PV->push(timestamp, data);
}

template<typename T>
size_t FilteringImpl<T>::getMaxElements()
{
    return m_data_PV->getMaxElements();
}

template<typename T>
void FilteringImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_data_PV->setDecimation((std::uint32_t)m_decimation_PV->getValue());
    m_onStartDelegate();
}


template class FilteringImpl<std::int32_t>;
template class FilteringImpl<double>;
template class FilteringImpl<std::vector<std::int8_t> >;
template class FilteringImpl<std::vector<std::uint8_t> >;
template class FilteringImpl<std::vector<std::int32_t> >;
template class FilteringImpl<std::vector<double> >;
template class FilteringImpl<std::string >;


}
