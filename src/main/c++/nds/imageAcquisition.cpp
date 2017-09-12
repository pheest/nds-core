/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/imageAcquisition.h"
#include "nds3/impl/imageAcquisitionImpl.h"

namespace nds
{

template <typename T>
imageAcquisition<T>::imageAcquisition(): Node()
{
}

/**
 * @brief Constructs the image acquisition node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
imageAcquisition<T>::imageAcquisition(  const std::string& name,
										size_t maxElements,
										stateChange_t  switchOnFunction,
										stateChange_t  switchOffFunction,
										stateChange_t  startFunction,
										stateChange_t  stopFunction,
										stateChange_t  recoverFunction,
										allowChange_t  allowStateChangeFunction,
										readerInt32_t  PV_MaxSizeX_Reader,
										readerInt32_t  PV_MaxSizeY_Reader,
										writerInt32_t  PV_BinX_Writer,
										readerInt32_t  PV_BinX_Reader,
										writerInt32_t  PV_BinY_Writer,
										readerInt32_t  PV_BinY_Reader,
										writerInt32_t  PV_MinX_Writer,
										readerInt32_t  PV_MinX_Reader,
										writerInt32_t  PV_MinY_Writer,
										readerInt32_t  PV_MinY_Reader,
										writerInt32_t  PV_SizeX_Writer,
										readerInt32_t  PV_SizeX_Reader,
										writerInt32_t  PV_SizeY_Writer,
										readerInt32_t  PV_SizeY_Reader,
										writerInt32_t  PV_ReverseX_Writer,
										readerInt32_t  PV_ReverseX_Reader,
										writerInt32_t  PV_ReverseY_Writer,
										readerInt32_t  PV_ReverseY_Reader,
										writerInt32_t  PV_Resolution_Writer,
										readerInt32_t  PV_Resolution_Reader,
										writerInt32_t  PV_SamplesPerPixel_Writer,
										readerInt32_t  PV_SamplesPerPixel_Reader,
										writerDouble_t PV_AcquireTime_Writer,
										readerDouble_t PV_AcquireTime_Reader,
										writerDouble_t PV_AcquirePeriod_Writer,
										readerDouble_t PV_AcquirePeriod_Reader,
										readerDouble_t PV_TimeRemaining_Reader,
										writerDouble_t PV_Gain_Writer,
										readerDouble_t PV_Gain_Reader,
										writerInt32_t  PV_FrameType_Writer,
										readerInt32_t  PV_FrameType_Reader,
										writerInt32_t  PV_LostFrames_Writer,
										readerInt32_t  PV_LostFrames_Reader,
										writerInt32_t  PV_ImageMode_Writer,
										readerInt32_t  PV_ImageMode_Reader,
										writerInt32_t  PV_TriggerMode_Writer,
										readerInt32_t  PV_TriggerMode_Reader,
										writerInt32_t  PV_NumExposures_Writer,
										readerInt32_t  PV_NumExposures_Reader,
										readerInt32_t  PV_NumExposuresCounter_Reader,
										writerInt32_t  PV_Exposure_Writer,
										readerInt32_t  PV_Exposure_Reader,
										writerInt32_t  PV_minExposure_Writer,
										readerInt32_t  PV_minExposure_Reader,
										writerInt32_t  PV_maxExposure_Writer,
										readerInt32_t  PV_maxExposure_Reader,
										writerInt32_t  PV_ExposureStep_Writer,
										readerInt32_t  PV_ExposureStep_Reader,
										writerInt32_t  PV_BlackLevel_Writer,
										readerInt32_t  PV_BlackLevel_Reader,
										writerInt32_t  PV_NumImages_Writer,
										readerInt32_t  PV_NumImages_Reader,
										readerInt32_t  PV_NumImagesCounter_Reader,
										writerInt32_t  PV_Acquire_Writer,
										readerInt32_t  PV_Acquire_Reader,
										readerInt32_t  PV_DetectorState_Reader,
										readerString_t PV_StatusMessage_Reader,
										readerString_t PV_StringToServer_Reader,
										readerString_t PV_StringFromServer_Reader,
										writerInt32_t  PV_ReadStatus_Writer,
										writerInt32_t  PV_ShutterMode_Writer,
										readerInt32_t  PV_ShutterMode_Reader,
										writerInt32_t  PV_ShutterControlMode_Writer,
										readerInt32_t  PV_ShutterControlMode_Reader,
										readerInt32_t  PV_ShutterStatus_Reader,
										writerInt32_t  PV_DelayStep_Writer,
										readerInt32_t  PV_DelayStep_Reader,
										writerDouble_t PV_ShutterOpenDelay_Writer,
										readerDouble_t PV_ShutterOpenDelay_Reader,
										writerDouble_t PV_ShutterMinOpenDelay_Writer,
										readerDouble_t PV_ShutterMinOpenDelay_Reader,
										writerDouble_t PV_ShutterMaxOpenDelay_Writer,
										readerDouble_t PV_ShutterMaxOpenDelay_Reader,
										writerDouble_t PV_ShutterCloseDelay_Writer,
										readerDouble_t PV_ShutterCloseDelay_Reader,
										writerDouble_t PV_ShutterMinCloseDelay_Writer,
										readerDouble_t PV_ShutterMinCloseDelay_Reader,
										writerDouble_t PV_ShutterMaxCloseDelay_Writer,
										readerDouble_t PV_ShutterMaxCloseDelay_Reader,
										writerVectorInt32_t PV_HotPixels_Writer,
										readerVectorInt32_t PV_HotPixels_Reader,
										writerVectorInt32_t PV_HotPixelsCorr_Writer,
										readerVectorInt32_t PV_HotPixelsCorr_Reader,
										writerDouble_t PV_Temperature_Writer,
										readerDouble_t PV_Temperature_Reader,
										readerDouble_t PV_ActualTemperature_Reader):
    Node(std::shared_ptr<imageAcquisitionImpl<T> >(new imageAcquisitionImpl<T>( name,
																				maxElements,
																				switchOnFunction,
																				switchOffFunction,
																				startFunction,
																				stopFunction,
																				recoverFunction,
																				allowStateChangeFunction,
																				PV_MaxSizeX_Reader,
																				PV_MaxSizeY_Reader,
																				PV_BinX_Writer,
																				PV_BinX_Reader,
																				PV_BinY_Writer,
																				PV_BinY_Reader,
																				PV_MinX_Writer,
																				PV_MinX_Reader,
																				PV_MinY_Writer,
																				PV_MinY_Reader,
																				PV_SizeX_Writer,
																				PV_SizeX_Reader,
																				PV_SizeY_Writer,
																				PV_SizeY_Reader,
																				PV_ReverseX_Writer,
																				PV_ReverseX_Reader,
																				PV_ReverseY_Writer,
																				PV_ReverseY_Reader,
																				PV_Resolution_Writer,
																				PV_Resolution_Reader,
																				PV_SamplesPerPixel_Writer,
																				PV_SamplesPerPixel_Reader,
																				PV_AcquireTime_Writer,
																				PV_AcquireTime_Reader,
																				PV_AcquirePeriod_Writer,
																				PV_AcquirePeriod_Reader,
																				PV_TimeRemaining_Reader,
																				PV_Gain_Writer,
																				PV_Gain_Reader,
																				PV_FrameType_Writer,
																				PV_FrameType_Reader,
																				PV_LostFrames_Writer,
																				PV_LostFrames_Reader,
																				PV_ImageMode_Writer,
																				PV_ImageMode_Reader,
																				PV_TriggerMode_Writer,
																				PV_TriggerMode_Reader,
																				PV_NumExposures_Writer,
																				PV_NumExposures_Reader,
																				PV_NumExposuresCounter_Reader,
																				PV_Exposure_Writer,
																				PV_Exposure_Reader,
																				PV_minExposure_Writer,
																				PV_minExposure_Reader,
																				PV_maxExposure_Writer,
																				PV_maxExposure_Reader,
																				PV_ExposureStep_Writer,
																				PV_ExposureStep_Reader,
																				PV_BlackLevel_Writer,
																				PV_BlackLevel_Reader,
																				PV_NumImages_Writer,
																				PV_NumImages_Reader,
																				PV_NumImagesCounter_Reader,
																				PV_Acquire_Writer,
																				PV_Acquire_Reader,
																				PV_DetectorState_Reader,
																				PV_StatusMessage_Reader,
																				PV_StringToServer_Reader,
																				PV_StringFromServer_Reader,
																				PV_ReadStatus_Writer,
																				PV_ShutterMode_Writer,
																				PV_ShutterMode_Reader,
																				PV_ShutterControlMode_Writer,
																				PV_ShutterControlMode_Reader,
																				PV_ShutterStatus_Reader,
																				PV_DelayStep_Writer,
																				PV_DelayStep_Reader,
																				PV_ShutterOpenDelay_Writer,
																				PV_ShutterOpenDelay_Reader,
																				PV_ShutterMinOpenDelay_Writer,
																				PV_ShutterMinOpenDelay_Reader,
																				PV_ShutterMaxOpenDelay_Writer,
																				PV_ShutterMaxOpenDelay_Reader,
																				PV_ShutterCloseDelay_Writer,
																				PV_ShutterCloseDelay_Reader,
																				PV_ShutterMinCloseDelay_Writer,
																				PV_ShutterMinCloseDelay_Reader,
																				PV_ShutterMaxCloseDelay_Writer,
																				PV_ShutterMaxCloseDelay_Reader,
																				PV_HotPixels_Writer,
																				PV_HotPixels_Reader,
																				PV_HotPixelsCorr_Writer,
																				PV_HotPixelsCorr_Reader,
																				PV_Temperature_Writer,
																				PV_Temperature_Reader,
																				PV_ActualTemperature_Reader)))
{
}

template <typename T>
imageAcquisition<T>::imageAcquisition(const imageAcquisition<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
imageAcquisition<T>& imageAcquisition<T>::operator=(const imageAcquisition<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void imageAcquisition<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<imageAcquisitionImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void imageAcquisition<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<imageAcquisitionImpl<T> >(m_pImplementation)->push(timestamp, data);
}
//
//template <typename T>
//double DataAcquisition<T>::getFrequencyHz()
//{
//    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getFrequencyHz();
//}
//
//template <typename T>
//double DataAcquisition<T>::getDurationSeconds()
//{
//    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDurationSeconds();
//}
//
//template <typename T>
//double DataAcquisition<T>::getAmplitude()
//{
//    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)-> getAmplitude();
//}
//
//template <typename T>
//double DataAcquisition<T>::getOffset()
//{
//    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)-> getOffset();
//}
//
//template <typename T>
//size_t DataAcquisition<T>::getMaxElements()
//{
//    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getMaxElements();
//}
//
//template <typename T>
//size_t DataAcquisition<T>::getDecimation()
//{
//    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)->getDecimation();
//}
//
//template <typename T>
//size_t DataAcquisition<T>::getSamplingMode()
//{
//    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)-> getSamplingMode();
//}

//template <typename T>
//size_t DataAcquisition<T>::getGround()
//{
//    return std::static_pointer_cast<DataAcquisitionImpl<T> >(m_pImplementation)-> getGround();
//}
template<typename T>
void imageAcquisitionImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}
template <typename T>
timespec imageAcquisition<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<imageAcquisitionImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template class imageAcquisition<std::int32_t>;
template class imageAcquisition<double>;
template class imageAcquisition<std::vector<std::int8_t> >;
template class imageAcquisition<std::vector<std::uint8_t> >;
template class imageAcquisition<std::vector<std::int32_t> >;
template class imageAcquisition<std::vector<double> >;
template class imageAcquisition<std::string >;


}
