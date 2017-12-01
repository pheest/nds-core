/*
 * Nominal Device Support v.3 (NDS3)
 *
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSIMAGEACQUISITIONIMPL_H
#define NDSIMAGEACQUISITIONIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class imageAcquisitionImpl: public NodeImpl
{
public:
    imageAcquisitionImpl(const std::string& name,
					     size_t maxElements,
					     stateChange_t switchOnFunction,
					     stateChange_t switchOffFunction,
					     stateChange_t startFunction,
					     stateChange_t stopFunction,
					     stateChange_t recoverFunction,
					     allowChange_t allowStateChangeFunction,
					     readerInt32_t PV_MaxSizeX_Reader,
					     readerInt32_t PV_MaxSizeY_Reader,
					     writerInt32_t PV_BinX_Writer,
					     readerInt32_t PV_BinX_Reader,
					     writerInt32_t PV_BinY_Writer,
					     readerInt32_t PV_BinY_Reader,
					     writerInt32_t PV_MinX_Writer,
					     readerInt32_t PV_MinX_Reader,
					     writerInt32_t PV_MinY_Writer,
					     readerInt32_t PV_MinY_Reader,
					     writerInt32_t PV_SizeX_Writer,
					     readerInt32_t PV_SizeX_Reader,
					     writerInt32_t PV_SizeY_Writer,
					     readerInt32_t PV_SizeY_Reader,
					     writerInt32_t PV_ReverseX_Writer,
					     readerInt32_t PV_ReverseX_Reader,
					     writerInt32_t PV_ReverseY_Writer,
					     readerInt32_t PV_ReverseY_Reader,
					     writerInt32_t PV_Resolution_Writer,
					     readerInt32_t PV_Resolution_Reader,
					     writerInt32_t PV_SamplesPerPixel_Writer,
					     readerInt32_t PV_SamplesPerPixel_Reader,
					     writerDouble_t PV_AcquireTime_Writer,
					     readerDouble_t PV_AcquireTime_Reader,
					     writerDouble_t PV_AcquirePeriod_Writer,
					     readerDouble_t PV_AcquirePeriod_Reader,
					     readerDouble_t PV_TimeRemaining_Reader,
					     writerDouble_t PV_Gain_Writer,
					     readerDouble_t PV_Gain_Reader,
					     writerInt32_t PV_FrameType_Writer,
					     readerInt32_t PV_FrameType_Reader,
					     writerInt32_t PV_LostFrames_Writer,
					     readerInt32_t PV_LostFrames_Reader,
					     writerInt32_t PV_ImageMode_Writer,
					     readerInt32_t PV_ImageMode_Reader,
					     writerInt32_t PV_TriggerMode_Writer,
					     readerInt32_t PV_TriggerMode_Reader,
					     writerInt32_t PV_NumExposures_Writer,
					     readerInt32_t PV_NumExposures_Reader,
					     readerInt32_t PV_NumExposuresCounter_Reader,
					     writerInt32_t PV_Exposure_Writer,
					     readerInt32_t PV_Exposure_Reader,
					     writerInt32_t PV_minExposure_Writer,
					     readerInt32_t PV_minExposure_Reader,
					     writerInt32_t PV_maxExposure_Writer,
					     readerInt32_t PV_maxExposure_Reader,
					     writerInt32_t PV_ExposureStep_Writer,
					     readerInt32_t PV_ExposureStep_Reader,
					     writerInt32_t PV_BlackLevel_Writer,
					     readerInt32_t PV_BlackLevel_Reader,
					     writerInt32_t PV_NumImages_Writer,
					     readerInt32_t PV_NumImages_Reader,
					     readerInt32_t PV_NumImagesCounter_Reader,
					     writerInt32_t PV_Acquire_Writer,
					     readerInt32_t PV_Acquire_Reader,
					     readerInt32_t PV_DetectorState_Reader,
					     readerString_t PV_StatusMessage_Reader,
					     readerString_t PV_StringToServer_Reader,
					     readerString_t PV_StringFromServer_Reader,
					     writerInt32_t PV_ReadStatus_Writer,
					     writerInt32_t PV_ShutterMode_Writer,
					     readerInt32_t PV_ShutterMode_Reader,
					     writerInt32_t PV_ShutterControlMode_Writer,
					     readerInt32_t PV_ShutterControlMode_Reader,
					     readerInt32_t PV_ShutterStatus_Reader,
					     writerInt32_t PV_DelayStep_Writer,
					     readerInt32_t PV_DelayStep_Reader,
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
					     readerDouble_t PV_ActualTemperature_Reader);


    /**
     * @brief Specifies the function to call to get the start timestamp.
     *
     * The function is called only once at each start of the acquisition and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Returns the timestamp at the moment of the start of the acquisition.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();


protected:

    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_onStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_startTimestampFunction;

    /**
     * @brief Acquisition start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs
    std::shared_ptr<PVVariableInImpl<T> > m_image_PV;

    std::shared_ptr<PVVariableInImpl<std::string>> m_imageSourceType_PV;

    std::shared_ptr<PVVariableInImpl<std::string>> m_imageSource_PV;

    std::shared_ptr<StateMachineImpl> m_stateMachine;

    std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_MaxSizeX_RBVPV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_MaxSizeY_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_BinX_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_BinX_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_BinY_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_BinY_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_MinX_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_MinX_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_MinY_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_MinY_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_SizeX_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_SizeX_RBVPV;
	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_SizeY_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_SizeY_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ReverseX_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_ReverseX_RBVPV;
	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ReverseY_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_ReverseY_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_Resolution_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_Resolution_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_SamplesPerPixel_PV;

	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_SamplesPerPixel_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double>> m_AcquireTime_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_AcquireTime_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double>> m_AcquirePeriod_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_AcquirePeriod_RBVPV;

	std::shared_ptr<PVDelegateInImpl<double>> m_Time_Remaining_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double>> m_Gain_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_Gain_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_FrameType_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_FrameType_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_LostFrames_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_LostFrames_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ImageMode_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_ImageMode_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_TriggerMode_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_TriggerMode_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_NumExposures_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_NumExposures_RBVPV;

	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_NumExposuresCounter_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_Exposure_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_Exposure_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_minExposure_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_minExposure_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_maxExposure_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_maxExposure_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ExposureStep_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_ExposureStep_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_BlackLevel_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_BlackLevel_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_NumImages_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_NumImages_RBVPV;

	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_NumImagesCounter_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_Acquire_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_Acquire_RBVPV;

	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_DetectorState_RBVPV;

	std::shared_ptr<PVDelegateInImpl<std::string>> m_StatusMessage_RBVPV;

	std::shared_ptr<PVDelegateInImpl<std::string>> m_StringToServer_RBVPV;

	std::shared_ptr<PVDelegateInImpl<std::string>> m_StringFromServer_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ReadStatus_PV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ShutterMode_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_ShutterMode_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ShutterControlMode_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_ShutterControlMode_RBVPV;

	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_ShutterStatus_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_DelayStep_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t>> m_DelayStep_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<double>> m_ShutterOpenDelay_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_ShutterOpenDelay_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<double>>m_ShutterMinOpenDelay_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_ShutterMinOpenDelay_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<double>>m_ShutterMaxOpenDelay_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_ShutterMaxOpenDelay_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<double>> m_ShutterCloseDelay_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_ShutterCloseDelay_RBVPV;


	std::shared_ptr<PVDelegateOutImpl<double>> m_ShutterMinCloseDelay_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_ShutterMinCloseDelay_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<double>> m_ShutterMaxCloseDelay_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_ShutterMaxCloseDelay_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::vector<std::int32_t>>> m_HotPixels_PV;
	std::shared_ptr<PVDelegateInImpl<std::vector<std::int32_t>>> m_HotPixels_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::vector<std::int32_t>>> m_HotPixelsCorr_PV;
	std::shared_ptr<PVDelegateInImpl<std::vector<std::int32_t>>> m_HotPixelsCorr_RBVPV;


	std::shared_ptr<PVDelegateOutImpl<double>> m_Temperature_PV;
	std::shared_ptr<PVDelegateInImpl<double>> m_Temperature_RBVPV;
	std::shared_ptr<PVDelegateInImpl<double>> m_ActualTemperature_RBVPV;

	std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_decimation_PV;

};

}
#endif // NDSIMAGEACQUISITIONIMPL_H

