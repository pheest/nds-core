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
 * @file firmwareSupport.h
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
 * @tparam T  the PV data type.
 *            The following data types are supported:
 *            - std::int32_t
 *            - std::double
 *            - std::vector<std::int8_t>
 *            - std::vector<std::uint8_t>
 *            - std::vector<std::int32_t>
 *            - std::vector<double>
 *            - std::string
 *
 */
template <typename T>
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
    FirmwareSup(const FirmwareSup<T>& right);

    FirmwareSup& operator=(const FirmwareSup<T>& right);

    /**
     * @brief Constructs the firmware support device node.
     *
     */
    FirmwareSup(const std::string& name,  ///< The node's name
    				readerString_t PV_FirmwareVersion_Reader, ///< Delegate function that performs the actions to get the firmware version
    				readerString_t PV_FirmwareStatus_Reader, ///< Delegate function that performs the actions to get the firmware status
    				readerString_t PV_HardwareRevision_Reader, ///< Delegate function that performs the actions to get the hardware revision id
    				readerString_t PV_SerialNumber_Reader, ///< Delegate function that performs the actions to get the device serial number
    				readerString_t PV_DeviceModel_Reader, ///< Delegate function that performs the actions to get the device model
    				readerString_t PV_DeviceType_Reader, ///< Delegate function that performs the actions to get the device type (DAQ/IMAQ)
    				writerString_t PV_FirmwarePath_Writer); ///< Delegate function that performs the actions to set the firmware path

    /**
     * @brief Retrieve the Firmware Path
     *
     * @return the firmware path
     */
    std::string getFirmwarePath();

    /**
     * @brief Sets the value of the m_FirmwarePath_RBV.
     *
     */
    void setFirmwarePath(const timespec& timestamp, const std::string& value);

};


}
#endif // NDSFIRMWARESUP_H

