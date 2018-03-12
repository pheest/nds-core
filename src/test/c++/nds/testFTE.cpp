#include <gtest/gtest.h>
#include <nds3/nds.h>

//#include <iostream>

#include "../include/Device.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testFTE, testStateMachineFTE)
{
    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type DeviceDigitalIO and named rootNode
    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-FTENode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-FTENode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

      // Destroy test device
    factory.destroyDevice("rootNode");

}

TEST(testFTE, testSetPVManaging)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("Device", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set the start time
	/////////////////////
	std::int32_t startTimestamp = 200; //TODO Study this
	pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

	// Set/Get TerminalSet
	std::int32_t terminalSet;
	pInterface->writeCSValue("/rootNode-FTENode.TerminalSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.TerminalSet",&readTimestamp,&terminalSet); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,terminalSet);

	// Set/Get ModeSet
	std::int32_t modeSet;
	pInterface->writeCSValue("/rootNode-FTENode.ModeSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.ModeSet",&readTimestamp,&modeSet); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,modeSet);

	// Set/Get LevelSet
	std::int32_t levelSet;
	pInterface->writeCSValue("/rootNode-FTENode.LevelSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.LevelSet",&readTimestamp,&levelSet); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,levelSet);

	// Set/Get StartTimeSet
	timespec startTimeSet;
	pInterface->writeCSValue("/rootNode-FTENode.StartTimeSet",readTimestamp,(timespec){1,1}); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.StartTimeSet",&readTimestamp,&startTimeSet); // PVVariables are thread safe
	EXPECT_EQ(1,startTimeSet.tv_sec);
	EXPECT_EQ(1,startTimeSet.tv_nsec);

	// Set/Get SopTimeSet
	timespec stopTimeSet;
	pInterface->writeCSValue("/rootNode-FTENode.StopTimeSet",readTimestamp,(timespec){1,1}); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.StopTimeSet",&readTimestamp,&stopTimeSet); // PVVariables are thread safe
	EXPECT_EQ(1,stopTimeSet.tv_sec);
	EXPECT_EQ(1,stopTimeSet.tv_nsec);

	// Set/Get PeriodNsecSet
	std::int32_t periodNsecSet;
	pInterface->writeCSValue("/rootNode-FTENode.PeriodNsecSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.PeriodNsecSet",&readTimestamp,&periodNsecSet); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,periodNsecSet);

	// Set/Get DutyCycleSet
	std::int32_t dutyCycleSet;
	pInterface->writeCSValue("/rootNode-FTENode.DutyCycleSet",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.DutyCycleSet",&readTimestamp,&dutyCycleSet); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,dutyCycleSet);

	std::int32_t set;
	std::string setStatus;
	std::int32_t setCode;
	////////////////////////////////////////////////////////////////
	///TEST NO Set
	////////////////////////////////////////////////////////////////
	// Set/Get SetStatus
	pInterface->writeCSValue("/rootNode-FTENode.Set",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.Set_RBV",&readTimestamp,&set); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0,set);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.SetStatus",&readTimestamp,&setStatus); // PVVariables are thread safe
	EXPECT_EQ((std::string)"OK",setStatus);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.SetCode",&readTimestamp,&setCode); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0,setCode);

	////////////////////////////////////////////////////////////////
	///TEST YES Set
	////////////////////////////////////////////////////////////////
	// Set/Get SetStatus
	pInterface->writeCSValue("/rootNode-FTENode.Set",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.Set_RBV",&readTimestamp,&set); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,set);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.SetStatus",&readTimestamp,&setStatus); // PVVariables are thread safe
	EXPECT_EQ((std::string)"OK",setStatus);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.SetCode",&readTimestamp,&setCode); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,setCode);

	factory.destroyDevice("rootNode");

}

