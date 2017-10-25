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
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/impl/pvVariableInImpl.h"

namespace nds
{

template<typename T>
FirmwareSupImpl<T>::FirmwareSupImpl(const std::string& name,
		readerString_t PV_FirmwareVersion_Reader, ///< Delegate function that performs the actions to get the firmware version
		readerString_t PV_FirmwareStatus_Reader, ///< Delegate function that performs the actions to get the firmware status
		readerString_t PV_HardwareRevision_Reader, ///< Delegate function that performs the actions to get the hardware revision id
		readerString_t PV_SerialNumber_Reader, ///< Delegate function that performs the actions to get the device serial number
		readerString_t PV_DeviceModel_Reader, ///< Delegate function that performs the actions to get the device model
		readerString_t PV_DeviceType_Reader,///< Delegate function that performs the actions to get the device type (DAQ/IMAQ)
		writerString_t PV_FirmwarePath_Writer) :  ///< Delegate function that performs the actions to set the firmware path
												NodeImpl(name, nodeType_t::inputChannel)
{
    	// Add the children PVs
	m_firmwareVersion_PV.reset(new PVDelegateInImpl<std::string>("FirmwareVersion", PV_FirmwareVersion_Reader));
	m_firmwareVersion_PV->setDescription("Firmware version");
    	addChild(m_firmwareVersion_PV);

    	m_firmwareStatus_PV.reset(new PVDelegateInImpl<std::string>("FirmwareStatus", PV_FirmwareStatus_Reader));
    	m_firmwareStatus_PV->setDescription("Firmware status");
	addChild(m_firmwareStatus_PV);

    	m_hardwareRevision_PV.reset(new PVDelegateInImpl<std::string>("HardwareRevision", PV_HardwareRevision_Reader));
    	m_hardwareRevision_PV->setDescription("Hardware revision");
	addChild(m_hardwareRevision_PV);

    	m_serialNumber_PV.reset(new PVDelegateInImpl<std::string>("SerialNumber", PV_SerialNumber_Reader));
    	m_serialNumber_PV->setDescription("Serial number");
	addChild(m_serialNumber_PV);

    	m_deviceModel_PV.reset(new PVDelegateInImpl<std::string>("DeviceModel", PV_DeviceModel_Reader));
    	m_deviceModel_PV->setDescription("Device model");
    	m_deviceModel_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_deviceModel_PV);

    	m_deviceType_PV.reset(new PVDelegateInImpl<std::string>("DeviceType", PV_DeviceType_Reader));
    	m_deviceType_PV->setDescription("Device type");
	addChild(m_deviceType_PV);

	m_firmwarePath_PV.reset(new PVDelegateOutImpl<std::string>("FirmwarePath",PV_FirmwarePath_Writer));
	m_firmwarePath_PV->setDescription("Firmware path");
	addChild(m_firmwarePath_PV);

	m_firmwarePath_RBVPV.reset(new PVVariableInImpl<std::string>("FirmwarePath_RBV"));
	m_firmwarePath_RBVPV->setDescription("Firmware path ReadBack");
	m_firmwarePath_RBVPV-> setScanType(scanType_t::passive,0);
	addChild(m_firmwarePath_RBVPV);


}


template <typename T>
std::string FirmwareSupImpl<T>::getFirmwarePath()
{
    std::string firmwarePath;
    timespec timestamp;
    m_firmwarePath_RBVPV->read(&timestamp, &firmwarePath);
    return (std::string)firmwarePath;
}

template<typename T>
void FirmwareSupImpl<T>::setFirmwarePath(const timespec& timestamp, const std::string& value)
{
    m_firmwarePath_RBVPV->setValue(timestamp, value);
}

template class FirmwareSupImpl<std::int32_t>;
template class FirmwareSupImpl<std::string>;
template class FirmwareSupImpl<double>;
template class FirmwareSupImpl<std::vector<std::int8_t> >;
template class FirmwareSupImpl<std::vector<std::uint8_t> >;
template class FirmwareSupImpl<std::vector<std::int32_t> >;
template class FirmwareSupImpl<std::vector<double> >;


}
