/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMING_H
#define NDSTIMING_H

/**
 * @file timing.h
 * @brief Defines the nds::timing node.
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

class NDS3_API Timing: public Node  {
  public:
    // ---------------- Constructors and assignment operator -------------------//
    /**
     * @brief Initializes an empty node.
     *
     * You must assign a valid node before calling initialize().
     */
    Timing();

    /**
     * @brief Copy constructor: copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
    Timing(const Timing& right);


    /**
     * @brief overloading of assignment operator
     * */
    Timing& operator=(const Timing& right);

    /**
     * @ingroup
     * @brief Constructs the Timing node
     *
     * @param name node name
     * @param maxElements not used in this node
     * @param switchOnFunction  Delegate function, performs the actions to switch the node on
     * @param switchOffFunction Delegate function, performs the actions to switch the node off
     * @param startFunction     Delegate function, performs the actions to start the timestamping
     * @param stopFunction      Delegate function, performs the actions to stop the timestamping
     * @param recoverFunction   Delegate function to execute to recover from an error state
     * @param allowStateChangeFunction  Delegate function that can deny a state change.
     *                                  Usually just returns true
     * @param PV_Time_Reader Delegate function, reads  Time PV
     *
     */
    Timing( const std::string& name,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        readerTime_t PV_Time_Reader);

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
     * Usually your device implementation will call th`is function from the
     *  data thread in order to push the data.
     *
     * @param timestamp the timestamp for the data
     * @param data      the data to push to the control system
     */
    void push(const timespec& timestamp, const timespec& data);

    /**
     * @ingroup
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

   // ----------------------------- Getters -------------------------------- //
   /**
    * @brief Retrieve the UNIX time
    *
    * @return UNIX time
    */
   timespec getTime();
   /**
    * @brief Retrieve human readable time (UTC)
    *
    * @return human readable time (UTC)
    */
   std::string getHumanTime();
   /**
    * @brief Retrieve the clock frequency
    *
    * @return the  clock frequency value
    */
   double getClkFrequency();
   /**
    * @brief Retrieve the clock multiplier
    *
    * @return clock multiplier value
    */
   std::int32_t getClkMultiplier();
   /**
    * @brief Retrieve synchronization status: NOT_SYNCED(0),
    *   SYNCING(1), SYNCED(2), LOST_SYNC(3)
    *
    * @return synchronization status
    *
    */
   std::int32_t getSyncStatus();
   /**
    * @brief Retrieve the seconds since last Sync
    *
    * @return the seconds since last Sync
    */
   std::int32_t getSecsLastSync();
   /**
    * @brief Retrieve the Reference base time
    *
    * @return the Reference base time value
    */
   timespec getRefTimeBase();

   // ----------------------------- Setters -------------------------------- //
   /**
    * @brief  sets the UNIX Time (PV is Delegate,
    *         push is called) It updates both Time and Htime
    * @param  timestamp timestamp
    * @param  value Time
    */
   void setTime(const timespec& timestamp, const timespec& value);
   /**
    * @brief Sets the value of the clock frequency
    *
    * @param  timestamp timestamp
    * @param  value clock frequency
    */
   void setClkFrequency(const timespec& timestamp, const double& value);
   /**
    * @brief Sets the value of the clock multiplier
    *
    * @param  timestamp timestamp
    * @param  value clock multiplier
    */
   void setClkMultiplier(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of synchronization status: NOT_SYNCED(0),
    *   SYNCING(1), SYNCED(2), LOST_SYNCED(3)
    *
    * @param  timestamp timestamp
    * @param  value synchronization status
    */
   void setSyncStatus(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of seconds since last sync
    *
    * @param  timestamp timestamp
    * @param  value seconds since last sync
    */
   void setSecsLastSync(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of the Reference base time
    *
    * @param  timestamp timestamp
    * @param  value reference base time
    */
   void setRefTimeBase(const timespec& timestamp, const timespec& value);

};

}

#endif // NDSTIMING_H
