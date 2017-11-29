/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSDATAACQUISITION_H
#define NDSDATAACQUISITION_H

/**
 * @file dataAcquisition.h
 * @brief Defines the nds::DataAcquisition node, which provides basic services for data
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
 * The user of a DataAcquisition class must declare few delegate functions that
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
class NDS3_API DataAcquisition: public Node
{
public:
    /**
     * @brief Initializes an empty data acquisition node.
     *
     * You must assign a valid DataAcquisition node before calling initialize().
     */
    DataAcquisition();

    /**
     * @brief Copies a data acquisition reference from another object.
     *
     * @param right a data acquisition holder from which the reference to
     *        the acquisition object implementation is copied
     */
    DataAcquisition(const DataAcquisition<T>& right);

    DataAcquisition& operator=(const DataAcquisition<T>& right);

    /**
     * @brief Constructs the data acquisition node.
     *
     */
    DataAcquisition(const std::string& name,                ///< The node's name
                    size_t maxElements,                     ///< Maximum size of the acquired array. Set to 1 for scalar values
                    stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on
                    stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off
                    stateChange_t startFunction,            ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
                    stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
                    stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state
                    allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true
					writerDouble_t PV_Gain_Writer,          ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Offset_Writer,        ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Bandwidth_Writer,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Resolution_Writer,    ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_Impedance_Writer,     ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_Coupling_Writer,       ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SignalRefType_Writer,      ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_Ground_Writer,        ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_DMAEnable_Writer);         ///< Delegate function setter/getter to interact to the Low Level Driver API

    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the data acquisition starts.
     *
     * @param timestampDelegate the function that returns the exact starting time of the
     *                           data acquisition
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup datareadwrite
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
     * @brief Retrieve the Gain
     *
     * @return the Gain value
     */
    size_t getGain();

    /**
     * @brief Retrieve the Offset
     *
     * @return the Offset value
     */
    size_t getOffset();

    /**
     * @brief Retrieve the Bandwidth
     *
     * @return the Bandwidth value
     */
    size_t getBandwidth();

    /**
     * @brief Retrieve the Resolution
     *
     * @return the Resolution value
     */
    size_t getResolution();

    /**
     * @brief Retrieve the Impedance
     *
     * @return the Impedance value
     */
    size_t getImpedance();

    /**
     * @brief Retrieve the Coupling
     *
     * @return the Coupling value
     */
    size_t getCoupling();

    /**
     * @brief Retrieve the SignalRef
     *
     * @return the SignalRef value
     */
    size_t getSignalRefType();

    /**
     * @brief Retrieve the Ground
     *
     * @return the Ground value
     */
    size_t getGround();
    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the DataAcquisition constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();
    /**
     * @ingroup timing
     * @brief Returns the timestamp at the moment of the start of the acquisition.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp() const;
    /**
     * @brief Retrieve the Number Of Pushed Data Blocks by the acquisition thread to the control system
     *
     * @return the Number Of Pushed Data Blocks
     */
    size_t getNumberOfPushedDataBlocks();
    /**
     * @brief Retrieve the DMA Buffer size value
     *
     * @return the m_DMABufferSize_PV value
     */
    size_t getDMABufferSize();
    /**
     * @brief Retrieve the DMAEnable status
     *
     * @return the m_DMAEnable_PV value
     */
    size_t getDMAEnable();
     /**
     * @brief Retrieve the Number of DMA channels used by the DAQ node
     *
     * @return the m_DMANumChannels_PV value
     */
    size_t getDMANumChannels();
    /**
    * @brief Retrieve the DMA Frame Type
    *
    * @return the m_DMAFrameType_PV value
    */
    size_t getDMAFrameType();
    /**
    * @brief Retrieve the DMA sample size
    *
    * @return the m_DMASampleSize_PV value
    */
    size_t getDMASampleSize();
    /**
    * @brief Retrieve the DMA sampling rate
    *
    * @return the m_DMASamplingRate_PV value
    */
    size_t getDMASamplingRate();


    /**
     * @brief Sets the value of the m_Gain_RBV.
     *
     */
    void setGain(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Offset_RBV.
     *
     */
    void setOffset(const timespec& timestamp, const double& value);

    /**
     * @brief Sets the value of the m_Bandwidth_RBV.
     *
     */
    void setBandwidth(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Resolution_RBV.
     *
     */
    void setResolution(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_Impedance_RBV.
     *
     */
    void setImpedance(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_Coupling_RBV.
     *
     */
    void setCoupling(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_SignalRef_RBV.
     *
     */
    void setSignalRefType(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_Ground_RBV.
     *
     */
    void setGround(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_NumberOfPushedDataBocks.
     *
     */
    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMABufferSize_PV.
     *
     */
    void setDMABufferSize(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_DMAEnable_PV.
     *
     */
    void setDMAEnable(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMANumChannels_PV.
     *
     */
    void setDMANumChannels(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMAFrameType_PV.
     *
     */
    void setDMAFrameType(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMASampleSize_PV.
     *
     */
    void setDMASampleSize(const timespec& timestamp, const std::int32_t& value);
    /**
     * @brief Sets the value of the m_DMASamplingRate_PV.
     *
     */
    void setDMASamplingRate(const timespec& timestamp, const std::int32_t& value);
};

}
#endif // NDSDATAACQUISITION_H

