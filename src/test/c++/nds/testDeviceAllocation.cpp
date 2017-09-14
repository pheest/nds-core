#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/testDevice.h"
#include "../include/ndsTestInterface.h"

TEST(testDeviceAllocation, testAllocationMissingDevice)
{
	//first we create the factory
    nds::Factory factory("test");

    //testDev1 does not exist, so a testDevice object called rootNode can not be created. Error should be raised
    EXPECT_THROW(factory.createDevice("testDev1", "rootNode", nds::namedParameters_t()), nds::DriverNotFound);
    //testDevice object called rootNode does not exist
    EXPECT_EQ((void*)0, testDevice::getInstance("rootNode"));
}

/*
 * Try to allocate the same device twice with the same name.
 */
TEST(testDeviceAllocation, testDoubleAllocation)
{
    nds::Factory factory("test");

    // First allocation, testDevice exists because has been loaded in the main program, so a testDevice object called rootNode can be created.
    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    // The port should have been allocated
    EXPECT_NE((void*)0, testDevice::getInstance("rootNode"));

    // 2nd allocation of a testDevice object with the same name (rootNode). Error should be raised
    EXPECT_THROW(factory.createDevice("testDevice", "rootNode", nds::namedParameters_t()), nds::DeviceAlreadyCreated);

    // Destroy the first allocated testDevice.
    factory.destroyDevice("rootNode");

    // The port should have been deallocated
    EXPECT_EQ((void*)0, testDevice::getInstance("rootNode"));

    // Try to destroy again the same device. Error should be raised
    EXPECT_THROW(factory.destroyDevice("rootNode"), nds::DeviceNotAllocated);
}

/*
 * Initialize and de-initialize the device.
 * Check that the port is actually allocated and then removed
 */
TEST(testDeviceAllocation, testInitDeinit)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    //factory.subscribe("rootNode-Channel1-variableIn0", "rootNode-Channel1-numAcquisitions");

    factory.destroyDevice("rootNode");

    // If the PVs have been removed the they have been de-registered correctly
    //EXPECT_THROW(factory.subscribe("rootNode-Channel1-variableIn0", "rootNode-Channel1-numAcquisitions"), nds::MissingOutputPV);
}

/*
 * Allocate the same device type with two different names.
 * Both should succeed
 */
TEST(testDeviceAllocation, testTwoAllocations)
{
    nds::Factory factory("test");

    factory.createDevice("testDevice", "rootNode0", nds::namedParameters_t());
    EXPECT_NE((void*)0, testDevice::getInstance("rootNode0"));

    factory.createDevice("testDevice", "rootNode1", nds::namedParameters_t());
    EXPECT_NE((void*)0, testDevice::getInstance("rootNode1"));

    //factory.subscribe("rootNode0-Channel1-data-StateMachine-getState", "rootNode1-Channel1-numAcquisitions");

    //nds::tests::TestControlSystemInterfaceImpl* pInterface0 = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode0-Channel1");
    //nds::tests::TestControlSystemInterfaceImpl* pInterface1 = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode1-Channel1");

    //timespec timestamp = {0, 0};
    //pInterface0->writeCSValue("/rootNode0-Channel1.data.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);

    //const timespec* pStateMachineSwitchTime;
    //const std::int32_t* pStateMachineState;
    //pInterface0->getPushedInt32("/rootNode0-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    //EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
    //pInterface0->getPushedInt32("/rootNode0-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    //EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);

    //timespec readTimestamp;
    //std::int32_t readValue;
    //pInterface1->readCSValue("/rootNode1-Channel1.numAcquisitions", &readTimestamp, &readValue);
    //EXPECT_EQ((std::int32_t)nds::state_t::initializing, readValue);

    factory.destroyDevice("rootNode0");
    factory.destroyDevice("rootNode1");

    EXPECT_EQ((void*)0, testDevice::getInstance("rootNode0"));
    EXPECT_EQ((void*)0, testDevice::getInstance("rootNode1"));

    EXPECT_THROW(factory.destroyDevice("rootNode0"), nds::DeviceNotAllocated);
    EXPECT_THROW(factory.destroyDevice("rootNode1"), nds::DeviceNotAllocated);
}

