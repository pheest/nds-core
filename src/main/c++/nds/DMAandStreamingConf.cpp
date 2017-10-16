/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/DMAandStreamingConf.h"
#include "nds3/impl/DMAandStreamingConfImpl.h"

namespace nds
{

template <typename T>
DMASupport<T>::DMASupport(): Node()
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
DMASupport<T>::DMASupport( const std::string& name,
						   size_t maxElements,
						   stateChange_t switchOnFunction,
						   stateChange_t switchOffFunction,
						   stateChange_t startFunction,
						   stateChange_t stopFunction,
						   stateChange_t recoverFunction,
						   allowChange_t allowStateChangeFunction,
						   readerDouble_t PV_BufferSize_Reader,
						   writerInt32_t PV_EnableDMA_Writer,
						   readerInt32_t PV_EnableDMA_Reader,
						   readerInt32_t PV_NumDMAChannels_Reader,
						   readerInt32_t PV_DMAFrameType_Reader,
						   readerInt32_t PV_DMASampleSize_Reader,
						   readerInt32_t PV_DMASamplingRate_Reader):
    Node(std::shared_ptr<DMASupportImpl<T> >(new DMASupportImpl<T>( name,
																	maxElements,
																	switchOnFunction,
																	switchOffFunction,
																	startFunction,
																	stopFunction,
																	recoverFunction,
																	allowStateChangeFunction,
																	PV_BufferSize_Reader,
																	PV_EnableDMA_Writer,
																	PV_EnableDMA_Reader,
																	PV_NumDMAChannels_Reader,
																	PV_DMAFrameType_Reader,
																	PV_DMASampleSize_Reader,
																	PV_DMASamplingRate_Reader)))
{
}

template <typename T>
DMASupport<T>::DMASupport(const DMASupport<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
DMASupport<T>& DMASupport<T>::operator=(const DMASupport<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void DMASupport<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<DMASupportImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void DMASupport<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<DMASupportImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t DMASupport<T>::getMaxElements()
{
    return std::static_pointer_cast<DMASupportImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec DMASupport<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<DMASupportImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
StreamingConf<T>::StreamingConf(): Node()
{
}

template class DMASupport<std::int32_t>;
template class DMASupport<double>;
template class DMASupport<std::vector<std::int8_t> >;
template class DMASupport<std::vector<std::uint8_t> >;
template class DMASupport<std::vector<std::int32_t> >;
template class DMASupport<std::vector<double> >;
template class DMASupport<std::string >;


/**
 * @brief Constructs the StreamingConf node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
StreamingConf<T>::StreamingConf( const std::string& name,
							     size_t maxElements,
								 stateChange_t switchOnFunction,
								 stateChange_t switchOffFunction,
								 stateChange_t startFunction,
								 stateChange_t stopFunction,
								 stateChange_t recoverFunction,
								 allowChange_t allowStateChangeFunction,
							     readerInt32_t PV_StreamingDataFormat_Reader,
							     writerInt32_t PV_StreamingType_Writer,
							     readerInt32_t PV_StreamingType_Reader):
    Node(std::shared_ptr<StreamingConfImpl<T> >(new StreamingConfImpl<T>( name,
																		  maxElements,
																		  switchOnFunction,
																		  switchOffFunction,
																		  startFunction,
																	      stopFunction,
																		  recoverFunction,
																		  allowStateChangeFunction,
																		  PV_StreamingDataFormat_Reader,
																		  PV_StreamingType_Writer,
																		  PV_StreamingType_Reader)))
{
}

template <typename T>
StreamingConf<T>::StreamingConf(const StreamingConf<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
StreamingConf<T>& StreamingConf<T>::operator=(const StreamingConf<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void StreamingConf<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<StreamingConfImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void StreamingConf<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<StreamingConfImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t StreamingConf<T>::getMaxElements()
{
    return std::static_pointer_cast<StreamingConfImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec StreamingConf<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<StreamingConfImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template class StreamingConf<std::int32_t>;
template class StreamingConf<double>;
template class StreamingConf<std::vector<std::int8_t> >;
template class StreamingConf<std::vector<std::uint8_t> >;
template class StreamingConf<std::vector<std::int32_t> >;
template class StreamingConf<std::vector<double> >;
template class StreamingConf<std::string >;

}
