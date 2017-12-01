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
#include "nds3/impl/dataProcessingImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
DataProcessingImpl<T>::DataProcessingImpl( const std::string& name,
										   size_t maxElements,
										   stateChange_t switchOnFunction,
										   stateChange_t switchOffFunction,
										   stateChange_t startFunction,
										   stateChange_t stopFunction,
										   stateChange_t recoverFunction,
										   allowChange_t allowStateChangeFunction
										   ):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{

	// Add the children PVs
	m_data_PV.reset(new PVVariableInImpl<T>("data"));
	m_data_PV->setMaxElements(maxElements);
	m_data_PV->setDescription("Acquired data");
	    m_data_PV->setScanType(scanType_t::interrupt, 0);
	    addChild(m_data_PV);
    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DataProcessingImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);

}

template<typename T>
timespec DataProcessingImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void DataProcessingImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void DataProcessingImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_data_PV->push(timestamp, data);
}

template<typename T>
size_t DataProcessingImpl<T>::getMaxElements()
{
    return m_data_PV->getMaxElements();
}

template<typename T>
void DataProcessingImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_data_PV->setDecimation((std::uint32_t)m_decimation_PV->getValue());
    m_onStartDelegate();
}


template class DataProcessingImpl<std::int32_t>;
template class DataProcessingImpl<double>;
template class DataProcessingImpl<std::vector<std::int8_t> >;
template class DataProcessingImpl<std::vector<std::uint8_t> >;
template class DataProcessingImpl<std::vector<std::int32_t> >;
template class DataProcessingImpl<std::vector<double> >;
template class DataProcessingImpl<std::string >;


}
