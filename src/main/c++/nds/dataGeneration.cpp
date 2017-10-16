/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/dataGeneration.h"
#include "nds3/impl/dataGenerationImpl.h"

namespace nds
{

template <typename T>
DataGeneration<T>::DataGeneration(): Node()
{
}

/**
 * @brief Constructs the data generation node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the array
 */
template <typename T>
DataGeneration<T>::DataGeneration(const std::string& name,
							      size_t maxElements,
							      stateChange_t switchOnFunction,
							      stateChange_t switchOffFunction,
							      stateChange_t startFunction,
							      stateChange_t stopFunction,
							      stateChange_t recoverFunction,
							      allowChange_t allowStateChangeFunction,
							      writerDouble_t PV_Frequency_Writer,
							      writerDouble_t PV_RefFrequency_Writer,
							      writerDouble_t PV_Amp_Writer,
							      writerDouble_t PV_Phase_Writer,
							      writerDouble_t PV_UpdateRate_Writer,
							      writerDouble_t PV_DutyCycle_Writer,
							      writerDouble_t PV_Gain_Writer,
							      writerDouble_t PV_Offset_Writer,
							      writerDouble_t PV_Bandwidth_Writer,
							      writerDouble_t PV_Resolution_Writer,
								  writerInt32_t PV_Impedance_Writer,
							      writerInt32_t PV_Coupling_Writer,
							      writerInt32_t PV_SignalRef_Writer,
							      writerInt32_t PV_SignalType_Writer,
							      writerInt32_t PV_Ground_Writer):
    Node(std::shared_ptr<DataGenerationImpl<T> >(new DataGenerationImpl<T>(name,
															               maxElements,
															               switchOnFunction,
															               switchOffFunction,
															               startFunction,
															               stopFunction,
															               recoverFunction,
															               allowStateChangeFunction,
															               PV_Frequency_Writer,
															               PV_RefFrequency_Writer,
															               PV_Amp_Writer,
															               PV_Phase_Writer,
															               PV_UpdateRate_Writer,
															               PV_DutyCycle_Writer,
															               PV_Gain_Writer,
															               PV_Offset_Writer,
															               PV_Bandwidth_Writer,
															               PV_Resolution_Writer,
															               PV_Impedance_Writer,
															               PV_Coupling_Writer,
															               PV_SignalRef_Writer,
															               PV_SignalType_Writer,
															               PV_Ground_Writer)))
{
}

template <typename T>
DataGeneration<T>::DataGeneration(const DataGeneration<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DataGeneration<T>& DataGeneration<T>::operator=(const DataGeneration<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void DataGeneration<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
timespec DataGeneration<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
void DataGeneration<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t DataGeneration<T>::getMaxElements()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
size_t DataGeneration<T>::getSignalType()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getSignalType();
}

template <typename T>
size_t DataGeneration<T>::getAmplitude()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getAmplitude();
}

template <typename T>
size_t DataGeneration<T>::getFrequency()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getFrequency();
}

template <typename T>
size_t DataGeneration<T>::getUpdateRate()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getUpdateRate();
}

template <typename T>
size_t DataGeneration<T>::getOffset()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getOffset();
}

template <typename T>
size_t DataGeneration<T>::getPhase()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getPhase();
}

template <typename T>
size_t DataGeneration<T>::getImpedance()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getImpedance();
}

template <typename T>
size_t DataGeneration<T>::getRefFrequency()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getRefFrequency();
}

template <typename T>
size_t DataGeneration<T>::getDutyCycle()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getDutyCycle();
}

template <typename T>
size_t DataGeneration<T>::getGain()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getGain();
}

template <typename T>
size_t DataGeneration<T>::getBandwidth()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getBandwidth();
}

template <typename T>
size_t DataGeneration<T>::getResolution()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getResolution();
}

template <typename T>
size_t DataGeneration<T>::getCoupling()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getCoupling();
}

template <typename T>
size_t DataGeneration<T>::getSignalRef()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getSignalRef();
}

template <typename T>
size_t DataGeneration<T>::getGround()
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getGround();
}

template <typename T>
void DataGeneration<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setNumberOfPushedDataBlocks(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setAmplitude(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setAmplitude(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setSignalType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setSignalType(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setFrequency(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setFrequency(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setUpdateRate(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setUpdateRate(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setOffset(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setOffset(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setPhase(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setPhase(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setImpedance(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setImpedance(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setRefFrequency(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setRefFrequency(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setDutyCycle(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setDutyCycle(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setGain(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setGain(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setBandwidth(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setBandwidth(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setResolution(const timespec& timestamp, const double& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setResolution(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setCoupling(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setCoupling(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setSignalRef(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setSignalRef(timestamp, value);
}

template <typename T>
void DataGeneration<T>::setGround(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->setGround(timestamp, value);
}

template class DataGeneration<std::int32_t>;
template class DataGeneration<double>;
template class DataGeneration<std::vector<std::int8_t> >;
template class DataGeneration<std::vector<std::uint8_t> >;
template class DataGeneration<std::vector<std::int32_t> >;
template class DataGeneration<std::vector<double> >;

}
