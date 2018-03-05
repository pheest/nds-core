/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/definitions.h"
#include "nds3/impl/dataAcquisitionImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

#include <iostream>
#include <list>
#include <algorithm>

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
											writerVectorDouble_t PV_Gain_Writer,
											writerDouble_t PV_Offset_Writer,
											writerDouble_t PV_Bandwidth_Writer,
											writerDouble_t PV_Resolution_Writer,
											writerDouble_t PV_Impedance_Writer,
											writerInt32_t PV_Coupling_Writer,
											writerInt32_t PV_SignalRefType_Writer,
											writerInt32_t PV_Ground_Writer,
										    writerInt32_t PV_DMAEnable_Writer,
											writerDouble_t PV_SamplingRate_Writer,
											writerVectorInt32_t PV_ChannelList_Writer
):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
    // Add the children PVs
    m_Data_PV.reset(new PVVariableInImpl<T>("Data"));
    m_Data_PV->setMaxElements(maxElements);
    m_Data_PV->setDescription("Data Acquired");
    m_Data_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Data_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(),(std::int32_t)1);
    addChild(m_Decimation_PV);

    //add enumeration for Decimation type
    enumerationStrings_t DecTypeEnumeratorStrings;
    DecTypeEnumeratorStrings.push_back("block");
    DecTypeEnumeratorStrings.push_back("sample");

    m_DecimationType_PV.reset(new PVVariableOutImpl<std::int32_t>("DecimationType"));
    m_DecimationType_PV->setDescription("block or sample");
    m_DecimationType_PV->setScanType(scanType_t::passive, 0);
    m_DecimationType_PV->setEnumeration(DecTypeEnumeratorStrings);
	addChild(m_DecimationType_PV);

    m_Gain_PV.reset(new PVDelegateOutImpl<std::vector<double> >("Gain",PV_Gain_Writer));
    m_Gain_PV->setDescription("Gain of the Channel");
    m_Gain_PV->setScanType(scanType_t::passive, 0);
    addChild(m_Gain_PV);

    m_Gain_RBVPV.reset(new PVVariableInImpl<std::vector<double>>("Gain_RBV"));
	m_Gain_RBVPV->setDescription("Gain of the Channel");
	m_Gain_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_Gain_RBVPV);

	m_Offset_PV.reset(new PVDelegateOutImpl<double>("Offset",PV_Offset_Writer));
	m_Offset_PV->setDescription("Offset");
    m_Offset_PV->setScanType(scanType_t::passive, 0);
	addChild(m_Offset_PV);

	m_Offset_RBVPV.reset(new PVVariableInImpl<double>("Offset_RBV"));
	m_Offset_RBVPV->setDescription("Offset ReadBack");
	m_Offset_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Offset_RBVPV);

	m_Bandwidth_PV.reset(new PVDelegateOutImpl<double>("BandWidth",PV_Bandwidth_Writer));
	m_Bandwidth_PV->setDescription("BandWidth");
    m_Bandwidth_PV->setScanType(scanType_t::passive, 0);
	addChild(m_Bandwidth_PV);

	m_Bandwidth_RBVPV.reset(new PVVariableInImpl<double>("BandWidth_RBV"));
	m_Bandwidth_RBVPV->setDescription("BandWidth ReadBack");
	m_Bandwidth_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Bandwidth_RBVPV);


	m_Resolution_PV.reset(new PVDelegateOutImpl<double>("Resolution",PV_Resolution_Writer));
	m_Resolution_PV->setDescription("Number of Bits per Sample");
    m_Resolution_PV->setScanType(scanType_t::passive, 0);
	addChild(m_Resolution_PV);

	m_Resolution_RBVPV.reset(new PVVariableInImpl<double>("Resolution_RBV"));
	m_Resolution_RBVPV->setDescription("Number of Bits per Sample ReadBack");
	m_Resolution_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Resolution_RBVPV);

	m_Impedance_PV.reset(new PVDelegateOutImpl<std::int32_t>("Impedance",PV_Impedance_Writer));
	m_Impedance_PV->setDescription("Impedance");
    m_Impedance_PV->setScanType(scanType_t::passive, 0);
	addChild(m_Impedance_PV);

	m_Impedance_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Impedance_RBV"));
	m_Impedance_RBVPV->setDescription("Impedance ReadBack");
	m_Impedance_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Impedance_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t CouplingEnumeratorStrings;
    CouplingEnumeratorStrings.push_back("AC");
    CouplingEnumeratorStrings.push_back("DC");

	m_Coupling_PV.reset(new PVDelegateOutImpl<std::int32_t>("Coupling",PV_Coupling_Writer));
	m_Coupling_PV->setDescription("AC or DC");
    m_Coupling_PV->setScanType(scanType_t::passive, 0);
	m_Coupling_PV->setEnumeration(CouplingEnumeratorStrings);
	addChild(m_Coupling_PV);

	m_Coupling_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Coupling_RBV"));
	m_Coupling_RBVPV->setDescription("AC or DC ReadBack");
	m_Coupling_RBVPV->setEnumeration(CouplingEnumeratorStrings);
	m_Coupling_RBVPV->setScanType(scanType_t::interrupt,0);
	addChild(m_Coupling_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t SignalRefTypeEnumeratorStrings;
    SignalRefTypeEnumeratorStrings.push_back("SingleEnded");
    SignalRefTypeEnumeratorStrings.push_back("Differential");

	m_SignalRefType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalRefType",PV_SignalRefType_Writer));
	m_SignalRefType_PV->setDescription("Differential or Single Ended");
    m_SignalRefType_PV->setScanType(scanType_t::passive, 0);
	m_SignalRefType_PV->setEnumeration(SignalRefTypeEnumeratorStrings);
	addChild(m_SignalRefType_PV);

	m_SignalRefType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SignalRefType_RBV"));
	m_SignalRefType_RBVPV->setDescription("Differential or Single Ended");
	m_SignalRefType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_SignalRefType_RBVPV->setEnumeration(SignalRefTypeEnumeratorStrings);
	addChild(m_SignalRefType_RBVPV);


    //ad enumerator for ground state
    enumerationStrings_t groundEnumeratorStrings;
    groundEnumeratorStrings.push_back("On");
    groundEnumeratorStrings.push_back("Off");

    m_Ground_PV.reset(new PVDelegateOutImpl<std::int32_t>("Ground",PV_Ground_Writer));
    m_Ground_PV->setDescription("Ground State");
    m_Ground_PV->setScanType(scanType_t::passive, 0);
    m_Ground_PV->setEnumeration(groundEnumeratorStrings);
    addChild(m_Ground_PV);

    m_Ground_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Ground_RBV"));
    m_Ground_RBVPV->setDescription("Ground State ReadBack");
    m_Ground_RBVPV->setEnumeration(groundEnumeratorStrings);
    m_Ground_RBVPV->setScanType(scanType_t::interrupt,0);
    addChild(m_Ground_RBVPV);

	m_NumberOfPushedDataBlocks.reset(new PVVariableInImpl<std::int32_t>("NumberOfPushedDataBlocks"));
	m_NumberOfPushedDataBlocks->setDescription("Number Of Pushed Data Blocks");
	m_NumberOfPushedDataBlocks->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumberOfPushedDataBlocks);

	m_DMABufferSize_PV.reset(new PVVariableInImpl<double>("DMABufferSize"));
	m_DMABufferSize_PV->setDescription("Internal DMA Buffer Size");
	m_DMABufferSize_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DMABufferSize_PV);

	m_DMAEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("DMAEnable",PV_DMAEnable_Writer));
	m_DMAEnable_PV->setDescription("Enable DMA");
	m_DMAEnable_PV->setScanType(scanType_t::passive, 0);
	addChild(m_DMAEnable_PV);

	m_DMAEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("DMAEnable_RBV"));
	m_DMAEnable_RBVPV->setDescription("Enable DMA ReadBack Value");
	m_DMAEnable_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DMAEnable_RBVPV);

	m_DMANumChannels_PV.reset(new PVVariableInImpl<std::int32_t>("DMANumChannels"));
	m_DMANumChannels_PV->setDescription("Number of DMA Channels");
	m_DMANumChannels_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DMANumChannels_PV);

	m_DMAFrameType_PV.reset(new PVVariableInImpl<std::int32_t>("DMAFrameType"));
	m_DMAFrameType_PV->setDescription("DMA Frame Type");
	m_DMAFrameType_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DMAFrameType_PV);

	m_DMASampleSize_PV.reset(new PVVariableInImpl<std::int32_t>("DMASampleSize"));
	m_DMASampleSize_PV->setDescription("DMA Sample Size");
	m_DMASampleSize_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DMASampleSize_PV);

	m_SamplingRate_PV.reset(new PVDelegateOutImpl<double>("SamplingRate",PV_SamplingRate_Writer));
	m_SamplingRate_PV->setDescription("Sampling Rate");
	m_SamplingRate_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SamplingRate_PV);
 
	m_SamplingRate_RBVPV.reset(new PVVariableInImpl<double>("SamplingRate_RBV"));
	m_SamplingRate_RBVPV->setDescription("Sampling Rate ReadBack");
	m_SamplingRate_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SamplingRate_RBVPV);

    m_ChannelList_PV.reset(new PVDelegateOutImpl<std::vector<std::int32_t> >("ChannelList",PV_ChannelList_Writer));
    m_ChannelList_PV->setDescription("List of channels");
    m_ChannelList_PV->setScanType(scanType_t::passive, 0);
    addChild(m_ChannelList_PV);

    m_ChannelList_RBVPV.reset(new PVVariableInImpl<std::vector<std::int32_t>>("ChannelList_RBV"));
    m_ChannelList_RBVPV->setDescription("List of channels");
    m_ChannelList_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_ChannelList_RBVPV);


    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DataAcquisitionImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);
}

