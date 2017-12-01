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
FirmwareSup<T>::FirmwareSup(const std::string& name,
		stateChange_t switchOnFunction,
		stateChange_t switchOffFunction,
		stateChange_t startFunction,
		stateChange_t stopFunction,
		stateChange_t recoverFunction,
		allowChange_t allowStateChangeFunction,
		writerString_t PV_FirmwarePath_Writer):
						Node(std::shared_ptr<FirmwareSupImpl<T> >(new FirmwareSupImpl<T>(	name,
																							switchOnFunction,
																							switchOffFunction,
																							startFunction,
																							stopFunction,
																							recoverFunction,
																							allowStateChangeFunction,
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
void FirmwareSup<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
void FirmwareSup<T>::push(const timespec& timestamp, const T& data)
{
    std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->push(timestamp, data);
}

template <typename T>
timespec FirmwareSup<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template <typename T>
std::string FirmwareSup<T>::getFirmwareVersion()
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getFirmwareVersion();
}
template <typename T>
std::string FirmwareSup<T>::getFirmwareStatus()
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getFirmwareStatus();
}
template <typename T>
std::string FirmwareSup<T>::getHardwareRevision()
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getHardwareRevision();
}
template <typename T>
std::string FirmwareSup<T>::getSerialNumber()
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getSerialNumber();
}
template <typename T>
std::string FirmwareSup<T>::getDeviceModel()
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getDeviceModel();
}
template <typename T>
std::string FirmwareSup<T>::getDeviceType()
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getDeviceType();
}
template <typename T>
std::string FirmwareSup<T>::getFirmwarePath()
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->getFirmwarePath();
}
template <typename T>
void FirmwareSup<T>::setFirmwareVersion(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setFirmwareVersion(timestamp, value);
}
template <typename T>
void FirmwareSup<T>::setFirmwareStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setFirmwareStatus(timestamp, value);
}
template <typename T>
void FirmwareSup<T>::setHardwareRevision(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setHardwareRevision(timestamp, value);
}
template <typename T>
void FirmwareSup<T>::setSerialNumber(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setSerialNumber(timestamp, value);
}
template <typename T>
void FirmwareSup<T>::setDeviceModel(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setDeviceModel(timestamp, value);
}
template <typename T>
void FirmwareSup<T>::setDeviceType(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setDeviceType(timestamp, value);
}
template <typename T>
void FirmwareSup<T>::setFirmwarePath(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl<T> >(m_pImplementation)->setFirmwarePath(timestamp, value);
}

template class FirmwareSup<std::string>;



}
