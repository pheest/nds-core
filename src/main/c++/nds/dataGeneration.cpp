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
							      readerDouble_t PV_Frequency_Reader,
							      writerDouble_t PV_RefFrequency_Writer,
							      readerDouble_t PV_RefFrequency_Reader,
							      writerDouble_t PV_Amp_Writer,
							      readerDouble_t PV_Amp_Reader,
							      writerDouble_t PV_Phase_Writer,
							      readerDouble_t PV_Phase_Reader,
							      writerDouble_t PV_UpdateRate_Writer,
							      readerDouble_t PV_UpdateRate_Reader,
							      writerDouble_t PV_DutyCycle_Writer,
							      readerDouble_t PV_DutyCycle_Reader,
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
							      writerInt32_t PV_SignalType_Writer,
							      readerInt32_t PV_SignalType_Reader,
							      writerInt32_t PV_Ground_Writer,
							      readerInt32_t PV_Ground_Reader):
    Node(std::shared_ptr<DataGenerationImpl<T> >(new DataGenerationImpl<T>(name,
															               maxElements,
															               switchOnFunction,
															               switchOffFunction,
															               startFunction,
															               stopFunction,
															               recoverFunction,
															               allowStateChangeFunction,
															               PV_Frequency_Writer,
															               PV_Frequency_Reader,
															               PV_RefFrequency_Writer,
															               PV_RefFrequency_Reader,
															               PV_Amp_Writer,
															               PV_Amp_Reader,
															               PV_Phase_Writer,
															               PV_Phase_Reader,
															               PV_UpdateRate_Writer,
															               PV_UpdateRate_Reader,
															               PV_DutyCycle_Writer,
															               PV_DutyCycle_Reader,
															               PV_Gain_Writer,
															               PV_Gain_Reader,
															               PV_Offset_Writer,
															               PV_Offset_Reader,
															               PV_Bw_Writer,
															               PV_Bw_Reader,
															               PV_Resolution_Writer,
															               PV_Resolution_Reader,
															               PV_Impedance_Writer,
															               PV_Impedance_Reader,
															               PV_Coupling_Writer,
															               PV_Coupling_Reader,
															               PV_SignalRef_Writer,
															               PV_SignalRef_Reader,
															               PV_SignalType_Writer,
															               PV_SignalType_Reader,
															               PV_Ground_Writer,
															               PV_Ground_Reader)))
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
void DataGeneration<T>::write(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->write(timestamp, data);
}

template <typename T>
timespec DataGeneration<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DataGenerationImpl<T> >(m_pImplementation)->getStartTimestamp();
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

template class DataGeneration<std::int32_t>;
template class DataGeneration<double>;
template class DataGeneration<std::vector<std::int8_t> >;
template class DataGeneration<std::vector<std::uint8_t> >;
template class DataGeneration<std::vector<std::int32_t> >;
template class DataGeneration<std::vector<double> >;
template class DataGeneration<std::string >;


}
