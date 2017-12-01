/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/Decimation.h"
#include "nds3/impl/DecimationImpl.h"

namespace nds
{

template <typename T>
Decimation<T>::Decimation(): Node()
{
}

/**
 * @brief Constructs the Decimation processing node
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
Decimation<T>::Decimation( const std::string& name,
		size_t maxElements,
		stateChange_t switchOnFunction,
		stateChange_t switchOffFunction,
		stateChange_t startFunction,
		stateChange_t stopFunction,
		stateChange_t recoverFunction,
		allowChange_t allowStateChangeFunction,
		writerInt32_t PV_DecimationEnable_Writer,
		writerInt32_t PV_DecimationType_Writer,
		writerInt32_t PV_DecimationFactor_Writer,
		writerInt32_t PV_DecimationOffset_Writer
):
Node(std::shared_ptr<DecimationImpl<T> >(new DecimationImpl<T>( name,
		maxElements,
		switchOnFunction,
		switchOffFunction,
		startFunction,
		stopFunction,
		recoverFunction,
		allowStateChangeFunction,
		PV_DecimationEnable_Writer,
		PV_DecimationType_Writer,
		PV_DecimationFactor_Writer,
		PV_DecimationOffset_Writer
)))
{
}

template <typename T>
Decimation<T>::Decimation(const Decimation<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
Decimation<T>& Decimation<T>::operator=(const Decimation<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void Decimation<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void Decimation<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t Decimation<T>::getMaxElements()
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec Decimation<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
size_t Decimation<T>::getDecimationEnable()
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->getDecimationEnable();
}

template <typename T>
size_t Decimation<T>::getDecimationType()
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->getDecimationType();
}

template <typename T>
size_t Decimation<T>::getDecimationFactor()
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->getDecimationFactor();
}

template <typename T>
size_t Decimation<T>::getDecimationOffset()
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->getDecimationOffset();
}

template <typename T>
void Decimation<T>::setDecimationEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->setDecimationEnable(timestamp, value);
}

template <typename T>
void Decimation<T>::setDecimationType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->setDecimationType(timestamp, value);
}

template <typename T>
void Decimation<T>::setDecimationFactor(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->setDecimationFactor(timestamp, value);
}

template <typename T>
void Decimation<T>::setDecimationOffset(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<DecimationImpl<T> >(m_pImplementation)->setDecimationOffset(timestamp, value);
}


template class Decimation<std::int32_t>;
template class Decimation<double>;
template class Decimation<std::vector<std::int8_t> >;
template class Decimation<std::vector<std::uint8_t> >;
template class Decimation<std::vector<std::int32_t> >;
template class Decimation<std::vector<double> >;


}
