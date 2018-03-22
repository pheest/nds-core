/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESUPP_H
#define NDSTIMESUPP_H

/**
 * @file timing.h
 * @brief TBD DESCRIBIR ALGO AQUI!!!! 
 *
 *
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
     * @brief Constructs the timing support device node.
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
     * Usually your device implementation will call this function from the
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
    * @brief Retrieve the Clock frequency
    *
    * @return the  Clock frequency value
    */
   double getClkFrequency();
   /**
    * @brief Retrieve the Clock multiplier
    *
    * @return the  Clock multiplier value
    */
   std::int32_t getClkMultiplier();
   /**
    * @brief Retrieve Synchronization status: NOT_SYNCED(0), 
    * SYNCING(1), SYNCED(2), LOST_SYNC(3)
    *
    * @return Synchronization status
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
    *         push is called)
    *
    */
   void setTime(const timespec& timestamp, const timespec& value);
   /** 
    * @brief  sets the human readable time (UTC format) 
    *
    */
   void setHumanTime(const timespec& timestamp, const std::string& value);

   /**
    * @brief Sets the value of the Clock frequency
    *
    */
   void setClkFrequency(const timespec& timestamp, const double& value);
   /**
    * @brief Sets the value of the Clock multiplier
    *
    */
   void setClkMultiplier(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of the Clock frequency
    *
    */
   void setSyncStatus(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of the seconds since last Sync
    *
    */
   void setSecsLastSync(const timespec& timestamp, const std::int32_t& value);
   /**
    * @brief Sets the value of the Reference base time
    *
    */
   void setRefTimeBase(const timespec& timestamp, const timespec& value);

};

}  

#endif // NDSTIMING_H
