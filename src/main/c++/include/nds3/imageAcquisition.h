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
    imageAcquisition(const std::string& name,                           ///< The node's name
					 size_t maxElements,                                ///< Maximum size of the acquired array. Set to 1 for scalar values
					 stateChange_t switchOnFunction,                    ///< Delegate function that performs the actions to switch the node on
					 stateChange_t switchOffFunction,                   ///< Delegate function that performs the actions to switch the node off
					 stateChange_t startFunction,                       ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
					 stateChange_t stopFunction,                        ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
					 stateChange_t recoverFunction,                     ///< Delegate function to execute to recover from an error state
					 allowChange_t allowStateChangeFunction,            ///< Delegate function that can deny a state change. Usually just returns true
					 readerInt32_t PV_MaxSizeX_Reader,                  ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_MaxSizeY_Reader,                  ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_BinX_Writer,                      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_BinX_Reader,                      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_BinY_Writer,                      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_BinY_Reader,                      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_MinX_Writer,                      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_MinX_Reader,                      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_MinY_Writer,                      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_MinY_Reader,                      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_SizeX_Writer,                     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_SizeX_Reader,                     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_SizeY_Writer,                     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_SizeY_Reader,                     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_ReverseX_Writer,                  ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_ReverseX_Reader,                  ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_ReverseY_Writer,                  ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_ReverseY_Reader,                  ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_Resolution_Writer,                ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_Resolution_Reader,                ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_SamplesPerPixel_Writer,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_SamplesPerPixel_Reader,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_AcquireTime_Writer,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_AcquireTime_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_AcquirePeriod_Writer,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_AcquirePeriod_Reader,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_TimeRemaining_Reader,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_Gain_Writer,                     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_Gain_Reader,                     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_FrameType_Writer,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_FrameType_Reader,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_LostFrames_Writer,                ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_LostFrames_Reader,                ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_ImageMode_Writer,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_ImageMode_Reader,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_TriggerMode_Writer,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_TriggerMode_Reader,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_NumExposures_Writer,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_NumExposures_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_NumExposuresCounter_Reader,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_Exposure_Writer,                  ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_Exposure_Reader,                  ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_minExposure_Writer,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_minExposure_Reader,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_maxExposure_Writer,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_maxExposure_Reader,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_ExposureStep_Writer,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_ExposureStep_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_BlackLevel_Writer,                ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_BlackLevel_Reader,                ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_NumImages_Writer,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_NumImages_Reader,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_NumImagesCounter_Reader,          ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_Acquire_Writer,                   ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_Acquire_Reader,                   ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_DetectorState_Reader,             ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerString_t PV_StatusMessage_Reader,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerString_t PV_StringToServer_Reader,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerString_t PV_StringFromServer_Reader,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_ReadStatus_Writer,                ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_ShutterMode_Writer,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_ShutterMode_Reader,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_ShutterControlMode_Writer,        ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_ShutterControlMode_Reader,        ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_ShutterStatus_Reader,             ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerInt32_t PV_DelayStep_Writer,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerInt32_t PV_DelayStep_Reader,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_ShutterOpenDelay_Writer,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_ShutterOpenDelay_Reader,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_ShutterMinOpenDelay_Writer,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_ShutterMinOpenDelay_Reader,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_ShutterMaxOpenDelay_Writer,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_ShutterMaxOpenDelay_Reader,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_ShutterCloseDelay_Writer,        ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_ShutterCloseDelay_Reader,        ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_ShutterMinCloseDelay_Writer,     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_ShutterMinCloseDelay_Reader,     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_ShutterMaxCloseDelay_Writer,     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_ShutterMaxCloseDelay_Reader,     ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerVectorInt32_t PV_HotPixels_Writer,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerVectorInt32_t PV_HotPixels_Reader,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerVectorInt32_t PV_HotPixelsCorr_Writer,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerVectorInt32_t PV_HotPixelsCorr_Reader,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					 writerDouble_t PV_Temperature_Writer,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_Temperature_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					 readerDouble_t PV_ActualTemperature_Reader);       ///< Delegate function setter/getter to interact to the Low Level Driver API

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

