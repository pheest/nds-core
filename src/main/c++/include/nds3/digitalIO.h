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
 *  the acquisition generation thread which pushes the acquired data via pushData(),
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
class NDS3_API DigitalIO: public Node
{
public:
    /**
     * @brief Initializes an empty data acquisition node.
     *
     * You must assign a valid DataAcquisition node before calling initialize().
     */
    DigitalIO();

    /**
     * @brief Copies a data reference from another object.
     *
     * @param right a data acquisition holder from which the reference to
     *        the acquisition object implementation is copied
     */
    DigitalIO(const DigitalIO<T>& right);

    DigitalIO& operator=(const DigitalIO<T>& right);

    /**
     * @brief Constructs the data acquisition node.
     *
     */
    DigitalIO(const std::string& name,             		   ///< The node's name
                    size_t maxElements,                    ///< Maximum size of the acquired array. Set to 1 for scalar values
                    stateChange_t switchOnFunction,        ///< Delegate function that performs the actions to switch the node on
                    stateChange_t switchOffFunction,       ///< Delegate function that performs the actions to switch the node off
                    stateChange_t startFunction,           ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
                    stateChange_t stopFunction,            ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
                    stateChange_t recoverFunction,         ///< Delegate function to execute to recover from an error state
	                allowChange_t allowStateChangeFunction,///< Delegate function that can deny a state change. Usually just returns true
					writerInt32_t PV_voltLevelHigh_Writer,  ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_voltLevelHigh_Reader,  ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_voltLevelLow_Writer,   ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_voltLevelLow_Reader,   ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_ChannelDir_Writer,     ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_ChannelDir_Reader);      ///< Delegate function setter/getter to interact to the Low Level Driver API



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

