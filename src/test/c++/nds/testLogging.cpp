#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <functional>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"
#include <unistd.h>
#include <thread>

#include "../include/Device_Vector_DBL.h"


void logPV(std::vector<nds::PVBase>& pvs, nds::logLevel_t severity)
	{
	    for(size_t scanPVs(0); scanPVs != pvs.size(); ++scanPVs)
	    {
	        switch(severity)
	        {
	        case nds::logLevel_t::debug:
	            ndsDebugStream(pvs.at(scanPVs)) << "Debug string" << scanPVs << std::endl;
	            break;
	        case nds::logLevel_t::info:
	            ndsInfoStream(pvs.at(scanPVs)) << "Info string" << scanPVs << std::endl;
	            break;
	        case nds::logLevel_t::warning:
	            ndsWarningStream(pvs.at(scanPVs)) << "Warning string" << scanPVs << std::endl;
	            break;
	        case nds::logLevel_t::error:
	            ndsErrorStream(pvs.at(scanPVs)) << "Error string" << scanPVs << std::endl;
	            break;
	        case nds::logLevel_t::none:
	            break;
	        }
	    }
	}

TEST(testLogging, testLotOfPVs)
{

	nds::Port rootNode("testLogging");

    std::vector<nds::PVBase> pvs;

    for(size_t allocatePV(0); allocatePV != 1000; ++allocatePV)
    {
        std::ostringstream name;
        name << "PV" << allocatePV;
        nds::PVVariableIn<std::int32_t> newPV(name.str());
        pvs.push_back(newPV);
        rootNode.addChild(newPV);

        if(allocatePV < 200)
        {
            newPV.setLogLevel(nds::logLevel_t::debug);
        }
        else if(allocatePV < 400)
        {
            newPV.setLogLevel(nds::logLevel_t::info);
        }
        else if(allocatePV < 600)
        {
            newPV.setLogLevel(nds::logLevel_t::warning);
        }
        else if(allocatePV < 800)
        {
            newPV.setLogLevel(nds::logLevel_t::error);
        }
        else
        {
            newPV.setLogLevel(nds::logLevel_t::none);
        }
    }

    nds::Factory factory("test");

    rootNode.initialize(0, factory);


    std::thread logDebug(std::bind(&logPV, std::ref(pvs), nds::logLevel_t::debug));
    std::thread logInfo(std::bind(&logPV, std::ref(pvs), nds::logLevel_t::info));
    std::thread logWarning(std::bind(&logPV, std::ref(pvs), nds::logLevel_t::warning));
    std::thread logError(std::bind(&logPV, std::ref(pvs), nds::logLevel_t::error));



    logDebug.join();
    logInfo.join();
    logWarning.join();
    logError.join();

    nds::tests::TestControlSystemFactoryImpl* pFactory = nds::tests::TestControlSystemFactoryImpl::getInstance();

    for(size_t scanPVs(0); scanPVs != pvs.size(); ++scanPVs)
    {
        std::ostringstream debugString;
        debugString << "Debug string" << scanPVs << std::endl;

        std::ostringstream infoString;
        infoString << "Info string" << scanPVs << std::endl;

        std::ostringstream warningString;
        warningString << "Warning string" << scanPVs << std::endl;

        std::ostringstream errorString;
        errorString << "Error string" << scanPVs << std::endl;

        EXPECT_EQ(scanPVs < 200 ? 1 : 0, pFactory->countStringInLog(debugString.str()));

        EXPECT_EQ(scanPVs < 400 ? 1 : 0, pFactory->countStringInLog(infoString.str()));

        EXPECT_EQ(scanPVs < 600 ? 1 : 0, pFactory->countStringInLog(warningString.str()));

        EXPECT_EQ(scanPVs < 800 ? 1 : 0, pFactory->countStringInLog(errorString.str()));
    }

    factory.destroyDevice("");

    EXPECT_EQ(0, pFactory->getRegisteredCommandsNumber());

}
