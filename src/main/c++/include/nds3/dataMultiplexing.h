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

#ifndef NDS_DATAMULTIPLEXING_H
#define NDS_DATAMULTIPLEXING_H


#include "nds3/definitions.h"
#include "nds3/dataScheduling.h"

namespace nds
{

template <typename T>
class NDS3_API DataMultiplexing: public DataScheduling<T>
{

public:

  DataMultiplexing();
  DataMultiplexing(const std::string& name,  ///< The node's name.
               size_t numberInputs ///< Node's number of PV inputs for the node. They are defined as NDS Output PVs.
               );

  void setSamplesPerChannel(const timespec &timestamp, const std::int32_t &maxSamples);
  std::int32_t getSamplesPerChannel();

};


}

#endif //NDS_DATAMULTIPLEXING_H
