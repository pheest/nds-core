/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMINGIMPL_H
#define NDSTIMINGIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/impl/pvVariableInImpl.h"

namespace nds {

class TimingImpl: public NodeImpl {
  public: 
    TimingImpl( const std::string& name,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        readerTime_t PV_Time_Reader);

   // Common functions 
   /**
    * @brief Returns the timestamp at start.
    *
    * This value is set when the state switches to running.
    * If a timing plugin is active then the timestamp is taken from the plugin.
    *
    * @return the time when started.
    */
   timespec getStartTimestamp() const;

   /**
    * @brief Specifies the function to call to get the start timestamp.
    *
    * The function is called only once at each start and its result
    * is stored in a local variable that can be retrieved with getStartTimestamp().
    *
    * If this function is not called then getTimestamp() is used to get the start time.
    *
    * @param timestampDelegate the function to call to get the start time
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
    * @brief Called by the state machine. Store the current timestamp and 
    *        then calls the delegated onStart function.
    */
   void onStart();

   // ---------------------------- Getters ---------------------------------- //
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

   // --------------------------- Setters ----------------------------------- //
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

  protected: 
    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_OnStartDelegate;
    /**
     * @brief Delegate function that retrieves the start time.
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Start time. Retrieved via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;

    // PVs

    std::shared_ptr<PVDelegateInImpl<timespec>> m_Time_PV;
    
    std::shared_ptr<PVDelegateInImpl<std::string>> m_HumanTime_PV;

    std::shared_ptr<PVVariableInImpl<double> > m_ClkFrequency_PV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ClkMultiplier_PV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SyncStatus_PV;
  
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SecsLastSync_PV;
    
    std::shared_ptr<PVVariableInImpl<timespec> > m_RefTimeBase_PV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;

   void PV_HTime_Reader(timespec *timestamp, std::string *value);
   //readerString_t PV_HTime_Reader; 

};
}


#endif //  NDSTIMINGIMPL_H
