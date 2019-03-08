#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <ndsex1.h>
#include "ndsTestInterface.h"

TEST(testDeviceAllocation, testDeviceAllocation)
{
	//first we create the factory
    nds::Factory factory("test");
    factory.createDevice("Device", "rootNode", nds::namedParameters_t());
    EXPECT_NE((void*)0, Device::getInstance("rootNode"));

    factory.destroyDevice("rootNode");
}


