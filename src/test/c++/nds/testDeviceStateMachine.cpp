#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDeviceStateMachine, stateMachine)
{
	const timespec* pDeviceStateMachineSwitchTime;
	const std::int32_t* pDeviceStateMachineState;
	const timespec* pStateMachineStateMachineSwitchTime;
	const std::int32_t* pStateMachineStateMachineState;
	timespec timestamp = {0, 0};

    //Create factory
    nds::Factory factory("test");

    // Create test device of type deviceStateMachine and name it deviceStateMachine
    factory.createDevice("DeviceStateMachine", "deviceStateMachine", nds::namedParameters_t());

    //Get instance of the Test Control System
    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceStateMachine");
    // Check StateMachine initial state (OFF)
    pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineStateMachineState);

	//Get the StateMachine global state
	std::int32_t globalStateStateMachine;
	pInterface->readCSValue("/deviceStateMachine-StateMachine.getGlobalState", &timestamp, &globalStateStateMachine);
	EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateStateMachine);

	//Change StateMachine state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/deviceStateMachine-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineStateMachineState);

	//Get the StateMachine global state
	pInterface->readCSValue("/deviceStateMachine-StateMachine.getGlobalState", &timestamp, &globalStateStateMachine);
	EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateStateMachine);

	//Change StateMachine state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/deviceStateMachine-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineStateMachineState);
	::sleep(2);

	pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineStateMachineState);
	::sleep(2);

	//Get the StateMachine global state
	pInterface->readCSValue("/deviceStateMachine-StateMachine.getGlobalState", &timestamp, &globalStateStateMachine);
	EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateStateMachine);

	//Change StateMachine state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/deviceStateMachine-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineStateMachineState);

	//Change StateMachine state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/deviceStateMachine-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/deviceStateMachine-StateMachine.getState", pStateMachineStateMachineSwitchTime, pStateMachineStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineStateMachineState);

    // Destroy test device
    factory.destroyDevice("deviceStateMachine");

}

