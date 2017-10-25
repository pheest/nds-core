#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/Device.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testFirmwareSupport, testVariables)
{

	timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type Device and named rootNode
    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    //Get the firmware version
    std::string firmwareVersion;
    pInterface->readCSValue("/rootNode-FirmwareNode.FirmwareVersion", &timestamp, &firmwareVersion);
    EXPECT_EQ((std::string)"Firmware test version", firmwareVersion);

    //Get the firmware status
    std::string firmwareStatus;
    pInterface->readCSValue("/rootNode-FirmwareNode.FirmwareStatus", &timestamp, &firmwareStatus);
    EXPECT_EQ((std::string)"Firmware test status", firmwareStatus);

    //Get the hardware revision
    std::string hardwareRevision;
    pInterface->readCSValue("/rootNode-FirmwareNode.HardwareRevision", &timestamp, &hardwareRevision);
    EXPECT_EQ((std::string)"Firmware test hardware", hardwareRevision);

    //Get the serial number
    std::string serialNumber;
    pInterface->readCSValue("/rootNode-FirmwareNode.SerialNumber", &timestamp, &serialNumber);
    EXPECT_EQ((std::string)"Firmware test serial number", serialNumber);

    //Get the device model
    std::string deviceModel;
    pInterface->readCSValue("/rootNode-FirmwareNode.DeviceModel", &timestamp, &deviceModel);
    EXPECT_EQ((std::string)"Firmware test device model", deviceModel);

    //Get the device type
    std::string deviceType;
    pInterface->readCSValue("/rootNode-FirmwareNode.DeviceType", &timestamp, &deviceType);
    EXPECT_EQ((std::string)"Firmware test device type", deviceType);

    //Get/Set the firmware path
    //Use it to check whether a firmware update is available in the system
    const std::string firmwarePathToSet = "Firmware path to be uploaded";
    pInterface->writeCSValue("/rootNode-FirmwareNode.FirmwarePath", timestamp, firmwarePathToSet);
    std::string firmwarePath;
    pInterface->readCSValue("/rootNode-FirmwareNode.FirmwarePath_RBV", &timestamp, &firmwarePath);
    EXPECT_EQ(firmwarePathToSet, firmwarePath);

    // Destroy test device
    factory.destroyDevice("rootNode");

}

