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
	HealthMonitSup(const HealthMonitSup& right);

	HealthMonitSup& operator=(const HealthMonitSup& right);

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
					readerDouble_t PV_DeviceTemperature_Reader,                 ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_DeviceVoltage_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerDouble_t PV_DeviceCurrent_Reader,              ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SEUEnable_Writer,                   ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_DAQEnable_Writer,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SelfTestEnable_Writer,             ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SelfTestType_Writer,               ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SelfTestVerboseEnable_Writer,            ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SelfTestIDEnable_Writer,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SelfTestTxtEnable_Writer,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerInt32_t PV_SelfTestCodeResultEnable_Writer, ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerString_t PV_SelfTestTextResult_Reader,         ///< Delegate function setter/getter to interact to the Low Level Driver API
					readerInt32_t PV_SignalQualityFlag_Reader,           ///< Delegate function setter/getter to interact to the Low Level Driver API
					writerDouble_t PV_SignalQualityFlagLevel_Writer);    ///< Delegate function setter/getter to interact to the Low Level Driver API

    /**
     * @ingroup
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param timestampDelegate the function that returns the exact starting time
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
    void push(const timespec& timestamp, const std::int32_t& data);

    /**
     * @ingroup
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

    /**
     * ---------------------------------------------------
     * Getter functions
     * ---------------------------------------------------
     */

    /**
     * @brief Retrieve the Device Power
     */
    double getDevicePower();
    /**
	 * @brief Retrieve the Device Temperature
	 */
	double getDeviceTemperature();
    /**
	 * @brief Retrieve the Device Voltage
	 */
	double getDeviceVoltage();
    /**
	 * @brief Retrieve the Device Temperature
	 */
	double getDeviceCurrent();

	/**
	 * @brief Retrieve the status of the flag for detecting Single Event Upsets (SEU)
	 */
	size_t getSEUEnable();

	/**
	 * @brief Retrieve the status of the flag for monitoring DAQ anomalies
	 */
	size_t getDAQMonitorEnable();

	/**
	 * @brief Retrieve the status of the flag for enabling the self-test
	 */
	size_t getSelfTestEnable();

	/**
	 * @brief Retrieve the type of self-test selected (Quick-Test/Full-Test)
	 */
	size_t getSelfTestType();

	/**
	 * @brief Retrieve the status of the flag for enabling verbose in the self-test
	 */
	size_t getSelfTestVerboseEnable();

	/**
	 * @brief Retrieve the status of the flag for enabling the identifier in the self-test
	 */
	size_t getSelfTestIDEnable();

	/**
	 * @brief Retrieve the status of the flag for enabling the textual description in the self-test
	 */
	size_t getSelfTestTextEnable();

	/**
	 * @brief Retrieve the status of the flag for enabling the numeric code with the result of the self-test
	 */
	size_t getSelfTestCodeResultEnable();

	/**
	 * @brief Retrieve a text summarizing the self-test result with the fields whose flags are enabled
	 */
	std::string getSelfTextTxtResult();

	/**
	 * @brief Retrieve the flag that indicates whether the quality signal is too low
	 */
	size_t getSignalQualityFlag();

	/**
	 * @brief Retrieve the trigger level below the signal quality flag should be flagged
	 */
	double getSignalQualityFlagLevel();


    /**
     * ---------------------------------------------------
     * Setter functions
     * ---------------------------------------------------
     */

	/**
	 * @brief Set the Device Power
	 */
	void setDevicePower(const timespec& timestamp, const double& value);

	/**
	 * @brief Set the Device Temperature
	 */
	void setDeviceTemperature(const timespec& timestamp, const double& value);

	/**
	 * @brief Set the Device Voltage
	 */
	void setDeviceVoltage(const timespec& timestamp, const double& value);

	/**
	 * @brief Set the Device Current
	 */
	void setDeviceCurrent(const timespec& timestamp, const double& value);

	/**
	 * @brief Set the status of the flag for detecting Single Event Upsets (SEU)
	 */
	void setSEUEnable(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the status of the flag for monitoring DAQ anomalies
	 */
	void setDAQMonitorEnable(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the status of the flag for enabling the self-test
	 */
	void setSelfTestEnable(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the type of self-test selected (Quick-Test/Full-Test)
	 */
	void setSelfTestType(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the status of the flag for enabling verbose in the self-test
	 */
	void setSelfTestVerboseEnable(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the status of the flag for enabling the identifier in the self-test
	 */
	void setSelfTestIDEnable(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the status of the flag for enabling the textual description in the self-test
	 */
	void setSelfTestTextEnable(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the status of the flag for enabling the numeric code with the result of the self-test
	 */
	void setSelfTestCodeResultEnable(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the text that summarizes the self-test result with the fields whose flags are enabled
	 */
	void setSelfTextTxtResult(const timespec& timestamp, const std::string& value);

	/**
	 * @brief Set the flag that indicates whether the quality signal is too low
	 */
	void setSignalQualityFlag(const timespec& timestamp, const std::int32_t& value);

	/**
	 * @brief Set the the trigger level below the signal quality flag should be flagged
	 */
	void setSignalQualityFlagLevel(const timespec& timestamp, const double& value);

};

}
#endif // NDSHEALTHMONITORINGSUP_H

