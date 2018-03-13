/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSFIRMWARESUP_H
#define NDSFIRMWARESUP_H

/**
 * @file firmwareSup.h
 * @brief Defines the nds::FirmwareSup node, which provides support to obtain
 * 			basic information about the device
 *
 * Include nds.h instead of this one, since nds3.h takes care of including all the
 * necessary header files (including this one).
 */

#include "nds3/definitions.h"
#include "nds3/node.h"

namespace nds
{

/**
 * This is a node that supplies a firmware support  with a few control
 * PVs that provides to the Control System information about the device.
 *
 * The user of a FirmwareSup class must declare few delegate functions that
 * provide some information about the device.
 *
 * In particular, the information to be provided should be: the firmware version,
 * the firmware status, the hardware version id, the device serial number, the
 * device model, and the device type DAQ/IMAQ
 *
 */

class NDS3_API FirmwareSup: public Node
{
public:
    /**
     * @brief Initializes an empty data acquisition node.
     *
     * You must assign a valid FirmwareSup node before calling initialize().
     */
    FirmwareSup();

    /**
     * @brief Copies a firmware support reference from another object.
     *
     * @param right a firmware support holder from which the reference to
     *        the firmware object implementation is copied
     */
    FirmwareSup(const FirmwareSup& right);

    FirmwareSup& operator=(const FirmwareSup& right);

    /**
     * @brief Constructs the firmware support device node.
     *
     */
    FirmwareSup(const std::string& name,  ///< The node's name
	stateChange_t switchOnFunction,   ///< Delegate function that performs the actions to switch the node on
	stateChange_t switchOffFunction,  ///< Delegate function that performs the actions to switch the node off
	stateChange_t startFunction,      ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
	stateChange_t stopFunction,      ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
	stateChange_t recoverFunction,   ///< Delegate function to execute to recover from an error state
	allowChange_t allowStateChangeFunction,   ///< Delegate function that can deny a state change. Usually just returns true
	writerString_t PV_FirmwarePath_Writer);  ///< Delegate function that performs the actions to set the firmware path


    /**
     * @ingroup
     * @brief Set the function that retrieves the exact start time when starts.
     *
     * @param
     *
     */
    //TODO: Discuss if necessary
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
    void push(const timespec& timestamp, const std::string& data);

    /**
     * @ingroup
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

    /**
     * @brief Retrieve the Firmware Version
     *
     * @return the Firmware Version
     */
    std::string getFirmwareVersion();
    /**
     * @brief Retrieve the Firmware Status
     *
     * @return the Firmware Status
     */
    std::string getFirmwareStatus();
    /**
     * @brief Retrieve the Hardware Revision
     *
     * @return the Hardware Revision
     */
    std::string getHardwareRevision();
    /**
     * @brief Retrieve the Serial Number
     *
     * @return the Serial Number
     */
    std::string getSerialNumber();
    /**
     * @brief Retrieve the Device Model
     *
     * @return the Device Model
     */
    std::string getDeviceModel();
    /**
     * @brief Retrieve the Device Type
     *
     * @return the Device Type
     */
    std::string getDeviceType();
    /**
     * @brief Retrieve the Driver Version
     *
     * @return the Driver Version
     */
    std::string getDriverVersion();
    /**
     * @brief Retrieve the Chassis Number
     *
     * @return the Chassis Number
     */
    int32_t getChassisNumber();
    /**
     * @brief Retrieve the Slot Number
     *
     * @return the Slot Number
     */
    int32_t getSlotNumber();

    /**
     * @brief Retrieve the Firmware Path
     *
     * @return the Firmware Path
     */
    std::string getFirmwarePath();
    /**
     * @brief Sets the value of the Firmware Version
     *
     */
    void setFirmwareVersion(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Firmware Status
     *
     */
    void setFirmwareStatus(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Hardware Revision
     *
     */
    void setHardwareRevision(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Serial Number
     *
     */
    void setSerialNumber(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Device Model
     *
     */
    void setDeviceModel(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Device Type
     *
     */
    void setDeviceType(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Driver Version
     *
     */
    void setDriverVersion(const timespec& timestamp, const std::string& value);
    /**
     * @brief Sets the value of the Chassis Number
     *
     */
    void setChassisNumber(const timespec& timestamp, const int32_t& value);
    /**
     * @brief Sets the value of the Slot Number
     *
     */
    void setSlotNumber(const timespec& timestamp, const int32_t& value);
    /**
     * @brief Sets the value of the Firmware Path
     *
     */
    void setFirmwarePath(const timespec& timestamp, const std::string& value);

};


}
#endif // NDSFIRMWARESUP_H

