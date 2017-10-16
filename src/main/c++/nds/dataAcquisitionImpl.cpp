/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * Modified by GMV & UPM
 *
 */


#include "nds3/definitions.h"
#include "nds3/impl/dataAcquisitionImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

template<typename T>
DataAcquisitionImpl<T>::DataAcquisitionImpl(const std::string& name,
											size_t maxElements,
											stateChange_t switchOnFunction,
											stateChange_t switchOffFunction,
											stateChange_t startFunction,
											stateChange_t stopFunction,
											stateChange_t recoverFunction,
											allowChange_t allowStateChangeFunction,
											writerDouble_t PV_Gain_Writer,
											writerDouble_t PV_Offset_Writer,
											writerDouble_t PV_Bandwidth_Writer,
											writerDouble_t PV_Resolution_Writer,
											writerDouble_t PV_Impedance_Writer,
											writerInt32_t PV_Coupling_Writer,
											writerInt32_t PV_SignalRef_Writer,
											writerInt32_t PV_Ground_Writer):
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

    m_decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_decimation_PV->setDescription("Decimation");
    m_decimation_PV->setScanType(scanType_t::passive, 0);
    m_decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_decimation_PV);

    m_Gain_PV.reset(new PVDelegateOutImpl<double>("Gain",PV_Gain_Writer));
    m_Gain_PV->setDescription("Gain of the Channel");
    addChild(m_Gain_PV);

    m_Gain_RBVPV.reset(new PVVariableInImpl<double>("Gain_RBV"));
	m_Gain_RBVPV->setDescription("Gain of the Channel");
	m_Gain_RBVPV-> setScanType(scanType_t::passive,0);
	addChild(m_Gain_RBVPV);

	m_offset_PV.reset(new PVDelegateOutImpl<double>("Offset",PV_Offset_Writer));
	m_offset_PV->setDescription("Offset");
	addChild(m_offset_PV);

	m_offset_RBVPV.reset(new PVVariableInImpl<double>("Offset_RBV"));
	m_offset_RBVPV->setDescription("Offset ReadBack");
	m_offset_RBVPV->setScanType(scanType_t::passive, 0);
	addChild(m_offset_RBVPV);

	m_Bandwidth_PV.reset(new PVDelegateOutImpl<double>("BandWidth",PV_Bandwidth_Writer));
	m_Bandwidth_PV->setDescription("BandWidth");
	addChild(m_Bandwidth_PV);

	m_Bandwidth_RBVPV.reset(new PVVariableInImpl<double>("BandWidth_RBV"));
	m_Bandwidth_RBVPV->setDescription("BandWidth ReadBack");
	m_Bandwidth_RBVPV->setScanType(scanType_t::passive, 0);
	addChild(m_Bandwidth_RBVPV);


	m_Resolution_PV.reset(new PVDelegateOutImpl<double>("Resolution",PV_Resolution_Writer));
	m_Resolution_PV->setDescription("Resolution: Number of Bits per Sample");
	addChild(m_Resolution_PV);

	m_Resolution_RBVPV.reset(new PVVariableInImpl<double>("Resolution_RBV"));
	m_Resolution_RBVPV->setDescription("Resolution: Number of Bits per Sample ReadBack");
	m_Resolution_RBVPV->setScanType(scanType_t::passive, 0);
	addChild(m_Resolution_RBVPV);

	m_Impedance_PV.reset(new PVDelegateOutImpl<std::int32_t>("Impedance",PV_Impedance_Writer));
	m_Impedance_PV->setDescription("Impedance");
	addChild(m_Impedance_PV);

	m_Impedance_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Impedance_RBV"));
	m_Impedance_RBVPV->setDescription("Impedance ReadBack");
	m_Impedance_RBVPV->setScanType(scanType_t::passive, 0);
	addChild(m_Impedance_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t CouplingEnumeratorStrings;
    CouplingEnumeratorStrings.push_back("AC");
    CouplingEnumeratorStrings.push_back("DC");

	m_Coupling_PV.reset(new PVDelegateOutImpl<std::int32_t>("Coupling",PV_Coupling_Writer));
	m_Coupling_PV->setDescription("Coupling: AC or DC");
	m_Coupling_PV->setEnumeration(CouplingEnumeratorStrings);
	addChild(m_Coupling_PV);

	m_Coupling_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Coupling_RBV"));
	m_Coupling_RBVPV->setDescription("Coupling: AC or DC ReadBack");
	m_Coupling_RBVPV->setEnumeration(CouplingEnumeratorStrings);
	m_Coupling_RBVPV->setScanType(scanType_t::interrupt,0);
	addChild(m_Coupling_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t SignalRefTypeEnumeratorStrings;
    SignalRefTypeEnumeratorStrings.push_back("SingleEnded");
    SignalRefTypeEnumeratorStrings.push_back("Differential");

	m_SignalRefType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalRefType",PV_SignalRef_Writer));
	m_SignalRefType_PV->setDescription("Type of input: Differential or Single Ended");
	m_SignalRefType_PV->setEnumeration(SignalRefTypeEnumeratorStrings);
	addChild(m_SignalRefType_PV);

	m_SignalRefType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SignalRefType_RBV"));
	m_SignalRefType_RBVPV->setDescription("Type of input: Differential or Single Ended");
	m_SignalRefType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_SignalRefType_RBVPV->setEnumeration(SignalRefTypeEnumeratorStrings);
	addChild(m_SignalRefType_RBVPV);


    //ad enumerator for ground state
    enumerationStrings_t groundEnumeratorStrings;
    groundEnumeratorStrings.push_back("On");
    groundEnumeratorStrings.push_back("Off");

    m_ground_PV.reset(new PVDelegateOutImpl<std::int32_t>("Ground",PV_Ground_Writer));
    m_ground_PV->setDescription("Ground State");
    m_ground_PV->setEnumeration(groundEnumeratorStrings);
    addChild(m_ground_PV);

    m_ground_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Ground_RBV"));
    m_ground_RBVPV->setDescription("Ground State ReadBack");
    m_ground_RBVPV->setEnumeration(groundEnumeratorStrings);
    m_ground_RBVPV->setScanType(scanType_t::interrupt,0);
    addChild(m_ground_RBVPV);

	m_NumberOfPushedDataBlocks.reset(new PVVariableInImpl<std::int32_t>("NumberOfPushedDataBlocks"));
	m_NumberOfPushedDataBlocks->setDescription("Number Of Pushed Data Blocks");
	m_NumberOfPushedDataBlocks->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumberOfPushedDataBlocks);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DataAcquisitionImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);
}

