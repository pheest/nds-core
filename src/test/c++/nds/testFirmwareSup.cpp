#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/Device.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testFirmwareSupport, testVariables)
{
	const timespec* pDeviceStateMachineSwitchTime;
	const std::int32_t* pDeviceStateMachineState;
	const timespec* pFirmwareSupStateMachineSwitchTime;
	const std::int32_t* pFirmwareSupStateMachineState;
	timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type Device and named rootNode
    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check Device initial state (OFF)
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pDeviceStateMachineSwitchTime, pDeviceStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pDeviceStateMachineState);

	//Change Device state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pDeviceStateMachineSwitchTime, pDeviceStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pDeviceStateMachineState);
	::sleep(10);
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pDeviceStateMachineSwitchTime, pDeviceStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pDeviceStateMachineState);

    //Get the firmware version
    std::string firmwareVersion;
    pInterface->readCSValue("/rootNode-Firm.Version", &timestamp, &firmwareVersion);
    EXPECT_EQ((std::string)"Firmware test version", firmwareVersion);

    //Get the firmware status
    std::string firmwareStatus;
    pInterface->readCSValue("/rootNode-Firm.Status", &timestamp, &firmwareStatus);
    EXPECT_EQ((std::string)"Firmware test status", firmwareStatus);

    //Get the hardware revision
    std::string hardwareRevision;
    pInterface->readCSValue("/rootNode-Firm.HWRevision", &timestamp, &hardwareRevision);
    EXPECT_EQ((std::string)"Firmware test hardware revision", hardwareRevision);

    //Get the serial number
    std::string serialNumber;
    pInterface->readCSValue("/rootNode-Firm.SerialNumber", &timestamp, &serialNumber);
    EXPECT_EQ((std::string)"Firmware test serial number", serialNumber);

    //Get the device model
    std::string deviceModel;
    pInterface->readCSValue("/rootNode-Firm.Model", &timestamp, &deviceModel);
    EXPECT_EQ((std::string)"Firmware test device model", deviceModel);

    //Get the device type
    std::string deviceType;
    pInterface->readCSValue("/rootNode-Firm.Type", &timestamp, &deviceType);
    EXPECT_EQ((std::string)"Firmware test device type", deviceType);

    //Get the initial firmware path
    std::string firmwarePath;
    pInterface->readCSValue("/rootNode-Firm.FilePath_RBV", &timestamp, &firmwarePath);
    EXPECT_EQ("Firmware path to be uploaded", firmwarePath);

    // Check FirmwareNode initial state (OFF)
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pFirmwareSupStateMachineState);

	//Change FirmwareNode state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pFirmwareSupStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pFirmwareSupStateMachineState);

	//Change FirmwareNode state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pFirmwareSupStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pFirmwareSupStateMachineState);
	::sleep(2);

	//Change Firmware Path
	std::string NewFirmwarePath="New FirmwarePath";
	pInterface->writeCSValue("/rootNode-Firm.FilePath", timestamp, NewFirmwarePath);
	pInterface->readCSValue("/rootNode-Firm.FilePath_RBV", &timestamp, &NewFirmwarePath);
	EXPECT_EQ((std::string)"New FirmwarePath", NewFirmwarePath);

	//Change FirmwareNode state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pFirmwareSupStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pFirmwareSupStateMachineState);

	//Change FirmwareNode state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-Firm.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pFirmwareSupStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-Firm.StateMachine.getState", pFirmwareSupStateMachineSwitchTime, pFirmwareSupStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pFirmwareSupStateMachineState);

	//Change Device state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pDeviceStateMachineSwitchTime, pDeviceStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pDeviceStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pDeviceStateMachineSwitchTime, pDeviceStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pDeviceStateMachineState);

    // Destroy test device
    factory.destroyDevice("rootNode");

}

