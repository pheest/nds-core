#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"


TEST(testDeviceTiming, fullTest){

  const timespec* pDeviceStateMachineSwitchTime;
  const std::int32_t* pDeviceStateMachineState;
  const timespec* pTimingStateMachineSwitchTime;
  const std::int32_t* pTimingStateMachineState;
  timespec timestamp = {0, 0};

  //Create factory
  nds::Factory factory("test");

  // Create test device of type DeviceTiming and name it deviceTiming
  factory.createDevice("DeviceTiming", "deviceTiming", nds::namedParameters_t());

  //Get instance of the Test Control System
  nds::tests::TestControlSystemInterfaceImpl* pInterface =
    nds::tests::TestControlSystemInterfaceImpl::getInstance("deviceTiming");


  // Check TimingNode initial state (OFF)
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
              pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pTimingStateMachineState);

  //Get the Timing global state
  std::int32_t globalStateTiming;
  pInterface->readCSValue("/deviceTiming-Timing.StateMachine.getGlobalState",
      &timestamp, &globalStateTiming);
  EXPECT_EQ((std::int32_t)nds::state_t::off, globalStateTiming);


  //Change TimingNode state:  OFF -> (initializing) -> ON
  pInterface->writeCSValue("/deviceTiming-Timing.StateMachine.setState",
      timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pTimingStateMachineState);

  ::sleep(2);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pTimingStateMachineState);

  //Get the Timing global state
  pInterface->readCSValue("/deviceTiming-Timing.StateMachine.getGlobalState",
      &timestamp, &globalStateTiming);
  EXPECT_EQ((std::int32_t)nds::state_t::on, globalStateTiming);


  //Get time (UTC).
  timespec timeval = {0, 0};
  pInterface->readCSValue("/deviceTiming-Timing.Time", &timestamp, &timeval);
  EXPECT_EQ((time_t)1514764800, timeval.tv_sec);
  EXPECT_EQ((long int)20091982, timeval.tv_nsec);

  //Get human readable time (UTC).
  std::string htime;
  pInterface->readCSValue("/deviceTiming-Timing.HTime", &timestamp, &htime);
  EXPECT_EQ((const char*)"Mon Jan  1 00:00:00 2018\n", htime);

  //Get clock frequency (UTC).
  double ClkFreqVal;
  pInterface->readCSValue("/deviceTiming-Timing.ClkFrequency",
			  &timestamp, &ClkFreqVal);
  EXPECT_EQ((double)100.001, ClkFreqVal);

  //Get clock multiplier (UTC).
  int32_t ClkMultiplier;
  pInterface->readCSValue("/deviceTiming-Timing.ClkMultiplier",
			  &timestamp, &ClkMultiplier);
  EXPECT_EQ((int32_t)2, ClkMultiplier);

  //Get synchronizing status.
  int32_t syncStatVal = -1;
  pInterface->readCSValue("/deviceTiming-Timing.SyncStatus",
			  &timestamp, &syncStatVal);
  EXPECT_EQ((int32_t)1, syncStatVal);

  //Get seconds since last synchronization.
  int32_t secsLastSyncVal = -1;
  pInterface->readCSValue("/deviceTiming-Timing.SecsLastSync",
			  &timestamp, &secsLastSyncVal);
  EXPECT_EQ((int32_t)0, secsLastSyncVal);

  //Get reference base time.
  timeval.tv_sec = 0;
  timeval.tv_nsec = 0;
  pInterface->readCSValue("/deviceTiming-Timing.RefTimeBase",
			  &timestamp, &timeval);
  EXPECT_EQ((time_t)1514764800, timeval.tv_sec);
  EXPECT_EQ((long int)1514764810, timeval.tv_nsec);

  //Change TimingNode state: ON -> (starting) -> RUNNING
  pInterface->writeCSValue("/deviceTiming-Timing.StateMachine.setState",
      timestamp, (std::int32_t)nds::state_t::running);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::starting, *pTimingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::running, *pTimingStateMachineState);
  ::sleep(2);


  //Get the Timing global state
  pInterface->readCSValue("/deviceTiming-Timing.StateMachine.getGlobalState",
      &timestamp, &globalStateTiming);
  EXPECT_EQ((std::int32_t)nds::state_t::running, globalStateTiming);

  //Get Synchronization status.
  syncStatVal = -1;
  pInterface->readCSValue("/deviceTiming-Timing.SyncStatus",
			  &timestamp, &syncStatVal);
  EXPECT_EQ((int32_t)2, syncStatVal);


  //Change TimingNode state: RUNNING -> (stopping) -> ON
  pInterface->writeCSValue("/deviceTiming-Timing.StateMachine.setState",
      timestamp, (std::int32_t)nds::state_t::on);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pTimingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::on, *pTimingStateMachineState);

  //Change TimingNode state: ON -> (switchingOff) -> OFF
  pInterface->writeCSValue("/deviceTiming-Timing.StateMachine.setState",
      timestamp, (std::int32_t)nds::state_t::off);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pTimingStateMachineState);
  ::sleep(2);
  pInterface->getPushedInt32("/deviceTiming-Timing.StateMachine.getState",
      pTimingStateMachineSwitchTime, pTimingStateMachineState);
  EXPECT_EQ((std::int32_t)nds::state_t::off, *pTimingStateMachineState);

  //Get Syncronizing status.
  syncStatVal = -1;
  pInterface->readCSValue("/deviceTiming-Timing.SyncStatus",
			  &timestamp, &syncStatVal);
  EXPECT_EQ((int32_t)0, syncStatVal);

  //Get Syncronizing status.
  secsLastSyncVal = -1;
  pInterface->readCSValue("/deviceTiming-Timing.SecsLastSync",
			  &timestamp, &secsLastSyncVal);
  EXPECT_EQ((int32_t)10, secsLastSyncVal);

  // Destroy test device
  factory.destroyDevice("deviceTiming");

}
