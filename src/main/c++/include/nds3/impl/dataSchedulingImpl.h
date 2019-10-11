/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDS_DATASCHEDULINGIMPL_H
#define NDS_DATASCHEDULINGIMPL_H

#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"

namespace nds
{

template<typename T>
class DataSchedulingImpl: public NodeImpl
{

public:

  DataSchedulingImpl(const std::string& name,  ///< The node's name.
               size_t numberInputs, ///< Node's number of PV inputs for the node. They are defined as NDS Output PVs.
               size_t numberOutputs, ///< Node's number of PV outputs for the node. . They are defined as NDS Input PVs.
               stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on.
               stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off.
               stateChange_t startFunction,            ///< Delegate function that performs the actions to start the data scheduling.
               stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the data scheduling.
               stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state.
               allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true.
               writerInt32_t triggerAction ///< Delegate function that defines what the node does to generate its outputs.
               );


  /**
   * @ingroup Scheduling
   * @brief Set the function that retrieves the exact start time when the data processing starts.
   *
   * @param timestampDelegate function that returns the exact starting time of the
   *                           data scheduling
   */
  void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

  /**
   * @ingroup
   * @brief Returns the timestamp at start.
   *
   * @return the time when started.
   */
  timespec getStartTimestamp() const;

  /**
   * @brief Called by the state machine. Store the current timestamp and then calls the
   *        delegated onStart function.
   */
  void onStart();

protected:

  const size_t nInputs;

  const size_t nOutputs;


  /**
   * @brief In the state machine we set the start function to onStart(), so we
   *        remember here what to call from onStart().
   */
  stateChange_t m_OnStartDelegate;

  /**
   * @brief Delegate function that retrieves the start time. Executed
   *        by onStart().
   *
   * By default points to BaseImpl::getTimestamp().
   *
   * Use setStartTimestampDelegate() to change the delegate function.
   */
  getTimestampPlugin_t m_StartTimestampFunction;

  /**
   * @brief Generation start time. Retrieved during onStart() via the delegate
   *        function declared in  m_startTimestampFunction.
   */
  timespec m_StartTime;

  std::shared_ptr<StateMachineImpl> m_StateMachine;

  std::shared_ptr< PVDelegateOutImpl<std::int32_t> > m_Trigger_PV;


  std::vector< std::shared_ptr< PVVariableOutImpl<T>>> m_DataIn_PV;
  std::vector< std::shared_ptr< PVVariableInImpl<T>>> m_DataOut_PV;


};

}

#endif //NDS_DATASCHEDULINGIMPL_H
