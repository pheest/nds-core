/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2017 GMV
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 */

#include "nds3/firmwareSup.h"
#include "../include/nds3/impl/firmwareSupImpl.h"

namespace nds
{

template <typename T>
FirmwareSup<T>::FirmwareSup(): Node()
{
}

/**
 * @brief Constructs the firmware support node
 *
 * @param name        the node name
 */
template <typename T>
FirmwareSup<T>::FirmwareSup(const std::string& name,  ///< The node's name
							readerString_t PV_FirmwareVersion_Reader, ///< Delegate function that performs the actions to get the firmware version
							readerString_t PV_FirmwareStatus_Reader, ///< Delegate function that performs the actions to get the firmware status
							readerString_t PV_HardwareRevision_Reader, ///< Delegate function that performs the actions to get the hardware revision id
							readerString_t PV_SerialNumber_Reader, ///< Delegate function that performs the actions to get the device serial number
							readerString_t PV_DeviceModel_Reader, ///< Delegate function that performs the actions to get the device model
							readerString_t PV_DeviceType_Reader, ///< Delegate function that performs the actions to get the device type (DAQ/IMAQ)
							writerString_t PV_FirmwarePath_Writer) : ///< Delegate function that performs the actions to set the firmware path
						Node(std::shared_ptr<FirmwareSupImpl<T> >(new FirmwareSupImpl<T>(name,
										PV_FirmwareVersion_Reader,
										PV_FirmwareStatus_Reader,
										PV_HardwareRevision_Reader,
										PV_SerialNumber_Reader,
										PV_DeviceModel_Reader,
										PV_DeviceType_Reader,
										PV_FirmwarePath_Writer)))
{
}
template <typename T>
FirmwareSup<T>::FirmwareSup(const FirmwareSup<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
FirmwareSup<T>& FirmwareSup<T>::operator=(const FirmwareSup<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
std::string FirmwareSup<T>::getFirmwarePath()
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getFirmwarePath();
}

template <typename T>
void FirmwareSup<T>::setFirmwarePath(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setFirmwarePath(timestamp, value);
}

template class FirmwareSup<std::int32_t>;
template class FirmwareSup<std::string>;
template class FirmwareSup<double>;
template class FirmwareSup<std::vector<std::int8_t> >;
template class FirmwareSup<std::vector<std::uint8_t> >;
template class FirmwareSup<std::vector<std::int32_t> >;
template class FirmwareSup<std::vector<double> >;


}
