/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * Modified by GMV & UPM
 *
 */


#include "nds3/definitions.h"
#include "nds3/impl/firmwareSupImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvVariableInImpl.h"

namespace nds
{

template<typename T>
FirmwareSupImpl<T>::FirmwareSupImpl(const std::string& name,
									stateChange_t switchOnFunction,
									stateChange_t switchOffFunction,
									stateChange_t startFunction,
									stateChange_t stopFunction,
									stateChange_t recoverFunction,
									allowChange_t allowStateChangeFunction,
									writerString_t PV_FirmwarePath_Writer):
	NodeImpl(name, nodeType_t::inputChannel),
	m_OnStartDelegate(startFunction),
	m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
	m_FirmwareVersion_PV.reset(new PVVariableInImpl<std::string>("FirmwareVersion"));
	m_FirmwareVersion_PV->setDescription("Firmware version");
	m_FirmwareVersion_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_FirmwareVersion_PV);

	m_FirmwareStatus_PV.reset(new PVVariableInImpl<std::string>("FirmwareStatus"));
	m_FirmwareStatus_PV->setDescription("Firmware status");
	m_FirmwareStatus_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_FirmwareStatus_PV);

	m_HardwareRevision_PV.reset(new PVVariableInImpl<std::string>("HardwareRevision"));
	m_HardwareRevision_PV->setDescription("Hardware revision");
	m_HardwareRevision_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_HardwareRevision_PV);

	m_SerialNumber_PV.reset(new PVVariableInImpl<std::string>("SerialNumber"));
	m_SerialNumber_PV->setDescription("Serial number");
	m_SerialNumber_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SerialNumber_PV);

	m_DeviceModel_PV.reset(new PVVariableInImpl<std::string>("DeviceModel"));
	m_DeviceModel_PV->setDescription("Device model");
	m_DeviceModel_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DeviceModel_PV);

	m_DeviceType_PV.reset(new PVVariableInImpl<std::string>("DeviceType"));
	m_DeviceType_PV->setDescription("Device type");
	m_DeviceType_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DeviceType_PV);

	m_FirmwarePath_PV.reset(new PVDelegateOutImpl<std::string>("FirmwarePath",PV_FirmwarePath_Writer));
	m_FirmwarePath_PV->setDescription("Firmware path");
	m_FirmwarePath_PV->setScanType(scanType_t::passive, 0);
	addChild(m_FirmwarePath_PV);

	m_FirmwarePath_RBVPV.reset(new PVVariableInImpl<std::string>("FirmwarePath_RBV"));
	m_FirmwarePath_RBVPV->setDescription("Firmware path ReadBack");
	m_FirmwarePath_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_FirmwarePath_RBVPV);

    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&FirmwareSupImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);
}

template<typename T>
timespec FirmwareSupImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void FirmwareSupImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void FirmwareSupImpl<T>::push(const timespec& timestamp, const T& data)
{
	//TODO
}

template<typename T>
void FirmwareSupImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    //Set Decimation when push method is defined
    m_OnStartDelegate();
}


template <typename T>
std::string FirmwareSupImpl<T>::getFirmwareVersion()
{
    std::string FirmwareVersion;
    timespec timestamp;
    m_FirmwareVersion_PV->read(&timestamp, &FirmwareVersion);
    return (std::string)FirmwareVersion;
}
template <typename T>
std::string FirmwareSupImpl<T>::getFirmwareStatus()
{
    std::string FirmwareStatus;
    timespec timestamp;
    m_FirmwareStatus_PV->read(&timestamp, &FirmwareStatus);
    return (std::string)FirmwareStatus;
}
template <typename T>
std::string FirmwareSupImpl<T>::getHardwareRevision()
{
    std::string HardwareRevision;
    timespec timestamp;
    m_HardwareRevision_PV->read(&timestamp, &HardwareRevision);
    return (std::string)HardwareRevision;
}
template <typename T>
std::string FirmwareSupImpl<T>::getSerialNumber()
{
    std::string SerialNumber;
    timespec timestamp;
    m_SerialNumber_PV->read(&timestamp, &SerialNumber);
    return (std::string)SerialNumber;
}
template <typename T>
std::string FirmwareSupImpl<T>::getDeviceModel()
{
    std::string DeviceModel;
    timespec timestamp;
    m_DeviceModel_PV->read(&timestamp, &DeviceModel);
    return (std::string)DeviceModel;
}
template <typename T>
std::string FirmwareSupImpl<T>::getDeviceType()
{
    std::string DeviceType;
    timespec timestamp;
    m_DeviceType_PV->read(&timestamp, &DeviceType);
    return (std::string)DeviceType;
}
template <typename T>
std::string FirmwareSupImpl<T>::getFirmwarePath()
{
    std::string FirmwarePath;
    timespec timestamp;
    m_FirmwarePath_RBVPV->read(&timestamp, &FirmwarePath);
    return (std::string)FirmwarePath;
}
template<typename T>
void FirmwareSupImpl<T>::setFirmwareVersion(const timespec& timestamp, const std::string& value)
{
    m_FirmwareVersion_PV->setValue(timestamp, value);
    m_FirmwareVersion_PV->push(timestamp, value);
}
template<typename T>
void FirmwareSupImpl<T>::setFirmwareStatus(const timespec& timestamp, const std::string& value)
{
    m_FirmwareStatus_PV->setValue(timestamp, value);
    m_FirmwareStatus_PV->push(timestamp, value);
}
template<typename T>
void FirmwareSupImpl<T>::setHardwareRevision(const timespec& timestamp, const std::string& value)
{
    m_HardwareRevision_PV->setValue(timestamp, value);
    m_HardwareRevision_PV->push(timestamp, value);
}
template<typename T>
void FirmwareSupImpl<T>::setSerialNumber(const timespec& timestamp, const std::string& value)
{
    m_SerialNumber_PV->setValue(timestamp, value);
    m_SerialNumber_PV->push(timestamp, value);
}
template<typename T>
void FirmwareSupImpl<T>::setDeviceModel(const timespec& timestamp, const std::string& value)
{
    m_DeviceModel_PV->setValue(timestamp, value);
    m_DeviceModel_PV->push(timestamp, value);
}
template<typename T>
void FirmwareSupImpl<T>::setDeviceType(const timespec& timestamp, const std::string& value)
{
    m_DeviceType_PV->setValue(timestamp, value);
    m_DeviceType_PV->push(timestamp, value);
}

template<typename T>
void FirmwareSupImpl<T>::setFirmwarePath(const timespec& timestamp, const std::string& value)
{
    m_FirmwarePath_RBVPV->setValue(timestamp, value);
    m_FirmwarePath_RBVPV->push(timestamp, value);
}


template class FirmwareSupImpl<std::string>;



}
