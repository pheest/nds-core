// imebra_tests.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/DeviceDBL.h"
#include "../include/DeviceDigitalIO.h"
#include "../include/DeviceFTE.h"
#include "../include/DeviceI32.h"
#include "../include/DeviceRouting.h"
#include "../include/DeviceVectorI32.h"
#include "../include/DeviceVectorI8.h"
#include "../include/DeviceVectorUI8.h"
#include "../include/Device.h"
#include "../include/DeviceHQMonitor.h"
#include "../include/DeviceStateMachine.h"
#include "../include/DeviceFirmware.h"
#include "../include/DeviceTiming.h"
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

    nds::Factory::registerDriver("DeviceFTE",
                           std::bind(&DeviceFTE::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceFTE::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceRouting",
                               std::bind(&DeviceRouting::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                               std::bind(&DeviceRouting::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceHQMonitor",
                           std::bind(&DeviceHQMonitor::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceHQMonitor::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceTiming",
                           std::bind(&DeviceTiming::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceTiming::deallocateDevice, std::placeholders::_1));

    nds::Factory::registerDriver("DeviceStateMachine",
                           std::bind(&DeviceStateMachine::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceStateMachine::deallocateDevice, std::placeholders::_1));



    //Devices which have been created for testing isolated nodes

    nds::Factory::registerDriver("DeviceFirmware",
                           std::bind(&DeviceFirmware::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceFirmware::deallocateDevice, std::placeholders::_1));



    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(new nds::tests::TestControlSystemFactoryImpl()));
    nds::Factory::registerControlSystem(testControlSystem);


    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
