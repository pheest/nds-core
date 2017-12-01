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
			stateChange_t switchOnFunction,               ///< Delegate function that performs the actions to switch the node on
			stateChange_t switchOffFunction,              ///< Delegate function that performs the actions to switch the node off
			stateChange_t startFunction,                  ///< Delegate function that performs the actions to start the acquisition (usually launches the acquisition thread)
			stateChange_t stopFunction,                   ///< Delegate function that performs the actions to stop the acquisition (usually stops the acquisition thread)
			stateChange_t recoverFunction,                ///< Delegate function to execute to recover from an error state
			allowChange_t allowStateChangeFunction,       ///< Delegate function that can deny a state change. Usually just returns true
			writerString_t PV_FirmwarePath_Writer); 	  ///< Delegate function that performs the actions to set the firmware path

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
    void push(const timespec& timestamp, const T& data);

    /**
     * @ingroup
     * @brief Returns the timestamp at start.
     *
     * @return the time when started.
     */
    //TODO: Discuss if necessary
    timespec getStartTimestamp() const;

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();
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
     * @brief Sets the value of the Firmware Path
     *
     */
    void setFirmwarePath(const timespec& timestamp, const std::string& value);

protected:

    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_OnStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Generation start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;
	// PVs
	std::shared_ptr<PVVariableInImpl<std::string> > m_FirmwareVersion_PV;
	std::shared_ptr<PVVariableInImpl<std::string> > m_FirmwareStatus_PV;
	std::shared_ptr<PVVariableInImpl<std::string> > m_HardwareRevision_PV;
	std::shared_ptr<PVVariableInImpl<std::string> > m_SerialNumber_PV;
	std::shared_ptr<PVVariableInImpl<std::string> > m_DeviceModel_PV;
	std::shared_ptr<PVVariableInImpl<std::string> > m_DeviceType_PV;
	std::shared_ptr<PVDelegateOutImpl<std::string> > m_FirmwarePath_PV;
	std::shared_ptr<PVVariableInImpl<std::string> > m_FirmwarePath_RBVPV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;

};

}
#endif // NDSFIRMWARESUPIMP_H

