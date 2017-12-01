/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/Streaming.h"
#include "nds3/impl/StreamingImpl.h"

namespace nds
{


template <typename T>
Streaming<T>::Streaming(): Node()
{
}

/**
 * @brief Constructs the Streaming node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
Streaming<T>::Streaming( const std::string& name,
							     size_t maxElements,
								 stateChange_t switchOnFunction,
								 stateChange_t switchOffFunction,
								 stateChange_t startFunction,
								 stateChange_t stopFunction,
								 stateChange_t recoverFunction,
								 allowChange_t allowStateChangeFunction,
								 writerInt32_t PV_BufferSize_Writer,
								 writerInt32_t PV_StreamingDataFormat_Writer,
							     writerInt32_t PV_StreamingType_Writer):
    Node(std::shared_ptr<StreamingImpl<T> >(new StreamingImpl<T>( name,
																		  maxElements,
																		  switchOnFunction,
																		  switchOffFunction,
																		  startFunction,
																	      stopFunction,
																		  recoverFunction,
																		  allowStateChangeFunction,
																		  PV_BufferSize_Writer,
																		  PV_StreamingDataFormat_Writer,
																		  PV_StreamingType_Writer)))
{
}

template <typename T>
Streaming<T>::Streaming(const Streaming<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
Streaming<T>& Streaming<T>::operator=(const Streaming<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void Streaming<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void Streaming<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t Streaming<T>::getMaxElements()
{
    return std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec Streaming<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
size_t Streaming<T>::getBufferSize()
{
    return std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->getBufferSize();
}

template <typename T>
size_t Streaming<T>::getStreamingType()
{
    return std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->getStreamingType();
}

template <typename T>
size_t Streaming<T>::getStreamingDataFormat()
{
    return std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->getStreamingDataFormat();
}

template <typename T>
void Streaming<T>::setBufferSize(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->setBufferSize(timestamp, value);
}

template <typename T>
void Streaming<T>::setStreamingType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->setStreamingType(timestamp, value);
}

template <typename T>
void Streaming<T>::setStreamingDataFormat(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<StreamingImpl<T> >(m_pImplementation)->setStreamingDataFormat(timestamp, value);
}

template class Streaming<std::int32_t>;
template class Streaming<double>;
template class Streaming<std::vector<std::int8_t> >;
template class Streaming<std::vector<std::uint8_t> >;
template class Streaming<std::vector<std::int32_t> >;
template class Streaming<std::vector<double> >;

}
