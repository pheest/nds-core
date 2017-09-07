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
DataAcquisitionImpl<T>::DataAcquisitionImpl(
        const std::string& name,
        size_t maxElements,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
		writerDouble_t PV_Gain_Writer,
		readerDouble_t PV_Gain_Reader,
		writerDouble_t PV_Offset_Writer,
		readerDouble_t PV_Offset_Reader,
		writerDouble_t PV_Bw_Writer,
		readerDouble_t PV_Bw_Reader,
		writerDouble_t PV_Resolution_Writer,
		readerDouble_t PV_Resolution_Reader,
		writerDouble_t PV_Impedance_Writer,
		readerDouble_t PV_Impedance_Reader,
		writerInt32_t PV_Coupling_Writer,
		readerInt32_t PV_Coupling_Reader,
		writerInt32_t PV_SignalRef_Writer,
		readerInt32_t PV_SignalRef_Reader,
		writerInt32_t PV_Ground_Writer,
		readerInt32_t PV_Ground_Reader):
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


    m_decimationPV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_decimationPV->setDescription("Decimation");
    m_decimationPV->setScanType(scanType_t::passive, 0);
    m_decimationPV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_decimationPV);

    m_Gain_PV.reset(new PVDelegateOutImpl<double>("Gain",PV_Gain_Writer));
    m_Gain_PV->setDescription("Gain of the Channel");
    addChild(m_Gain_PV);

    m_Gain_RBVPV.reset(new PVDelegateInImpl<double>("Gain_RBV",PV_Gain_Reader));
	m_Gain_RBVPV->setDescription("Gain of the Channel");
	m_Gain_RBVPV-> setScanType(scanType_t::passive,0);
	addChild(m_Gain_RBVPV);

	m_offset_PV.reset(new PVDelegateOutImpl<double>("Offset",PV_Offset_Writer));
	m_offset_PV->setDescription("Offset");
	addChild(m_offset_PV);

	m_offset_RBVPV.reset(new PVDelegateInImpl<double>("Offset_RBV",PV_Offset_Reader));
	m_offset_RBVPV->setDescription("Offset ReadBack");
	m_offset_RBVPV->setScanType(scanType_t::passive, 0);
	addChild(m_offset_RBVPV);

	m_BW_PV.reset(new PVDelegateOutImpl<double>("BandWidth",PV_Bw_Writer));
	m_BW_PV->setDescription("BandWidth");
	addChild(m_BW_PV);

	m_BW_RBVPV.reset(new PVDelegateInImpl<double>("BandWidth_RBV",PV_Bw_Reader));
	m_BW_RBVPV->setDescription("BandWidth ReadBack");
	m_BW_RBVPV->setScanType(scanType_t::passive, 0);
	addChild(m_BW_RBVPV);


	m_Resolution_PV.reset(new PVDelegateOutImpl<double>("Resolution",PV_Resolution_Writer));
	m_Resolution_PV->setDescription("Resolution: Number of Bits per Sample");
	addChild(m_Resolution_PV);

	m_Resolution_RBVPV.reset(new PVDelegateInImpl<double>("Resolution_RBV",PV_Resolution_Reader));
	m_Resolution_RBVPV->setDescription("Resolution: Number of Bits per Sample ReadBack");
	m_Resolution_RBVPV->setScanType(scanType_t::passive, 0);
	addChild(m_Resolution_RBVPV);

	m_Impedance_PV.reset(new PVDelegateOutImpl<double>("Impedance",PV_Impedance_Writer));
	m_Impedance_PV->setDescription("Impedance");
	addChild(m_Impedance_PV);

	m_Impedance_RBVPV.reset(new PVDelegateInImpl<double>("Impedance_RBV",PV_Impedance_Reader));
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

	m_Coupling_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("Coupling_RBV",PV_Coupling_Reader));
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

	m_SignalRefType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("SignalRefType_RBV",PV_SignalRef_Reader));
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

    m_ground_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("Ground_RBV",PV_Ground_Reader));
    m_ground_RBVPV->setDescription("Ground State ReadBack");
    m_ground_RBVPV->setEnumeration(groundEnumeratorStrings);
    m_ground_RBVPV->setScanType(scanType_t::interrupt,0);
    addChild(m_ground_RBVPV);

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
size_t DataAcquisitionImpl<T>::getMaxElements()
{
    return m_dataPV->getMaxElements();
}

template<typename T>
size_t DataAcquisitionImpl<T>::getDecimation()
{
    std::int32_t decimation;
    timespec timestamp;
    m_decimationPV->read(&timestamp, &decimation);
    return (size_t)decimation;
}

template<typename T>
timespec DataAcquisitionImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void DataAcquisitionImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void DataAcquisitionImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_dataPV->push(timestamp, data);
}

template<typename T>
void DataAcquisitionImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}


template class DataAcquisitionImpl<std::int32_t>;
template class DataAcquisitionImpl<double>;
template class DataAcquisitionImpl<std::vector<std::int8_t> >;
template class DataAcquisitionImpl<std::vector<std::uint8_t> >;
template class DataAcquisitionImpl<std::vector<std::int32_t> >;
template class DataAcquisitionImpl<std::vector<double> >;
template class DataAcquisitionImpl<std::string >;


}
