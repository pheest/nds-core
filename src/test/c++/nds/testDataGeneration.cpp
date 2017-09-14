#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/testDevice.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDataGeneration, testStateMachine)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type testdevice and named rootNode
    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    //This state machine is Asynchronous.

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    // Destroy test device
    factory.destroyDevice("rootNode");

}



//TEST(testDataGeneration, testPushDataGenerated)
//{
//    nds::Factory factory("test");
//
//    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());
//
//    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");
//
//    const timespec* pStateMachineSwitchTime;
//    const std::int32_t* pStateMachineState;
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
//
//    timespec timestamp = {0, 0};
//    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
//
//    // Wait for the switch on state (it should take one second)
//    ///////////////////////////////////////////////////////////
//    ::sleep(2);
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);
//
//    // Set the start time
//    /////////////////////
//    //std::int32_t startTimestamp = 200;
//    //pInterface->writeCSValue("/rootNode-DataGenerationNode.setCurrentTime", timestamp, startTimestamp);
//
//    // Start the data acquisition
//    /////////////////////////////
//    //pInterface->writeCSValue("/rootNode-DataGenerationNode.numAcquisitions", timestamp, (std::int32_t)100);
//    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
//    ::sleep(1);
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState); // Retrieve the starting state
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
//
//    ::sleep(1);
//
//    // Initialize comparison vectors
////    std::vector<double> pushData0(128);
////    std::vector<double> pushData1(128);
////    for(size_t initializeVectors(0); initializeVectors != 128; ++initializeVectors)
////    {
////        pushData0[initializeVectors] = initializeVectors;
////        pushData1[initializeVectors] = 128 - initializeVectors;
////    }
////
////    const std::vector<double>* pRetrievedPushedValues;
////    const timespec* pTime;
////    for(size_t numAcquisitions(0); numAcquisitions != 100; ++numAcquisitions)
////    {
////        pInterface->getPushedVectorInt32("/rootNode-DataGenerationNode.Data", pTime, pRetrievedPushedValues);
////        EXPECT_EQ(startTimestamp, pTime->tv_sec);
////        EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
////        ++startTimestamp;
////
////        if((numAcquisitions & 1) == 0)
////        {
////            ASSERT_EQ(pushData0.size(), pRetrievedPushedValues->size());
////            for(size_t compare(0); compare != pushData0.size(); ++compare)
////            {
////                EXPECT_EQ(pushData0[compare], (*pRetrievedPushedValues)[compare]);
////            }
////        }
////        else
////        {
////            ASSERT_EQ(pushData1.size(), pRetrievedPushedValues->size());
////            for(size_t compare(0); compare != pushData1.size(); ++compare)
////            {
////                EXPECT_EQ(pushData1[compare], (*pRetrievedPushedValues)[compare]);
////            }
////        }
////    }
//
//    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
//
//    factory.destroyDevice("rootNode");
//
//}
//
//TEST(testDataGeneration, testDecimation)
//{
//    nds::Factory factory("test");
//
//    factory.createDevice("testDevice", "rootNode", nds::namedParameters_t());
//
//    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");
//
//    const timespec* pStateMachineSwitchTime;
//    const std::int32_t* pStateMachineState;
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
//
//    timespec timestamp = {0, 0};
//    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
//
//    // Wait for the switch on state (it should take one second)
//    ///////////////////////////////////////////////////////////
//    ::sleep(2);
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);
//
//    // Start the data acquisition
//    /////////////////////////////
////    pInterface->writeCSValue("/rootNode-DataGenerationNode.decimation", timestamp, (std::int32_t)2);
////    pInterface->writeCSValue("/rootNode-DataGenerationNode.numAcquisitions", timestamp, (std::int32_t)100);
////
////    // Start the acquisition via node command
////    /////////////////////////////////////////
//      nds::parameters_t emptyParameters;
//      nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("start", "rootNode-DataGenerationNode", emptyParameters);
//      ::sleep(1);
//      pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState); // Retrieve the starting state
//      pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//      EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
////
//      ::sleep(1);
////
////    // Initialize comparison vector
////    std::vector<double> pushData(128);
////    for(size_t initializeVectors(0); initializeVectors != 128; ++initializeVectors)
////    {
////        pushData[initializeVectors] = 128- initializeVectors;
////    }
////
////    const std::vector<double>* pRetrievedPushedValues;
////    const timespec* pTime;
////    for(size_t numAcquisitions(0); numAcquisitions != 100; ++numAcquisitions)
////    {
////        if((numAcquisitions & 1) == 1)
////        {
////            pInterface->getPushedVectorInt32("/rootNode-DataGenerationNode.Data", pTime, pRetrievedPushedValues);
////            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
////            for(size_t compare(0); compare != pushData.size(); ++compare)
////            {
////                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
////            }
////        }
////    }
//
//    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
//    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
//
//    factory.destroyDevice("rootNode");
//
//}

