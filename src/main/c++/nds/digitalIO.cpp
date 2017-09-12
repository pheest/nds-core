/*
 * Nominal Device Support v.3 (NDS3)
 *
 * By GMV & UPM
 *
 */

#include "nds3/digitalIO.h"
#include "nds3/impl/digitalIOImpl.h"

namespace nds
{

template <typename T>
DigitalIO<T>::DigitalIO(): Node()
{
}

/**
 * @brief Constructs the DigitalIO node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
DigitalIO<T>::DigitalIO( const std::string& name,
						 size_t maxElements,
						 stateChange_t switchOnFunction,
						 stateChange_t switchOffFunction,
						 stateChange_t startFunction,
						 stateChange_t stopFunction,
						 stateChange_t recoverFunction,
						 allowChange_t allowStateChangeFunction,
						 writerInt32_t PV_voltLevelHigh_Writer,
						 readerInt32_t PV_voltLevelHigh_Reader,
						 writerInt32_t PV_voltLevelLow_Writer,
						 readerInt32_t PV_voltLevelLow_Reader,
						 writerInt32_t PV_ChannelDir_Writer,
						 readerInt32_t PV_ChannelDir_Reader):

    Node(std::shared_ptr<DigitalIOImpl<T> >(new DigitalIOImpl<T>( name,
																  maxElements,
																  switchOnFunction,
																  switchOffFunction,
																  startFunction,
																  stopFunction,
																  recoverFunction,
																  allowStateChangeFunction,
																  PV_voltLevelHigh_Writer,
																  PV_voltLevelHigh_Reader,
																  PV_voltLevelLow_Writer,
																  PV_voltLevelLow_Reader,
																  PV_ChannelDir_Writer,
																  PV_ChannelDir_Reader)))
{
}

template <typename T>
DigitalIO<T>::DigitalIO(const DigitalIO<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DigitalIO<T>& DigitalIO<T>::operator=(const DigitalIO<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void DigitalIO<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void DigitalIO<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
timespec DigitalIO<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DigitalIOImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template class DigitalIO<std::int32_t>;
template class DigitalIO<double>;
template class DigitalIO<std::vector<std::int8_t> >;
template class DigitalIO<std::vector<std::uint8_t> >;
template class DigitalIO<std::vector<std::int32_t> >;
template class DigitalIO<std::vector<double> >;
template class DigitalIO<std::string >;


}
