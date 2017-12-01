/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/filtering.h"
#include "nds3/impl/filteringImpl.h"

namespace nds
{

template <typename T>
Filtering<T>::Filtering(): Node()
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
Filtering<T>::Filtering( const std::string& name,
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
    Node(std::shared_ptr<FilteringImpl<T> >(new FilteringImpl<T>( name,
																			maxElements,
																		    switchOnFunction,
																		    switchOffFunction,
																		    startFunction,
																		    stopFunction,
																		    recoverFunction,
																			allowStateChangeFunction,
																			PV_EnableFilter_Writer,
																			PV_FilterType_Writer,
																			PV_FilterParams_Writer
																			)))
{
}

template <typename T>
Filtering<T>::Filtering(const Filtering<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
Filtering<T>& Filtering<T>::operator=(const Filtering<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void Filtering<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void Filtering<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t Filtering<T>::getMaxElements()
{
    return std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec Filtering<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
size_t Filtering<T>::getEnableFilter()
{
    return std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->getEnableFilter();
}

template <typename T>
size_t Filtering<T>::getFilterType()
{
    return std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->getFilterType();
}

template <typename T>
std::vector<std::int32_t> Filtering<T>::getFilterParams()
{
    return std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->getFilterParams();
}

template <typename T>
void Filtering<T>::setEnableFilter(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->setEnableFilter(timestamp, value);
}

template <typename T>
void Filtering<T>::setFilterType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->setFilterType(timestamp, value);
}

template <typename T>
void Filtering<T>::setFilterParams(const timespec& timestamp, const std::vector<std::int32_t>& value)
{
    return std::static_pointer_cast<FilteringImpl<T> >(m_pImplementation)->setFilterParams(timestamp, value);
}

template class Filtering<std::int32_t>;
template class Filtering<double>;
template class Filtering<std::vector<std::int8_t> >;
template class Filtering<std::vector<std::uint8_t> >;
template class Filtering<std::vector<std::int32_t> >;
template class Filtering<std::vector<double> >;


}