template<typename T>
std::vector<double> DataAcquisitionImpl<T>::getGain()
{
    std::vector<double> Gain;
    timespec timestamp;
    m_Gain_RBVPV->read(&timestamp, &Gain);
    return (std::vector<double>)Gain;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getOffset()
{
    double Offset;
    timespec timestamp;
    m_Offset_RBVPV->read(&timestamp, &Offset);
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
size_t DataAcquisitionImpl<T>::getSignalRefType()
{
	std::int32_t SignalRefTYpe;
    timespec timestamp;
    m_SignalRefType_RBVPV->read(&timestamp, &SignalRefTYpe);
    return (std::int32_t)SignalRefTYpe;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getGround()
{
	std::int32_t Ground;
    timespec timestamp;
    m_Ground_RBVPV->read(&timestamp, &Ground);
    return (std::int32_t)Ground;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getMaxElements()
{
    return m_Data_PV->getMaxElements();
}

template<typename T>
timespec DataAcquisitionImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getNumberOfPushedDataBlocks()
{
       std::int32_t NumberOfPushedDataBlocks;
    timespec timestamp;
    m_NumberOfPushedDataBlocks->read(&timestamp, &NumberOfPushedDataBlocks);
    return (std::int32_t)NumberOfPushedDataBlocks;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getDMABufferSize()
{
       double DMABufferSize;
    timespec timestamp;
    m_DMABufferSize_PV->read(&timestamp, &DMABufferSize);
    return (double)DMABufferSize;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getDMAEnable()
{
       std::int32_t DMAEnable;
    timespec timestamp;
    m_DMAEnable_RBVPV->read(&timestamp, &DMAEnable);
    return (std::int32_t)DMAEnable;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getDMANumChannels()
{
       std::int32_t DMANumChannels;
    timespec timestamp;
    m_DMANumChannels_PV->read(&timestamp, &DMANumChannels);
    return (std::int32_t)DMANumChannels;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getDMAFrameType()
{
       std::int32_t DMAFrameType;
    timespec timestamp;
    m_DMAFrameType_PV->read(&timestamp, &DMAFrameType);
    return (std::int32_t)DMAFrameType;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getDMASampleSize()
{
       std::int32_t DMASampleSize;
    timespec timestamp;
    m_DMASampleSize_PV->read(&timestamp, &DMASampleSize);
    return (std::int32_t)DMASampleSize;
}

template<typename T>
size_t DataAcquisitionImpl<T>::getSamplingRate()
{
       double SamplingRate;
    timespec timestamp;
    m_SamplingRate_RBVPV->read(&timestamp, &SamplingRate);
    return (double)SamplingRate;
}

template<typename T>
std::vector<std::int32_t> DataAcquisitionImpl<T>::getChannelList()
{
    std::vector<std::int32_t> ChannelList;
    timespec timestamp;
    m_ChannelList_RBVPV->read(&timestamp, &ChannelList);
    return (std::vector<std::int32_t>)ChannelList;
}

template<typename T>
void DataAcquisitionImpl<T>::setGain(const timespec& timestamp, const std::vector<double>& value)
{
	m_Gain_RBVPV->setValue(timestamp, value);
	m_Gain_RBVPV->push(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setOffset(const timespec& timestamp, const double& value)
{
	m_Offset_RBVPV->setValue(timestamp, value);
	m_Offset_RBVPV->push(timestamp, value);

}

template<typename T>
void DataAcquisitionImpl<T>::setBandwidth(const timespec& timestamp, const double& value)
{
	m_Bandwidth_RBVPV->setValue(timestamp, value);
	m_Bandwidth_RBVPV->push(timestamp, value);

}

template<typename T>
void DataAcquisitionImpl<T>::setResolution(const timespec& timestamp, const double& value)
{
	m_Resolution_RBVPV->setValue(timestamp, value);
	m_Resolution_RBVPV->push(timestamp, value);

}

template<typename T>
void DataAcquisitionImpl<T>::setImpedance(const timespec& timestamp, const std::int32_t& value)
{
	m_Impedance_RBVPV->setValue(timestamp, value);
	m_Impedance_RBVPV->push(timestamp, value);

}

template<typename T>
void DataAcquisitionImpl<T>::setCoupling(const timespec& timestamp, const std::int32_t& value)
{
	m_Coupling_RBVPV->setValue(timestamp, value);
	m_Coupling_RBVPV->push(timestamp, value);

}

template<typename T>
void DataAcquisitionImpl<T>::setSignalRefType(const timespec& timestamp, const std::int32_t& value)
{
	m_SignalRefType_RBVPV->setValue(timestamp, value);
	m_SignalRefType_RBVPV->push(timestamp, value);

}

template<typename T>
void DataAcquisitionImpl<T>::setGround(const timespec& timestamp, const std::int32_t& value)
{
	m_Ground_RBVPV->setValue(timestamp, value);
	m_Ground_RBVPV->push(timestamp, value);

}

template<typename T>
void DataAcquisitionImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void DataAcquisitionImpl<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
	m_NumberOfPushedDataBlocks->setValue(timestamp, value);
	m_NumberOfPushedDataBlocks->push(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setDMABufferSize(const timespec& timestamp, const double& value)
{
	m_DMABufferSize_PV->setValue(timestamp, value);
	m_DMABufferSize_PV->push(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setDMAEnable(const timespec& timestamp, const std::int32_t& value)
{
	m_DMAEnable_RBVPV->setValue(timestamp, value);
	m_DMAEnable_RBVPV->push(timestamp, value);

}

template<typename T>
void DataAcquisitionImpl<T>::setDMANumChannels(const timespec& timestamp, const std::int32_t& value)
{
	m_DMANumChannels_PV->setValue(timestamp, value);
	m_DMANumChannels_PV->push(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setDMAFrameType(const timespec& timestamp, const std::int32_t& value)
{
	m_DMAFrameType_PV->setValue(timestamp, value);
	m_DMAFrameType_PV->push(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setDMASampleSize(const timespec& timestamp, const std::int32_t& value)
{
	m_DMASampleSize_PV->setValue(timestamp, value);
	m_DMASampleSize_PV->push(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setSamplingRate(const timespec& timestamp, const double& value)
{
	m_SamplingRate_RBVPV->setValue(timestamp, value);
	m_SamplingRate_RBVPV->push(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::setChannelList(const timespec& timestamp, const std::vector<std::int32_t>& value)
{
	m_ChannelList_RBVPV->setValue(timestamp, value);
	m_ChannelList_RBVPV->push(timestamp, value);
}

template<typename T>
void DataAcquisitionImpl<T>::push(const timespec& timestamp, const T& data)
{
	m_Data_PV->push(timestamp, data);
}

template<typename T>
void DataAcquisitionImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();

    std::int32_t decType;
    timespec timestamp;
    m_DecimationType_PV->read(&timestamp, &decType);

    if (decType == (std::int32_t)0) m_Data_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());

    m_OnStartDelegate();
}


template class DataAcquisitionImpl<std::int32_t>;
template class DataAcquisitionImpl<double>;
template class DataAcquisitionImpl<std::vector<std::int8_t> >;
template class DataAcquisitionImpl<std::vector<std::uint8_t> >;
template class DataAcquisitionImpl<std::vector<std::int32_t> >;
template class DataAcquisitionImpl<std::vector<double> >;


}
