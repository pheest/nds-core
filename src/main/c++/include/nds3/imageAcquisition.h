/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSIMAGEACQUISITION_H
#define NDSIMAGEACQUISITION_H

/**
 * @file imageAcquisition.h
 * @brief Defines the nds::imageAcquisition node, which provides basic services for image
 *        acquisition.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

/**
 * This is a node that supplies a data acquisition PV and few control
 * PV that specifies how the acquisition should be performed.
 *
 * It also provides a state machine that allows to start/stop the acquisition.
 *
 * The user of a imageAcquisition class must declare few delegate functions that
 *  specify the actions to perform when the acquisition node's state changes.
 *
 * In particular, the transition from the state off to running should launch
 *  the data acquisition thread which pushes the acquired data via pushData(),
 *  while the transition from running to on should stop the data acquisition thread.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::int32_t
 *            - std::double
 *            - std::vector<std::int8_t>
 *            - std::vector<std::uint8_t>
 *            - std::vector<std::int32_t>
 *            - std::vector<double>
 *            - std::string
 *
 */
template <typename T>
class NDS3_API imageAcquisition: public Node
{
public:
    /**
     * @brief Initializes an empty image acquisition node.
     *
     * You must assign a valid imageAcquisition node before calling initialize().
     */
    imageAcquisition();

    /**
     * @brief Copies a data acquisition reference from another object.
     *
     * @param right a data acquisition holder from which the reference to
     *        the acquisition object implementation is copied
     */
    imageAcquisition(const imageAcquisition<T>& right);

    imageAcquisition& operator=(const imageAcquisition<T>& right);

    /**
     * @brief Constructs the data acquisition node.
     *
     */
    imageAcquisition(const std::string& name,               ///< The node's name
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
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the acquisition starts.
     *
     * @param timestampDelegate the function that returns the exact starting time of the
     *                           data acquisition
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup
     * @brief Push acquired data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data acquisition thread in order to push the acquired data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);


    /**
     * @ingroup
     * @brief Returns the timestamp at the moment of the start of the acquisition.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp() const;
};

}
#endif // NDSIMAGEACQUISITION_H

