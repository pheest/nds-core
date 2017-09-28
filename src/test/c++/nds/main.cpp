// imebra_tests.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/testDevice.h"
#include "../include/testDeviceDataGen_VI8.h"
#include "../include/testDeviceDataGen_VUI8.h"
#include "../include/testDeviceDataGen_VI32.h"
#include "../include/testDeviceDataGen_DBL.h"
#include "../include/testDeviceDataGen_I32.h"

#include "../include/ndsTestFactory.h"

int main(int argc, char **argv)
{
    nds::Factory::registerDriver("testDevice",
                           std::bind(&testDevice::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&testDevice::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("testDeviceDataGenVI8",
                           std::bind(&testDeviceDataGenVI8::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&testDeviceDataGenVI8::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("testDeviceDataGenVUI8",
                           std::bind(&testDeviceDataGenVUI8::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&testDeviceDataGenVUI8::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("testDeviceDataGenVI32",
                           std::bind(&testDeviceDataGenVI32::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&testDeviceDataGenVI32::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("testDeviceDataGenDBL",
                           std::bind(&testDeviceDataGenDBL::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&testDeviceDataGenDBL::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("testDeviceDataGenI32",
                           std::bind(&testDeviceDataGenI32::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&testDeviceDataGenI32::deallocateDevice, std::placeholders::_1));


    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(new nds::tests::TestControlSystemFactoryImpl()));
    nds::Factory::registerControlSystem(testControlSystem);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
