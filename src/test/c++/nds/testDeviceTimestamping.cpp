#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDeviceTimestamping, StateMachineTest){

  const timespec* pTimestampingStateMachineSwitchTime;
  const std::int32_t* pTimestampingStateMachineState;
  timespec timestamp = {0, 0};

  //Create factory
  nds::Factory factory("test");

  // Create test device of type DeviceTimestamping and name it deviceTimestamping
  factory.createDevice("DeviceTimestamping",
		       "deviceTimestamping",
		       nds::namedParameters_t());

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface =
    nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceTimestamping");

  // Check TimestampingNode initial state (OFF)
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
  			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pTimestampingStateMachineState);

  // Get the Timestamping global state
  std::int32_t globalStateTimestamping = -1;
  pInterface->readCSValue("/deviceTimestamping-Timestamping.StateMachine.getGlobalState",
			  &timestamp, &globalStateTimestamping);
  EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateTimestamping);


  // Change TimestampingNode state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
			   timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pTimestampingStateMachineState);

  ::sleep(2);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pTimestampingStateMachineState);

  // Get the Timestamping global state
  pInterface->readCSValue("/deviceTimestamping-Timestamping.StateMachine.getGlobalState",
			  &timestamp, &globalStateTimestamping);
  EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateTimestamping);

  //Change TimestampingNode state: ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
			   timestamp, (std::int32_t)nds::state_t::running);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::starting, *pTimestampingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::running, *pTimestampingStateMachineState);
  ::sleep(2);

  // Get the Timestamping global state
  pInterface->readCSValue("/deviceTimestamping-Timestamping.StateMachine.getGlobalState",
			  &timestamp, &globalStateTimestamping);
  EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateTimestamping);

  // Change TimestampingNode state: RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
			   timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pTimestampingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pTimestampingStateMachineState);

  // Change TimestampingNode state: ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
			   timestamp, (std::int32_t)nds::state_t::off);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pTimestampingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.StateMachine.getState",
			     pTimestampingStateMachineSwitchTime,
			     pTimestampingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pTimestampingStateMachineState);


  // Destroy test device
  factory.destroyDevice("deviceTimestamping");

}



TEST(testDeviceTimestamping, SetGetTest){

  const timespec* ptimestamp = NULL;
  timespec timestamp = {0, 0};

  //Create factory
  nds::Factory factory("test");

  // Create test device of type DeviceTimestamping and name it deviceTimestamping
  factory.createDevice("DeviceTimestamping",
		       "deviceTimestamping",
		       nds::namedParameters_t());

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface =
    nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceTimestamping");


  // Change TimestampingNode state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
			   timestamp, (std::int32_t)nds::state_t::on);
  ::sleep(2);

  // Get enable status.
  const std::int32_t * enable_val = NULL;
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Enable_RBV",
			     ptimestamp, enable_val);
  EXPECT_EQ((std::int32_t)0 /* OFF */, *enable_val);

  // Get edge value.
  const std::int32_t * edge_val = NULL;
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Edge_RBV",
			     ptimestamp, edge_val);
  EXPECT_EQ((std::int32_t)1 /* FALLING */, *edge_val);

  // Get maximum number of timestamps.
  const std::int32_t * max_tstamp = NULL;
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.MaxTimestamps",
			     ptimestamp, max_tstamp);
  EXPECT_EQ((std::int32_t)5, *max_tstamp);

  //Get overflow state.
  const std::int32_t * overflow_val = NULL;
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Overflow",
			     ptimestamp, overflow_val);
  EXPECT_EQ((std::int32_t)0, *overflow_val);

  //Change TimestampingNode state: ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
			   timestamp, (std::int32_t)nds::state_t::running);
  ::sleep(2);

  // Set Edge to ANY
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.Edge",
			   timestamp, (std::int32_t)2);

  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Edge_RBV",
			     ptimestamp, edge_val);
  EXPECT_EQ((std::int32_t)2 /* ANY */, *edge_val);

  // Set Enable to ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.Enable",
			   timestamp, (std::int32_t)1);

  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Enable_RBV",
			     ptimestamp, enable_val);
  EXPECT_EQ((std::int32_t)1 /* ON */, *enable_val);
  ::sleep(4);

  // Testing values of the timestamps pushed.
  const nds::timestamp_t * pushed_timestamp;
  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
				   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)true, pushed_timestamp->rising);
  EXPECT_EQ((std::int32_t)1, pushed_timestamp->id);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
				   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)true, pushed_timestamp->rising);
  EXPECT_EQ((std::int32_t)2, pushed_timestamp->id);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
				   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)true, pushed_timestamp->rising);
  EXPECT_EQ((std::int32_t)3, pushed_timestamp->id);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
				   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)false, pushed_timestamp->rising);
  EXPECT_EQ((std::int32_t)4, pushed_timestamp->id);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
				   ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)false, pushed_timestamp->rising);
  EXPECT_EQ((std::int32_t)5, pushed_timestamp->id);

  // Get overflow state.
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Overflow",
			     ptimestamp, overflow_val);
  EXPECT_EQ((std::int32_t)2 /* FULL */, *overflow_val);

  pInterface->getPushedTimestamp("/deviceTimestamping-Timestamping.Timestamps",
				 ptimestamp, pushed_timestamp);
  EXPECT_EQ((std::int32_t)0, pushed_timestamp->timestamp.tv_sec);
  EXPECT_EQ((std::int32_t)10, pushed_timestamp->timestamp.tv_nsec);
  EXPECT_EQ((bool)true, pushed_timestamp->rising);
  EXPECT_EQ((std::int32_t)6, pushed_timestamp->id);

  // Get overflow state.
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Overflow",
			     ptimestamp, overflow_val);
  EXPECT_EQ((std::int32_t)1 /* OVERFLOWED */, *overflow_val);

  // Clear overflow
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.ClearOverflow",
			   timestamp, (std::int32_t)2736);
			   /*TODO: this function should not accept any value. */
  ::sleep(2);

  // Get overflow state.
  pInterface->getPushedInt32("/deviceTimestamping-Timestamping.Overflow",
			     ptimestamp, overflow_val);
  EXPECT_EQ((std::int32_t)0 /* NO OVERFLOW */, *overflow_val);


  // Change TimestampingNode state: RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
			   timestamp, (std::int32_t)nds::state_t::on);
  ::sleep(2);

  // Change TimestampingNode state: ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceTimestamping-Timestamping.StateMachine.setState",
			   timestamp, (std::int32_t)nds::state_t::off);
  ::sleep(2);

  // Destroy test device
  factory.destroyDevice("deviceTimestamping");

}
