/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * Modified by GMV & UPM
 */

#include "nds3/dataAcquisition.h"
#include "nds3/impl/dataAcquisitionImpl.h"

namespace nds
{

template <typename T>
DataAcquisition<T>::DataAcquisition(): Node()
{
}

/**
 * @brief Constructs the data acquisition node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
DataAcquisition<T>::DataAcquisition(const std::string& name,
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
    Node(std::shared_ptr<DataAcquisitionImpl<T> >(new DataAcquisitionImpl<T>(name,
                                                                             maxElements,
                                                                             switchOnFunction,
                                                                             switchOffFunction,
                                                                             startFunction,
                                                                             stopFunction,
                                                                             recoverFunction,
                                                                             allowStateChangeFunction,
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
																	 		 PV_Ground_Writer,
																	 		 PV_Ground_Reader)))
{
}

template <typename T>
DataAcquisition<T>::DataAcquisition(const DataAcquisition<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DataAcquisition<T>& DataAcquisition<T>::operator=(const DataAcquisition<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void DataAcquisition<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void DataAcquisition<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t DataAcquisition<T>::getMaxElements()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
size_t DataAcquisition<T>::getDecimation()
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDecimation();
}

template <typename T>
timespec DataAcquisition<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template class DataAcquisition<std::int32_t>;
template class DataAcquisition<double>;
template class DataAcquisition<std::vector<std::int8_t> >;
template class DataAcquisition<std::vector<std::uint8_t> >;
template class DataAcquisition<std::vector<std::int32_t> >;
template class DataAcquisition<std::vector<double> >;
template class DataAcquisition<std::string >;


}
