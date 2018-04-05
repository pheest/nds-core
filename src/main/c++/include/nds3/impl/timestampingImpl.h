/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSTIMESTAMPINGIMPL_H
#define NDSTIMESTAMPINGIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"

namespace nds {

  template <typename T> class PVVariableInImpl;
  template <typename T> class PVDelegateOutImpl;
  template <typename T> class PVVariableOutImpl;

  template<typename T>
  class TimestampingImpl: public NodeImpl {
   public:

     TimestampingImpl(const std::string& name,
                 size_t maxElements,
                 stateChange_t switchOnFunction,
                 stateChange_t switchOffFunction,
                 stateChange_t startFunction,
                 stateChange_t stopFunction,
                 stateChange_t recoverFunction,
                 allowChange_t allowStateChangeFunction,
                 writerInt32_t PV_Enable_Writer,
                 writerInt32_t PV_Edge_Writer,
                 writerInt32_t PV_ClearOverflow_Writer);

    // ----------------------- Common functions ----------------------------- //
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
    void push(const timespec& timestamp, const T& data);

    /**
     * @brief Called by the state machine. Store the current timestamp and
     *        then calls the delegated onStart function.
     */
    void onStart();

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
    std::int32_t  getEdge();

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
     std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_Enable_PV;
     std::shared_ptr<PVVariableInImpl<std::int32_t>> m_Enable_RBVPV;

     std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_Edge_PV;
     std::shared_ptr<PVVariableInImpl<std::int32_t>> m_Edge_RBVPV;

     std::shared_ptr<PVVariableOutImpl<std::int32_t>> m_Decimation_PV;

     std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_ClearOverflow_PV;

     std::shared_ptr<PVVariableInImpl<T>> m_Timestamps_PV;

     std::shared_ptr<PVVariableInImpl<std::int32_t>> m_MaxTimestamps_PV;

     std::shared_ptr<PVVariableInImpl<std::int32_t>> m_Overflow_PV;

     std::shared_ptr<StateMachineImpl> m_StateMachine;
  };
}

#endif // NDSTIMESTAMPINGIMPL_H
