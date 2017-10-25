/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSFIRMWARESUPIMP_H
#define NDSFIRMWARESUPIMP_H

#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"
#include "nds3/impl/pvVariableInImpl.h"

namespace nds
{

//template <typename T> class PVDelegateInImpl;

template <typename T>
class FirmwareSupImpl: public NodeImpl
{
public:
    FirmwareSupImpl(const std::string& name,  ///< The node's name
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

protected:

	// PVs
	std::shared_ptr<PVDelegateInImpl<std::string> > m_firmwareVersion_PV;
	std::shared_ptr<PVDelegateInImpl<std::string> > m_firmwareStatus_PV;
	std::shared_ptr<PVDelegateInImpl<std::string> > m_hardwareRevision_PV;
	std::shared_ptr<PVDelegateInImpl<std::string> > m_serialNumber_PV;
	std::shared_ptr<PVDelegateInImpl<std::string> > m_deviceModel_PV;
	std::shared_ptr<PVDelegateInImpl<std::string> > m_deviceType_PV;
	std::shared_ptr<PVDelegateOutImpl<std::string> > m_firmwarePath_PV;
	std::shared_ptr<PVVariableInImpl<std::string> > m_firmwarePath_RBVPV;

};

}
#endif // NDSFIRMWARESUPIMP_H

