/*
 * Nominal Device Support v.3 (NDS3)
 *
 *
 */

#ifndef NDSDMAANDSTREAMINGCONF_H
#define NDSDMAANDSTREAMINGCONF_H

/**
 * @file DMAandStreamingConf.h
 * @brief TBD
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{


template <typename T>
class NDS3_API StreamingConf: public Node
{
public:
    /**
     * @brief Initializes an empty data generation node.
     *
     * You must assign a valid node before calling initialize().
     */
	StreamingConf();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
	StreamingConf(const StreamingConf<T>& right);

	StreamingConf& operator=(const StreamingConf<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
	StreamingConf( const std::string& name,                       ///< The node's name
				   size_t maxElements,                           ///< Maximum size of the acquired array. Set to 1 for scalar values
                   stateChange_t switchOnFunction,               ///< Delegate function that performs the actions to switch the node on
                   stateChange_t switchOffFunction,              ///< Delegate function that performs the actions to switch the node off
                   stateChange_t startFunction,                  ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
                   stateChange_t stopFunction,                   ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
                   stateChange_t recoverFunction,                ///< Delegate function to execute to recover from an error state
                   allowChange_t allowStateChangeFunction,       ///< Delegate function that can deny a state change. Usually just returns true
				   readerInt32_t PV_StreamingDataFormat_Reader,  ///< Delegate function setter/getter to interact to the Low Level Driver API
				   writerInt32_t PV_StreamingType_Writer,        ///< Delegate function setter/getter to interact to the Low Level Driver API
				   readerInt32_t PV_StreamingType_Reader);       ///< Delegate function setter/getter to interact to the Low Level Driver API


    /**
     * @ingroup
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param
     *
     */
    //TODO: Discuss if necessary
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Retrieve the maximum number of elements that can be stored in the
     *        pushed array. This number is set in the constructor.
     *
     * @return the maximum number of elements that can be stored in the pushed array
     */
    size_t getMaxElements();

    /**
     * @ingroup
     * @brief Push data to the control system.
     *
     * Usually your device implementation will call this function from the
     *  data thread in order to push the data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const T& data);

    /**
     * @ingroup
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

};


}
#endif // NDSDMAANDSTREAMINGCONF_H

