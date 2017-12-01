/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/dataProcessing.h"
#include "nds3/impl/dataProcessingImpl.h"

namespace nds
{

template <typename T>
DataProcessing<T>::DataProcessing(): Node()
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
DataProcessing<T>::DataProcessing( const std::string& name,
		                           size_t maxElements,
							       stateChange_t switchOnFunction,
							       stateChange_t switchOffFunction,
							       stateChange_t startFunction,
							       stateChange_t stopFunction,
							       stateChange_t recoverFunction,
								   allowChange_t allowStateChangeFunction
		                          ):
    Node(std::shared_ptr<DataProcessingImpl<T> >(new DataProcessingImpl<T>( name,
																			maxElements,
																		    switchOnFunction,
																		    switchOffFunction,
																		    startFunction,
																		    stopFunction,
																		    recoverFunction,
																			allowStateChangeFunction
																			)))
{
}

template <typename T>
DataProcessing<T>::DataProcessing(const DataProcessing<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DataProcessing<T>& DataProcessing<T>::operator=(const DataProcessing<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void DataProcessing<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DataProcessingImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void DataProcessing<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DataProcessingImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t DataProcessing<T>::getMaxElements()
{
    return std::static_pointer_cast<DataProcessingImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec DataProcessing<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DataProcessingImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template class DataProcessing<std::int32_t>;
template class DataProcessing<double>;
template class DataProcessing<std::vector<std::int8_t> >;
template class DataProcessing<std::vector<std::uint8_t> >;
template class DataProcessing<std::vector<std::int32_t> >;
template class DataProcessing<std::vector<double> >;
template class DataProcessing<std::string >;


}
