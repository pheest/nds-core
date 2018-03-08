// imebra_tests.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/Device.h"
#include "../include/Device_DBL.h"
#include "../include/Device_I32.h"
#include "../include/Device_Vector_I32.h"
#include "../include/Device_Vector_I8.h"
#include "../include/Device_Vector_UI8.h"
#include "../include/Device_DigitalIO.h"
#include "../include/ndsTestFactory.h"


int main(int argc, char **argv)
{
    nds::Factory::registerDriver("Device",
                           std::bind(&Device::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&Device::deallocateDevice, std::placeholders::_1));


    nds::Factory::registerDriver("DeviceDBL",
                           std::bind(&DeviceDBL::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceDBL::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceI32",
                           std::bind(&DeviceI32::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceI32::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorI32",
                           std::bind(&DeviceVectorI32::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceVectorI32::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorI8",
                           std::bind(&DeviceVectorI8::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceVectorI8::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceVectorUI8",
                           std::bind(&DeviceVectorUI8::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceVectorUI8::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceDigitalIO",
                           std::bind(&DeviceDigitalIO::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceDigitalIO::deallocateDevice, std::placeholders::_1));


    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(new nds::tests::TestControlSystemFactoryImpl()));
    nds::Factory::registerControlSystem(testControlSystem);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
