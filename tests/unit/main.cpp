// imebra_tests.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <vector>
#include <string>
#include <iostream>

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

void tryLoadDriver(const std::string& name)
{
    std::string prefix = "lib";
    std::string suffix = ".so";
#ifdef _WIN32
    prefix = "";
    suffix = ".dll";
#endif

    std::vector<std::string> searchPaths = {
        "../../examples/nodesTest/" + prefix + "nds3-" + name + suffix,
        "../../../lib/" + prefix + "nds3-" + name + suffix,
        prefix + "nds3-" + name + suffix
    };

#ifdef _WIN32
    // Also try with 'lib' prefix on Windows
    searchPaths.push_back("../../examples/nodesTest/libnds3-" + name + suffix);
    searchPaths.push_back("../../../lib/libnds3-" + name + suffix);
    searchPaths.push_back("libnds3-" + name + suffix);
#endif

    bool loaded = false;
    for (const auto& path : searchPaths)
    {
        try
        {
            nds::FactoryBaseImpl::loadDriver(path);
            loaded = true;
            break;
        }
        catch (...)
        {
            // Try next path
        }
    }
    if (!loaded)
    {
        // If not loaded by any of the searched paths, try the original one to report error
        try
        {
            nds::FactoryBaseImpl::loadDriver("../../../lib/libnds3-" + name + ".so");
        }
        catch (const std::exception& e)
        {
            std::cerr << "Failed to load driver " << name << ": " << e.what() << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    nds::Factory::registerDriver("Device",
                           std::bind(&DeviceBasic::allocateDevice, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                           std::bind(&DeviceBasic::deallocateDevice, std::placeholders::_1));

    tryLoadDriver("DeviceFloat");
    tryLoadDriver("DeviceDBL");
    tryLoadDriver("DeviceI32");
    tryLoadDriver("DeviceVectorI32");
    tryLoadDriver("DeviceI64");
    tryLoadDriver("DeviceVectorI64");
    tryLoadDriver("DeviceVectorI8");
    tryLoadDriver("DeviceVectorUI8");
    tryLoadDriver("DeviceVectorFloat");
    tryLoadDriver("DeviceVectorDBL");
    tryLoadDriver("DeviceDigitalIO");
    tryLoadDriver("DeviceFTE");
    tryLoadDriver("DeviceRouting");
    tryLoadDriver("DeviceHQMonitor");
    tryLoadDriver("DeviceTiming");
    tryLoadDriver("DeviceTimestamping");
    tryLoadDriver("DeviceStateMachine");
    tryLoadDriver("DeviceFirmware");
    tryLoadDriver("DevicePVs");
    tryLoadDriver("DeviceTrigAndClk");
    tryLoadDriver("DeviceDataMultiplexing");
    tryLoadDriver("DeviceError");

    nds::Factory testControlSystem(std::shared_ptr<nds::FactoryBaseImpl>(new nds::tests::TestControlSystemFactoryImpl()));
    nds::Factory::registerControlSystem(testControlSystem);

    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "testDeviceDataMultiplexing*";
    return RUN_ALL_TESTS();
}
