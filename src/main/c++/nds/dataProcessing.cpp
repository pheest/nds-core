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
								   allowChange_t allowStateChangeFunction,
		                           writerInt32_t PV_EnableFilter_Writer,
		                           readerInt32_t PV_EnableFilter_Reader,
		                           writerInt32_t PV_FilterType_Writer,
		                           readerInt32_t PV_FilterType_Reader,
		                           writerVectorInt32_t PV_FilterParams_Writer,
		                           readerVectorInt32_t PV_FilterParams_Reader,
		                           size_t 		maxFFTElements,
		                           writerInt32_t PV_EnableFFT_Writer,
		                           readerInt32_t PV_EnableFFT_Reader,
		                           writerInt32_t PV_EnableSwFFT_Writer,
		                           readerInt32_t PV_EnableSwFFT_Reader,
		                           writerInt32_t PV_FFTwindowType_Writer,
		                           readerInt32_t PV_FFTwindowType_Reader,
		                           writerInt32_t PV_FFTOverlap_Writer,
		                           readerInt32_t PV_FFTOverlap_Reader,
		                           writerInt32_t PV_FFTFrameSize_Writer,
		                           readerInt32_t PV_FFTFrameSize_Reader,
		                           writerInt32_t PV_FFTSmooth_Writer,
		                           readerInt32_t PV_FFTSmooth_Reader,
		                           writerInt32_t PV_EnableDecimation_Writer,
		                           readerInt32_t PV_EnableDecimation_Reader,
		                           writerInt32_t PV_DecimationType_Writer,
		                           readerInt32_t PV_DecimationType_Reader,
		                           writerInt32_t PV_DecimationOffset_Writer,
		                           readerInt32_t PV_DecimationOffset_Reader,
		                           writerInt32_t PV_RAW2Eng_Writer,
		                           readerInt32_t PV_RAW2Eng_Reader):
    Node(std::shared_ptr<DataProcessingImpl<T> >(new DataProcessingImpl<T>( name,
																			maxElements,
																		    switchOnFunction,
																		    switchOffFunction,
																		    startFunction,
																		    stopFunction,
																		    recoverFunction,
																			allowStateChangeFunction,
																			PV_EnableFilter_Writer,
																			PV_EnableFilter_Reader,
																			PV_FilterType_Writer,
																			PV_FilterType_Reader,
																			PV_FilterParams_Writer,
																			PV_FilterParams_Reader,
																			maxFFTElements,
																			PV_EnableFFT_Writer,
																			PV_EnableFFT_Reader,
																			PV_EnableSwFFT_Writer,
																			PV_EnableSwFFT_Reader,
																			PV_FFTwindowType_Writer,
																			PV_FFTwindowType_Reader,
																			PV_FFTOverlap_Writer,
																			PV_FFTOverlap_Reader,
																			PV_FFTFrameSize_Writer,
																			PV_FFTFrameSize_Reader,
																			PV_FFTSmooth_Writer,
																			PV_FFTSmooth_Reader,
																			PV_EnableDecimation_Writer,
																			PV_EnableDecimation_Reader,
																			PV_DecimationType_Writer,
																			PV_DecimationType_Reader,
																			PV_DecimationOffset_Writer,
																			PV_DecimationOffset_Reader,
																			PV_RAW2Eng_Writer,
																			PV_RAW2Eng_Reader)))
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
