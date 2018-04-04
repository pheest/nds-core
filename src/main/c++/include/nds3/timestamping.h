/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESTAMPING_H
#define NDSTIMESTAMPING_H

/**
 * @file timestamping.h
 * @brief Defines the nds::timestamping node.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{
template <typename T>
class NDS3_API Timestamping: public Node  {
  public:
    // ---------------- Constructors and assignment operator -------------------//
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
    Timestamping();

    /**
     * @brief Copy constructor: copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
    Timestamping(const Timestamping& right);


    /**
     * @brief overloading of assignment operator
     * */
    Timestamping& operator=(const Timestamping& right);

    /**
     * @brief Constructs the Time stamping node
     *
     * @description it calls the constructor of
     *
     * @param name
     * @param maxElements
     * @param switchOnFunction  Delegate function, performs the actions to switch the node on
     * @param switchOffFunction Delegate function, performs the actions to switch the node off
     * @param startFunction     Delegate function, performs the actions to start the timestamping
     * @param stopFunction      Delegate function, performs the actions to stop the timestamping
     * @param recoverFunction   Delegate function to execute to recover from an error state
     * @param allowStateChangeFunction  Delegate function that can deny a state change.
     *                                  Usually just returns true
     * @param PV_Enable_Writer Delegate function to enable/disable timestamping
     * @param PV_Edge_Writer   Delegate function, performs the actions to set
     *
     */
    Timestamping(const std::string& name,
                 size_t maxElements,
                 stateChange_t switchOnFunction,
                 stateChange_t switchOffFunction,
                 stateChange_t startFunction,
                 stateChange_t stopFunction,
                 stateChange_t recoverFunction,
                 allowChange_t allowStateChangeFunction,
                 writerInt32_t PV_Enable_Writer, //Delegate function to enable/disable timestamping
                 writerInt32_t PV_Edge_Writer,
                 writerInt32_t PV_ClearOverflow_Writer);

    // ------------------ Functions common to all nodes ---------------------//
    /**
     * @ingroup
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param timestampDelegate
     *
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

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
    timespec getStartTimestamp() const;

    // ---------------------------- Getters ---------------------------------- //
    /**
     * @brief Retrieves timestamping status
     *
     * @return timestamping status
     */
    std::int32_t getEnable();

    /**
     * @brief Retreives detection edge
     *
     * @return detection edge
     */
    std::int32_t getEdge();

    /**
     * @brief Retrieves max number of timestamps
     *
     * @return max number of timestamps
     */
    std::int32_t getMaxTimestamps();

    /**
     * @brief Retreives overflow error status
     *
     * @return overflow error status
     */
    std::int32_t getOverflow();

    // --------------------------- Setters ----------------------------------- //
    /**
    * @brief Sets timestamping status
    *
    * @param timestamp timestamp
    * @param value timestamping status value
    */
    void setEnable(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets detection Edge
    *
    * @param timestamp timestamp
    * @param value detection edge
    */
    void setEdge(const timespec& timestamp, const std::int32_t& value);

    /**
     * @brief Retrieves max number of timestamps
     *
     * @param  timestamp timestamp
     * @param  value  max number of timestamps
     */
    void setMaxTimestamps(const timespec& timestamp, const std::int32_t& value);

    /**
    * @brief Sets overflow error status
    *
    * @param timestamp timestamp
    * @param value overflow error status
    */
    void setOverflow(const timespec& timestamp, const std::int32_t& value);

  };
}

#endif // NDSTIMESTAMPING_H