template<typename T>
size_t DataAcquisitionImpl<T>::getGain()
{
    double Gain;
    timespec timestamp;
    m_Gain_RBVPV->read(&timestamp, &Gain);
    return (double)Gain;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getOffset()
{
    double Offset;
    timespec timestamp;
    m_offset_RBVPV->read(&timestamp, &Offset);
    return (double)Offset;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getBandwidth()
{
    double Bandwidth;
    timespec timestamp;
    m_Bandwidth_RBVPV->read(&timestamp, &Bandwidth);
    return (double)Bandwidth;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getResolution()
{
    double Resolution;
    timespec timestamp;
    m_Resolution_RBVPV->read(&timestamp, &Resolution);
    return (double)Resolution;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getImpedance()
{
	std::int32_t Impedance;
    timespec timestamp;
    m_Impedance_RBVPV->read(&timestamp, &Impedance);
    return (std::int32_t)Impedance;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getCoupling()
{
    std::int32_t Coupling;
    timespec timestamp;
    m_Coupling_RBVPV->read(&timestamp, &Coupling);
    return (std::int32_t)Coupling;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getSignalRef()
{
	std::int32_t SignalRef;
    timespec timestamp;
    m_SignalRefType_RBVPV->read(&timestamp, &SignalRef);
    return (std::int32_t)SignalRef;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getGround()
{
	std::int32_t Ground;
    timespec timestamp;
    m_ground_RBVPV->read(&timestamp, &Ground);
    return (std::int32_t)Ground;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getMaxElements()
{
    return m_data_PV->getMaxElements();
}

template<typename T>
timespec DataAcquisitionImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void DataAcquisitionImpl<T>::setGain(const timespec& timestamp, const double& value)
{
	m_Gain_RBVPV->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setOffset(const timespec& timestamp, const double& value)
{
	m_offset_RBVPV->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setBandwidth(const timespec& timestamp, const double& value)
{
	m_Bandwidth_RBVPV->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setResolution(const timespec& timestamp, const double& value)
{
	m_Resolution_RBVPV->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setImpedance(const timespec& timestamp, const std::int32_t& value)
{
	m_Impedance_RBVPV->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setCoupling(const timespec& timestamp, const std::int32_t& value)
{
	m_Coupling_RBVPV->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setSignalRef(const timespec& timestamp, const std::int32_t& value)
{
	m_SignalRefType_RBVPV->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setGround(const timespec& timestamp, const std::int32_t& value)
{
	m_ground_RBVPV->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void DataAcquisitionImpl<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
	m_NumberOfPushedDataBlocks->setValue(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::push(const timespec& timestamp, const T& data)
{
	m_data_PV->push(timestamp, data);
}

template<typename T>
void DataAcquisitionImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_data_PV->setDecimation((std::uint32_t)m_decimation_PV->getValue());
    m_onStartDelegate();
}


template class DataAcquisitionImpl<std::int32_t>;
template class DataAcquisitionImpl<double>;
template class DataAcquisitionImpl<std::vector<std::int8_t> >;
template class DataAcquisitionImpl<std::vector<std::uint8_t> >;
template class DataAcquisitionImpl<std::vector<std::int32_t> >;
template class DataAcquisitionImpl<std::vector<double> >;


}