TEST(testFTE, testSuppressPVManaging)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("Device", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set the start time
	/////////////////////
	std::int32_t startTimestamp = 200; //TODO Study this
	pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

	// Set/Get TerminalSet
	std::int32_t terminalSuppress;
	pInterface->writeCSValue("/rootNode-FTENode.TerminalSuppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.TerminalSuppress",&readTimestamp,&terminalSuppress); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,terminalSuppress);

	// Set/Get ModeSet
	std::int32_t modeSuppress;
	pInterface->writeCSValue("/rootNode-FTENode.ModeSuppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.ModeSuppress",&readTimestamp,&modeSuppress); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,modeSuppress);

	// Set/Get AllSuppress
	std::int32_t allSuppress;
	pInterface->writeCSValue("/rootNode-FTENode.AllSuppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.AllSuppress",&readTimestamp,&allSuppress); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,allSuppress);

	std::int32_t suppress;
	std::string suppressStatus;
	std::int32_t suppressCode;
	////////////////////////////////////////////////////////////////
	///TEST NO Suppress
	////////////////////////////////////////////////////////////////
	// Set/Get SuppressStatus
	pInterface->writeCSValue("/rootNode-FTENode.Suppress",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.Suppress_RBV",&readTimestamp,&suppress); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0,suppress);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.SuppressStatus",&readTimestamp,&suppressStatus); // PVVariables are thread safe
	EXPECT_EQ((std::string)"OK",suppressStatus);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.SuppressCode",&readTimestamp,&suppressCode); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0,suppressCode);

	////////////////////////////////////////////////////////////////
	///TEST YES Suppress
	////////////////////////////////////////////////////////////////
	// Set/Get SuppressStatus
	pInterface->writeCSValue("/rootNode-FTENode.Suppress",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.Suppress_RBV",&readTimestamp,&suppress); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,suppress);

	// Get SuppressCode
	pInterface->readCSValue("/rootNode-FTENode.SuppressStatus",&readTimestamp,&suppressStatus); // PVVariables are thread safe
	EXPECT_EQ((std::string)"OK",suppressStatus);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.SuppressCode",&readTimestamp,&suppressCode); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,suppressCode);

	factory.destroyDevice("rootNode");

}

TEST(testFTE, testChgPeriodPVManaging)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("Device", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set the start time
	/////////////////////
	std::int32_t startTimestamp = 200; //TODO Study this
	pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

	// Set/Get TerminalSet
	std::int32_t terminalChgPeriod;
	pInterface->writeCSValue("/rootNode-FTENode.TerminalChgPeriod",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.TerminalChgPeriod",&readTimestamp,&terminalChgPeriod); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,terminalChgPeriod);

	// Set/Get ModeSet
	std::int32_t periodChgPeriod;
	pInterface->writeCSValue("/rootNode-FTENode.PeriodChgPeriod",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.PeriodChgPeriod",&readTimestamp,&periodChgPeriod); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,periodChgPeriod);

	std::int32_t ChgPeriod;
	std::string ChgPeriodStatus;
	std::int32_t ChgPeriodCode;
	////////////////////////////////////////////////////////////////
	///TEST NO ChgPeriod
	////////////////////////////////////////////////////////////////
	// Set/Get ChgPeriodStatus
	pInterface->writeCSValue("/rootNode-FTENode.ChgPeriod",readTimestamp,(std::int32_t)0); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.ChgPeriod_RBV",&readTimestamp,&ChgPeriod); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0,ChgPeriod);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.ChgPeriodStatus",&readTimestamp,&ChgPeriodStatus); // PVVariables are thread safe
	EXPECT_EQ((std::string)"OK",ChgPeriodStatus);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.ChgPeriodCode",&readTimestamp,&ChgPeriodCode); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0,ChgPeriodCode);

	////////////////////////////////////////////////////////////////
	///TEST YES ChgPeriod
	////////////////////////////////////////////////////////////////
	// Set/Get ChgPeriodStatus
	pInterface->writeCSValue("/rootNode-FTENode.ChgPeriod",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.ChgPeriod_RBV",&readTimestamp,&ChgPeriod); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,ChgPeriod);

	// Get ChgPeriodCode
	pInterface->readCSValue("/rootNode-FTENode.ChgPeriodStatus",&readTimestamp,&ChgPeriodStatus); // PVVariables are thread safe
	EXPECT_EQ((std::string)"OK",ChgPeriodStatus);

	// Get SetCode
	pInterface->readCSValue("/rootNode-FTENode.ChgPeriodCode",&readTimestamp,&ChgPeriodCode); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,ChgPeriodCode);

	factory.destroyDevice("rootNode");

}

TEST(testFTE, testPendingAndMaximumPVManaging)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("Device", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set the start time
	/////////////////////
	std::int32_t startTimestamp = 200; //TODO Study this
	pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

	// Pending Value Testing
	std::int32_t pendingValue;
	pInterface->writeCSValue("/rootNode-FTENode.TerminalPending",readTimestamp,(std::int32_t)1); // PVVariables are thread safe
	pInterface->readCSValue("/rootNode-FTENode.PendingValue",&readTimestamp,&pendingValue); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1,pendingValue);

    // Check initial state (OFF)
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

    //Change state:  OFF -> (initializing) -> ON
    pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Maximum Value Testing
	std::int32_t maximum;
	pInterface->readCSValue("/rootNode-FTENode.Maximum",&readTimestamp,&maximum); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)20,maximum);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    //Change state:  RUNNING -> (stopping) -> ON
    pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

    //Change state:  ON -> (switchingOff) -> OFF
    pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	factory.destroyDevice("rootNode");

}


