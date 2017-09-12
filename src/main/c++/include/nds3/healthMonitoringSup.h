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
	HealthMonitSup(	const std::string& name,                             ///< The node's name
            		stateChange_t switchOnFunction,                      ///< Delegate function that performs the actions to switch the node on
					stateChange_t switchOffFunction,                     ///< Delegate function that performs the actions to switch the node off
					stateChange_t startFunction,                         ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
					stateChange_t stopFunction,                          ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
					stateChange_t recoverFunction,                       ///< Delegate function to execute to recover from an error state
					allowChange_t allowStateChangeFunction,              ///< Delegate function that can deny a state change. Usually just returns truereaderDouble_t PV_DevicePower_Reader,
					readerDouble_t PV_DevicePower_Reader,				 ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_DeviceTemp_Reader,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_DeviceVoltage_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_DeviceCurrent_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_EnableSEU_Writer,                   ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_EnableSEU_Reader,                   ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_EnableMonitorDAQ_Writer,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_EnableMonitorDAQ_Reader,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_EnableShelfTest_Writer,             ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_EnableShelfTest_Reader,             ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_ShelfTestType_Writer,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_ShelfTestType_Reader,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_VerboseShelfTest_Writer,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_VerboseShelfTest_Reader,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_EnableShelfTestId_Writer,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_EnableShelfTestId_Reader,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_EnableShelfTestText_Writer,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_EnableShelfTestText_Reader,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_SignalQualityFlag_Reader,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_SignalQualityFlagLevel_Reader);    ///< Delegate function setter/getter to interact to the Low Level Driver API

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

