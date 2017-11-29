/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/FFT.h"
#include "nds3/impl/FFTImpl.h"

namespace nds
{

template <typename T>
FFT<T>::FFT(): Node()
{
}

/**
 * @brief Constructs the FFT processing node
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
FFT<T>::FFT( const std::string& name,
		size_t maxElements,
		stateChange_t switchOnFunction,
		stateChange_t switchOffFunction,
		stateChange_t startFunction,
		stateChange_t stopFunction,
		stateChange_t recoverFunction,
		allowChange_t allowStateChangeFunction,
		writerInt32_t PV_FFTEnable_Writer,
		writerInt32_t PV_FFTWindowType_Writer,
		writerInt32_t PV_FFTFrameOverlap_Writer,
		writerInt32_t PV_FFTFrameSize_Writer,
		writerInt32_t PV_FFTSmoothFactor_Writer
):
Node(std::shared_ptr<FFTImpl<T> >(new FFTImpl<T>( name,
		maxElements,
		switchOnFunction,
		switchOffFunction,
		startFunction,
		stopFunction,
		recoverFunction,
		allowStateChangeFunction,
		PV_FFTEnable_Writer,
		PV_FFTWindowType_Writer,
		PV_FFTFrameOverlap_Writer,
		PV_FFTFrameSize_Writer,
		PV_FFTSmoothFactor_Writer
)))
{
}

template <typename T>
FFT<T>::FFT(const FFT<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
FFT<T>& FFT<T>::operator=(const FFT<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void FFT<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void FFT<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
size_t FFT<T>::getMaxElements()
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->getMaxElements();
}

template <typename T>
timespec FFT<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
size_t FFT<T>::getFFTEnable()
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->getFFTEnable();
}

template <typename T>
size_t FFT<T>::getFFTWindowType()
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->getFFTWindowType();
}

template <typename T>
size_t FFT<T>::getFFTFrameOverlap()
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->FFTFrameOverlap();
}

template <typename T>
size_t FFT<T>::getFFTFrameSize()
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->getFFTFrameSize();
}

template <typename T>
size_t FFT<T>::getFFTSmoothFactor()
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->getFFTSmoothFactor();
}

template <typename T>
void FFT<T>::setFFTEnable(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->setFFTEnable(timestamp, value);
}

template <typename T>
void FFT<T>::setFFTWindowType(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->setFFTWindowType(timestamp, value);
}

template <typename T>
void FFT<T>::setFFTFrameOverlap(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->setFFTFrameOverlap(timestamp, value);
}

template <typename T>
void FFT<T>::setFFTFrameSize(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->setFFTFrameSize(timestamp, value);
}

template <typename T>
void FFT<T>::setFFTSmoothFactor(const timespec& timestamp, const std::int32_t& value)
{
    return std::static_pointer_cast<FFTImpl<T> >(m_pImplementation)->setFFTSmoothFactor(timestamp, value);
}


template class FFTImpl<std::int32_t>;
template class FFTImpl<double>;
template class FFTImpl<std::vector<std::int8_t> >;
template class FFTImpl<std::vector<std::uint8_t> >;
template class FFTImpl<std::vector<std::int32_t> >;
template class FFTImpl<std::vector<double> >;


}
