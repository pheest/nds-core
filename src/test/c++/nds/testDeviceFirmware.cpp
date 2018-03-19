#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDeviceFirmware, fullTest)
{
  const timespec* pDeviceStateMachineSwitchTime;
  const std::int32_t* pDeviceStateMachineState;
  const timespec* pFirmwareStateMachineSwitchTime;
  const std::int32_t* pFirmwareStateMachineState;
  timespec timestamp = {0, 0};

  //Create factory
  nds::Factory factory("test");

  // Create test device of type DeviceFirmware and name it deviceFirmware
  factory.createDevice("DeviceFirmware", "deviceFirmware", nds::namedParameters_t());

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface = 
    nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceFirmware");


  // Check FirmwareNode initial state (OFF)
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
              pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pFirmwareStateMachineState);


  //Get the Firmware global state
  std::int32_t globalStateFirmware;
  pInterface->readCSValue("/deviceFirmware-Firm.StateMachine.getGlobalState", 
      &timestamp, &globalStateFirmware);
  EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateFirmware);

  //Change FirmwareNode state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState", 
      timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
      pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pFirmwareStateMachineState);

  ::sleep(2);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
      pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pFirmwareStateMachineState);

  //Get the Firmware global state
  pInterface->readCSValue("/deviceFirmware-Firm.StateMachine.getGlobalState", 
      &timestamp, &globalStateFirmware);
  EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateFirmware);

  //Get the firmware version
  std::string firmwareVersion;
  pInterface->readCSValue("/deviceFirmware-Firm.Version", &timestamp, &firmwareVersion);
  EXPECT_EQ((std::string)"Firmware test version", firmwareVersion);

  //Get the firmware status
  std::int32_t firmwareStatus;
  pInterface->readCSValue("/deviceFirmware-Firm.Status", &timestamp, &firmwareStatus);
  EXPECT_EQ(0, firmwareStatus);

  //Get the hardware revision
  std::string hardwareRevision;
  pInterface->readCSValue("/deviceFirmware-Firm.HWRevision", &timestamp, &hardwareRevision);
  EXPECT_EQ((std::string)"Firmware test hardware revision", hardwareRevision);

  //Get the serial number
  std::string serialNumber;
  pInterface->readCSValue("/deviceFirmware-Firm.SerialNumber", &timestamp, &serialNumber);
  EXPECT_EQ((std::string)"Firmware test serial number", serialNumber);

  //Get the device model
  std::string deviceModel;
  pInterface->readCSValue("/deviceFirmware-Firm.Model", &timestamp, &deviceModel);
  EXPECT_EQ((std::string)"Firmware test device model", deviceModel);

  //Get the device type
  std::string deviceType;
  pInterface->readCSValue("/deviceFirmware-Firm.Type", &timestamp, &deviceType);
  EXPECT_EQ((std::string)"Firmware test device type", deviceType);

  //Get the driver version
  std::string driverVersion;
  pInterface->readCSValue("/deviceFirmware-Firm.DriverVersion", &timestamp, &driverVersion);
  EXPECT_EQ("<major_id>.<minor_id>.<maintenance_id>", driverVersion);

  //Get the chassis number
  std::int32_t chassisNumber;
  pInterface->readCSValue("/deviceFirmware-Firm.ChassisNumber", &timestamp, &chassisNumber);
  EXPECT_EQ(42, chassisNumber);

  //Get the slot number
  std::int32_t slotNumber;
  pInterface->readCSValue("/deviceFirmware-Firm.SlotNumber", &timestamp, &slotNumber);
  EXPECT_EQ(42, slotNumber);

  //Get the initial firmware path
  std::string firmwarePath;
  pInterface->readCSValue("/deviceFirmware-Firm.FilePath_RBV", &timestamp, &firmwarePath);
  EXPECT_EQ("Firmware path to be uploaded", firmwarePath);


  //Change FirmwareNode state:  ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState", 
      timestamp, (std::int32_t)nds::state_t::running);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
      pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::starting, *pFirmwareStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
      pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::running, *pFirmwareStateMachineState);
  ::sleep(2);


  //Get the Firmware global state
  pInterface->readCSValue("/deviceFirmware-Firm.StateMachine.getGlobalState", 
      &timestamp, &globalStateFirmware);
  EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateFirmware);

  //Change Firmware Path
  std::string NewFirmwarePath="New FirmwarePath";
  pInterface->writeCSValue("/deviceFirmware-Firm.FilePath", timestamp, NewFirmwarePath);
  pInterface->readCSValue("/deviceFirmware-Firm.FilePath_RBV", &timestamp, &NewFirmwarePath);
  EXPECT_EQ((std::string)"New FirmwarePath", NewFirmwarePath);

  //Change FirmwareNode state:  RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState", 
      timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
      pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pFirmwareStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
      pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pFirmwareStateMachineState);

  //Change FirmwareNode state:  ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceFirmware-Firm.StateMachine.setState", 
      timestamp, (std::int32_t)nds::state_t::off);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
      pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pFirmwareStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceFirmware-Firm.StateMachine.getState", 
      pFirmwareStateMachineSwitchTime, pFirmwareStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pFirmwareStateMachineState);

  // Destroy test device
  factory.destroyDevice("deviceFirmware");

}


