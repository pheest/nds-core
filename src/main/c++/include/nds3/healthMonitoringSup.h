/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSHEALTHMONITORINGSUP_H
#define NDSHEALTHMONITORINGSUP_H

/**
 * @file healthMonitoringSup.h
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
class NDS3_API HealthMonitSup: public Node
{
public:
    /**
     * @brief TBD
     *
     */
	HealthMonitSup();

    /**
     * @brief Copies a reference from another object.
     *
     * @param right a holder from which the reference to
     *        the object implementation is copied
     */
	HealthMonitSup(const HealthMonitSup<T>& right);

	HealthMonitSup& operator=(const HealthMonitSup<T>& right);

    /**
     * @brief Constructs the node.
     *
     */
	HealthMonitSup(	const std::string& name,                  ///< The node's name
					readerDouble_t PV_DevicePower_Reader,
					readerDouble_t PV_DeviceTemp_Reader,
					readerDouble_t PV_DeviceVoltage_Reader,
					readerDouble_t PV_DeviceCurrent_Reader,
					writerInt32_t PV_EnableSEU_Writer,
					readerInt32_t PV_EnableSEU_Reader,
					writerInt32_t PV_EnableMonitorDAQ_Writer,
					readerInt32_t PV_EnableMonitorDAQ_Reader,
					writerInt32_t PV_EnableShelfTest_Writer,
					readerInt32_t PV_EnableShelfTest_Reader,
					writerInt32_t PV_ShelfTestType_Writer,
					readerInt32_t PV_ShelfTestType_Reader,
					writerInt32_t PV_VerboseShelfTest_Writer,
					readerInt32_t PV_VerboseShelfTest_Reader,
					writerInt32_t PV_EnableShelfTestId_Writer,
					readerInt32_t PV_EnableShelfTestId_Reader,
					writerInt32_t PV_EnableShelfTestText_Writer,
					readerInt32_t PV_EnableShelfTestText_Reader,
					readerInt32_t PV_SignalQualityFlag_Reader,
					readerDouble_t PV_SignalQualityFlagLevel_Reader);


    /**
     * @ingroup
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param timestampDelegate the function that returns the exact starting time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @ingroup
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;
};

}
#endif // NDSHEALTHMONITORINGSUP_H

