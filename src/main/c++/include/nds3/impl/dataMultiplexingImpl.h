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

#ifndef NDS_DATASMULTIPLEXINGIMPL_H
#define NDS_DATASMULTIPLEXINGIMPL_H

#include "nds3/definitions.h"
#include "nds3/impl/dataSchedulingImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"

namespace nds
{

template<typename T>
class DataMultiplexingImpl: public DataSchedulingImpl<T>
{


public:

  DataMultiplexingImpl(const std::string& name,  ///< The node's name.
               size_t numberInputs ///< Node's number of PV inputs for the node. They are defined as NDS Output PVs.
               );

protected:

  std::shared_ptr< PVDelegateOutImpl<std::int32_t> > m_SamplesPerChannel_PV;
  std::shared_ptr< PVVariableInImpl<std::int32_t> > m_SamplesPerChannel_RBVPV;

protected:

  void switchOn(void);
  void switchOff(void);
  void start(void);
  void stop(void);
  void recover(void);
  bool allowStateChange(const state_t currentState, const state_t currentGlobalState, const state_t newState);
  void multiplex(const timespec &time, const std::int32_t &doIt);

public:
  void setSamplesPerChannel(const timespec &timestamp, const std::int32_t &maxSamples);
  std::int32_t getSamplesPerChannel();
};

}
#endif //NDS_DATASMULTIPLEXINGIMPL_H
