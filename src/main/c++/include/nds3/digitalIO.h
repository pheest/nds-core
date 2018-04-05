/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSDIGITALIO_H
#define NDSDIGITALIO_H

/**
 * @file digitalIO.h
 * @brief Defines the nds::DigitalIO node, which provides basic services for Digital
 *        Input/Output.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

/**
 * This is a node that supplies PVs that specifies how the IO acquisition
 * generation should be performed.
 *
 * It also provides a state machine for these kind of channels
 *
 * The user of a DigitalIO class must declare few delegate functions that
 *  specify the actions to perform when the node's state changes.
 *
 * In particular, the transition from the state off to running should launch
 *  the DigitalIO thread which pushes the data via push(), while the transition
 *  from running to on should stop the DigitalIO thread.
 *
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::int32_t
 *            - std::double
 *            - std::vector<bool>
 *            - std::vector<std::int8_t>
 *            - std::vector<std::int16_t>
 *            - std::vector<std::int32_t>
 *
 */
template <typename T>
class NDS3_API DigitalIO: public Node
{
public:
    /**
     * @brief Initializes an empty data acquisition node.
     *
     * You must assign a valid DigitalIO node before calling initialize().
     */
    DigitalIO();

    /**
     * @brief Copies a data reference from another object.
     *
     * @param right a digitalIO holder from which the reference to
     *        the digitalIO object implementation is copied
     */
    DigitalIO(const DigitalIO<T>& right);

    DigitalIO& operator=(const DigitalIO<T>& right);

    /**
     * @brief Constructs the Digital IO node.
     *
     */
    DigitalIO( const std::string& name,                ///< The node's name
               size_t maxElements,                     ///< Maximum size of the acquired array. Set to 1 for scalar values
               stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on
               stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off
               stateChange_t startFunction,            ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
               stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
               stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state
	           allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true
			   writerVectorBool_t PV_dataOutMask_Writer,///< Delegate function setter/getter to interact to the Low Level Driver API
			   writerDouble_t PV_voltLevelHigh_Writer,  ///< Delegate function setter/getter to interact to the Low Level Driver API
			   writerDouble_t PV_voltLevelLow_Writer,   ///< Delegate function setter/getter to interact to the Low Level Driver API
			   writerVectorBool_t PV_ChannelDir_Writer);    ///< Delegate function setter/getter to interact to the Low Level Driver API



    /**
     * @ingroup timing
     * @brief Set the function that retrieves the exact start time when the digitalIO node starts.
     *
     * @param timestampDelegate the function that returns the exact starting time of the
     *                           digitalIO node
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Push the data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  DigitalIO thread in order to push the data.
     *
     * @param timestamp timestamp for the data
     * @param data      data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the DigitalIO constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();
    /**
     * @brief Retrieve the dataOutMask
     *
     * @return the dataOutMask value
     */
    std::vector<bool> getDataOutMask();
    /**
     * @brief Retrieve the voltLevelHigh
     *
     * @return the voltLevelHigh value
     */
    double getVoltLevelHigh();
    /**
     * @brief Retrieve the voltLevelLow.
     *
     * @return the voltLevelLow value
     */
    double getVoltLevelLow();
    /**
     * @brief Retrieve the ChannelDir.
     *
     * @return the ChannelDir value
     */
    std::vector<bool> getChannelDir();
    /**
     * @brief Sets the value of the m_NumberOfPushedDataBocks.
     *
     * @param timestamp timestamp for the value
     * @param value Number of pushed data blocks during the data acquisition
     */
    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    /**
	 * @brief Sets the value of the m_dataOutMask_RBV.
	 *
     * @param timestamp timestamp for the value
     * @param value Data Out Mask value. Each position of the array should be set to true or false.
     */
	void setDataOutMask(const timespec& timestamp, const std::vector<bool>& value);
    /**
     * @brief Sets the value of the m_voltLevelHigh_RBV.
     *
     * @param timestamp timestamp for the value
     * @param value Voltage value for high voltage levels.
     */
    void setVoltLevelHigh(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_voltLevelLow_RBV.
     *
     * @param timestamp timestamp for the value
     * @param value Voltage value for low voltage levels.
     */
    void setVoltLevelLow(const timespec& timestamp, const double& value);
    /**
     * @brief Sets the value of the m_channelDir_RBV.
     *
     * @param timestamp timestamp for the value
     * @param value Channel direction. Each position of the array should be set to true or false.
     */
    void setChannelDir(const timespec& timestamp, const std::vector<bool>& value);
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

};

}
#endif // NDSDIGITALIO_H

