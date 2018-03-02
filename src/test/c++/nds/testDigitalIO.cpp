#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/Device.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDigitalIO, testStateMachine)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type Device and named rootNode
    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIONode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DigitalIONode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-DigitalIONode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-DigitalIONode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    // Destroy test device
    factory.destroyDevice("rootNode");

}

TEST(testDigitalIO, testPushData)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("Device", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get dataOutMask
	std::vector<bool> dataOutMaskIn = {1,1,0,1,0};
	std::vector<bool> dataOutMaskOut = {0,0,0,0,0};
	pInterface->writeCSValue("/rootNode-DigitalIONode.DataOutMask", timestamp, dataOutMaskIn);
	pInterface->readCSValue("/rootNode-DigitalIONode.DataOutMask_RBV",&readTimestamp,&dataOutMaskOut); // PVVariables are thread safe
	//TODO: How to check arrays in GoogleTest??
	for(int i=0; i<dataOutMaskOut.size();i++){
		EXPECT_EQ(dataOutMaskIn[i],dataOutMaskOut[i]);
	}

	// Set/Get voltLevelHigh
	double voltLevelHigh;
	pInterface->writeCSValue("/rootNode-DigitalIONode.VoltLevelHigh", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DigitalIONode.VoltLevelHigh_RBV",&readTimestamp,&voltLevelHigh); // PVVariables are thread safe
	EXPECT_EQ((double)0, voltLevelHigh);

	// Set/Get voltLevelLow
	double voltLevelLow;
	pInterface->writeCSValue("/rootNode-DigitalIONode.VoltLevelLow", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DigitalIONode.VoltLevelLow_RBV",&readTimestamp,&voltLevelLow); // PVVariables are thread safe
	EXPECT_EQ((double)0, voltLevelLow);

	// Set/Get channelDir
	std::vector<bool> channelDirIn = {1,0,1,0,1};
	std::vector<bool> channelDirOut = {0,0,0,0,0};
	pInterface->writeCSValue("/rootNode-DigitalIONode.ChannelDir", timestamp, channelDirIn);
	pInterface->readCSValue("/rootNode-DigitalIONode.ChannelDir_RBV",&readTimestamp,&channelDirOut); // PVVariables are thread safe
	for(int i=0; i<dataOutMaskOut.size();i++){
		EXPECT_EQ(channelDirIn[i],channelDirOut[i]);
	}

	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DigitalIONode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	std::int32_t startTimestamp = 200;
	pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DigitalIONode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DigitalIONode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DigitalIONode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DigitalIONode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DigitalIONode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

	//Initialize comparison vector
	std::vector<std::uint8_t> pushData(128);
	const std::vector<std::uint8_t>* pRetrievedPushedValues;
	const timespec* pTime;
	double readCount=0;
	size_t scanVector(0);
	try{
		while(readCount<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = readCount;
			}

			pInterface->getPushedVectorUint8("/rootNode-DigitalIONode.DataIn", pTime, pRetrievedPushedValues);
			++readCount;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cout << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
	}
	EXPECT_EQ(startTimestamp, pTime->tv_sec);
	EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	++startTimestamp;

	factory.destroyDevice("rootNode");

}
