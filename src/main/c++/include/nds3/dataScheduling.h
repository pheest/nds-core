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

#ifndef NDS_DATASCHEDULING_H
#define NDS_DATASCHEDULING_H


#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

template <typename T>
class NDS3_API DataScheduling: virtual public Node
{

protected:

  DataScheduling();

  DataScheduling(const std::string& name,  ///< The node's name.
               size_t numberInputs, ///< Node's number of PV inputs.
               size_t numberOutputs, ///< Node's number of PV outputs.
               stateChange_t switchOnFunction,         ///< Delegate function that performs the actions to switch the node on.
               stateChange_t switchOffFunction,        ///< Delegate function that performs the actions to switch the node off.
               stateChange_t startFunction,            ///< Delegate function that performs the actions to start the data scheduling.
               stateChange_t stopFunction,             ///< Delegate function that performs the actions to stop the data scheduling.
               stateChange_t recoverFunction,          ///< Delegate function to execute to recover from an error state.
               allowChange_t allowStateChangeFunction, ///< Delegate function that can deny a state change. Usually just returns true.
               writerInt32_t triggerAction ///< Delegate function that defines what the node does to generate its outputs.
               );

  DataScheduling(const DataScheduling<T>& right);

public:

  /**
   * @ingroup Scheduling
   * @brief Set the function that retrieves the exact start time when the data processing starts.
   *
   * @param timestampDelegate function that returns the exact starting time of the
   *                           data scheduling
   */
  void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

  timespec getStartTimestamp() const;

  DataScheduling& operator=(const DataScheduling& right);

};


}

#endif //NDS_DATASCHEDULING_H
