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

FirmwareSup::FirmwareSup(): Node()
{
}

/**
 * @brief Constructs the firmware support node
 *
 * @param name        the node name
 */
FirmwareSup::FirmwareSup(const std::string& name,
		stateChange_t switchOnFunction,
		stateChange_t switchOffFunction,
		stateChange_t startFunction,
		stateChange_t stopFunction,
		stateChange_t recoverFunction,
		allowChange_t allowStateChangeFunction,
		writerString_t PV_FirmwarePath_Writer):
						Node(std::shared_ptr<FirmwareSupImpl>(new FirmwareSupImpl(	name,
																							switchOnFunction,
																							switchOffFunction,
																							startFunction,
																							stopFunction,
																							recoverFunction,
																							allowStateChangeFunction,
																							PV_FirmwarePath_Writer)))
{
}

FirmwareSup::FirmwareSup(const FirmwareSup& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

FirmwareSup& FirmwareSup::operator=(const FirmwareSup& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

void FirmwareSup::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

void FirmwareSup::push(const timespec& timestamp, const std::string& data)
{
    std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->push(timestamp, data);
}

timespec FirmwareSup::getStartTimestamp() const
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->getStartTimestamp();
}

std::string FirmwareSup::getFirmwareVersion()
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->getFirmwareVersion();
}

std::string FirmwareSup::getFirmwareStatus()
{
    return std::static_pointer_cast<FirmwareSupImpl>(m_pImplementation)->getFirmwareStatus();
}

std::string FirmwareSup::getHardwareRevision()
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->getHardwareRevision();
}

std::string FirmwareSup::getSerialNumber()
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->getSerialNumber();
}

std::string FirmwareSup::getDeviceModel()
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->getDeviceModel();
}

std::string FirmwareSup::getDeviceType()
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->getDeviceType();
}

std::string FirmwareSup::getFirmwarePath()
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->getFirmwarePath();
}

void FirmwareSup::setFirmwareVersion(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->setFirmwareVersion(timestamp, value);
}

void FirmwareSup::setFirmwareStatus(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->setFirmwareStatus(timestamp, value);
}

void FirmwareSup::setHardwareRevision(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->setHardwareRevision(timestamp, value);
}

void FirmwareSup::setSerialNumber(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->setSerialNumber(timestamp, value);
}

void FirmwareSup::setDeviceModel(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->setDeviceModel(timestamp, value);
}

void FirmwareSup::setDeviceType(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->setDeviceType(timestamp, value);
}

void FirmwareSup::setFirmwarePath(const timespec& timestamp, const std::string& value)
{
    return std::static_pointer_cast<FirmwareSupImpl >(m_pImplementation)->setFirmwarePath(timestamp, value);
}


}
