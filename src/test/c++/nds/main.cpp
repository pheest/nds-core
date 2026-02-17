// imebra_tests.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "DeviceBasic.h"
#include "DeviceFloat.h"
#include "DeviceDBL.h"
#include "DeviceDigitalIO.h"
#include "DeviceFTE.h"
#include "DeviceI32.h"
#include "DeviceI64.h"
#include "DeviceRouting.h"
#include "DeviceVectorI32.h"
#include "DeviceVectorI64.h"
#include "DeviceVectorI8.h"
#include "DeviceVectorUI8.h"
#include "DeviceVectorFloat.h"
#include "DeviceVectorDBL.h"
#include "DeviceHQMonitor.h"
#include "DeviceStateMachine.h"
#include "DeviceFirmware.h"
#include "DeviceTiming.h"
#include "DeviceTimestamping.h"
#include "DevicePVs.h"
#include "DeviceTrigAndClk.h"
#include "DeviceDataMultiplexing.h"
#include "DeviceError.h"

#include "nds3/ndsTestFactory.h"


int main(int argc, char **argv)
{
    nds::Factory::registerDriver("Device",
                           std::bind(&DeviceBasic::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceBasic::deallocateDevice, std::placeholders::_1));

    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceFloat.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceDBL.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceI32.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceVectorI32.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceI64.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceVectorI64.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceVectorI8.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceVectorUI8.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceVectorFloat.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceVectorDBL.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceDigitalIO.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceFTE.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceRouting.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceHQMonitor.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceTiming.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceTimestamping.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceStateMachine.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceFirmware.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DevicePVs.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceTrigAndClk.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceDataMultiplexing.so");
    nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-DeviceError.so");

    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(new nds::tests::TestControlSystemFactoryImpl()));
    nds::Factory::registerControlSystem(testControlSystem);


    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "testDeviceDataMultiplexing*";
    return RUN_ALL_TESTS();
}
