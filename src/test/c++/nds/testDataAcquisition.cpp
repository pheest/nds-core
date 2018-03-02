
#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/Device.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDataAcquisition, testStateMachine)
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

	//This state machine is Asynchronous.

	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
	::sleep(2);

	//TODO: Check generated data

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	// Destroy test device
	factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredVDBL)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("Device", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get Gain
	std::vector<double> Gain;
	std::vector<double> Gain_in;
	Gain_in.push_back(10.0);
	Gain_in.push_back(20.0);

	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, Gain_in);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ(10.0, Gain[0]);
	EXPECT_EQ(20.0, Gain[1]);

	// Set/Get offset
	double offset;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

	// Set/Get Bandwidth
	double Bandwidth;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
	EXPECT_EQ((double)0, Bandwidth);

	// Set/Get Resolution
	double Resolution;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
	EXPECT_EQ((double)0, Resolution);

	// Set/Get Impedance
	std::int32_t Impedance;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Impedance);

	// Set/Get Coupling
	std::int32_t Coupling;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Coupling);

	// Set/Get SignalRef
	std::int32_t SignalRef;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, SignalRef);

	// Set/Get Ground
	std::int32_t Ground;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Ground);


	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	std::int32_t startTimestamp = 200;
	pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

	//Initialize comparison vector
	std::vector<double> pushData(128);
	const std::vector<double>* pRetrievedPushedValues;
	const timespec* pTime;
	double valueData=0;
	std::int32_t pushCounter=0;
	size_t scanVector(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = valueData;
			}

			pInterface->getPushedVectorDouble("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
			++pushCounter;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}
			++valueData;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}
	EXPECT_EQ(startTimestamp, pTime->tv_sec);
	EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	++startTimestamp;

	factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredVI32)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("DeviceVectorI32", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get Gain
	std::vector<double> Gain;
	std::vector<double> Gain_in;
	Gain_in.push_back(10.0);
	Gain_in.push_back(20.0);

	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, Gain_in);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ(10.0, Gain[0]);
	EXPECT_EQ(20.0, Gain[1]);

	// Set/Get offset
	double offset;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

	// Set/Get Bandwidth
	double Bandwidth;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
	EXPECT_EQ((double)0, Bandwidth);

	// Set/Get Resolution
	double Resolution;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
	EXPECT_EQ((double)0, Resolution);

	// Set/Get Impedance
	std::int32_t Impedance;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Impedance);

	// Set/Get Coupling
	std::int32_t Coupling;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Coupling);

	// Set/Get SignalRef
	std::int32_t SignalRef;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, SignalRef);

	// Set/Get Ground
	std::int32_t Ground;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Ground);


	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

	//Initialize comparison vector
	std::vector<std::int32_t> pushData(128);
	const std::vector<std::int32_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t valueData=0;
	std::int32_t pushCounter=0;

	size_t scanVector(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = valueData;
			}

			pInterface->getPushedVectorInt32("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
			++pushCounter;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}
			++valueData;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

	factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredVI8)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("DeviceVectorI8", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get Gain
	std::vector<double> Gain;
	std::vector<double> Gain_in;
	Gain_in.push_back(10.0);
	Gain_in.push_back(20.0);

	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, Gain_in);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ(10.0, Gain[0]);
	EXPECT_EQ(20.0, Gain[1]);

	// Set/Get offset
	double offset;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

	// Set/Get Bandwidth
	double Bandwidth;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
	EXPECT_EQ((double)0, Bandwidth);

	// Set/Get Resolution
	double Resolution;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
	EXPECT_EQ((double)0, Resolution);

	// Set/Get Impedance
	std::int32_t Impedance;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Impedance);

	// Set/Get Coupling
	std::int32_t Coupling;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Coupling);

	// Set/Get SignalRef
	std::int32_t SignalRef;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, SignalRef);

	// Set/Get Ground
	std::int32_t Ground;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Ground);


	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

	//Initialize comparison vector
	std::vector<std::int8_t> pushData(128);
	const std::vector<std::int8_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int8_t valueData=0;
	std::int32_t pushCounter=0;

	size_t scanVector(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = valueData;
			}

			pInterface->getPushedVectorInt8("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
			++pushCounter;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}
			++valueData;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

	factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredVUI8)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("DeviceVectorUI8", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get Gain
	std::vector<double> Gain;
	std::vector<double> Gain_in;
	Gain_in.push_back(10.0);
	Gain_in.push_back(20.0);

	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, Gain_in);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ(10.0, Gain[0]);
	EXPECT_EQ(20.0, Gain[1]);

	// Set/Get offset
	double offset;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

	// Set/Get Bandwidth
	double Bandwidth;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
	EXPECT_EQ((double)0, Bandwidth);

	// Set/Get Resolution
	double Resolution;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
	EXPECT_EQ((double)0, Resolution);

	// Set/Get Impedance
	std::int32_t Impedance;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Impedance);

	// Set/Get Coupling
	std::int32_t Coupling;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Coupling);

	// Set/Get SignalRef
	std::int32_t SignalRef;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, SignalRef);

	// Set/Get Ground
	std::int32_t Ground;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Ground);


	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

	//Initialize comparison vector
	std::vector<std::uint8_t> pushData(128);
	const std::vector<std::uint8_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::uint8_t valueData=0;
	std::int32_t pushCounter=0;

	size_t scanVector(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = valueData;
			}

			pInterface->getPushedVectorUint8("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
			++pushCounter;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}
			++valueData;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

	factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredDBL)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("DeviceDBL", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get Gain
	std::vector<double> Gain;
	std::vector<double> Gain_in;
	Gain_in.push_back(10.0);
	Gain_in.push_back(20.0);

	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, Gain_in);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ(10.0, Gain[0]);
	EXPECT_EQ(20.0, Gain[1]);

	// Set/Get offset
	double offset;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

	// Set/Get Bandwidth
	double Bandwidth;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
	EXPECT_EQ((double)0, Bandwidth);

	// Set/Get Resolution
	double Resolution;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
	EXPECT_EQ((double)0, Resolution);

	// Set/Get Impedance
	std::int32_t Impedance;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Impedance);

	// Set/Get Coupling
	std::int32_t Coupling;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Coupling);

	// Set/Get SignalRef
	std::int32_t SignalRef;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, SignalRef);

	// Set/Get Ground
	std::int32_t Ground;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Ground);


	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

	//Initialize comparison vector
	double pushData(0);
	const double* pRetrievedPushedValues;
	const timespec* pTime;
	double valueData=0;
	std::int32_t pushCounter=0;


	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			pushData = valueData;
			pInterface->getPushedDouble("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
			++pushCounter;
			EXPECT_EQ(pushData, (*pRetrievedPushedValues));
			++valueData;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

	factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testPushDataAcquiredI32)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("DeviceI32", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get Gain
	std::vector<double> Gain;
	std::vector<double> Gain_in;
	Gain_in.push_back(10.0);
	Gain_in.push_back(20.0);

	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, Gain_in);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ(10.0, Gain[0]);
	EXPECT_EQ(20.0, Gain[1]);

	// Set/Get offset
	double offset;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

	// Set/Get Bandwidth
	double Bandwidth;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
	EXPECT_EQ((double)0, Bandwidth);

	// Set/Get Resolution
	double Resolution;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
	EXPECT_EQ((double)0, Resolution);

	// Set/Get Impedance
	std::int32_t Impedance;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Impedance);

	// Set/Get Coupling
	std::int32_t Coupling;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Coupling);

	// Set/Get SignalRef
	std::int32_t SignalRef;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, SignalRef);

	// Set/Get Ground
	std::int32_t Ground;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Ground);


	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

	//Initialize comparison vector
	std::int32_t pushData(0);
	const std::int32_t* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t valueData=0;
	std::int32_t pushCounter=0;

	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			pushData = valueData;

			pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
			++pushCounter;
			EXPECT_EQ(pushData, (*pRetrievedPushedValues));
			++valueData;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

	factory.destroyDevice("rootNode");

}

TEST(testDataAcquisition, testDecimation)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("Device", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get Gain
	std::vector<double> Gain;
	std::vector<double> Gain_in;
	Gain_in.push_back(10.0);
	Gain_in.push_back(20.0);

	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, Gain_in);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ(10.0, Gain[0]);
	EXPECT_EQ(20.0, Gain[1]);

	// Set/Get offset
	double offset;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Offset", timestamp, (double)1);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe

	// Set/Get Bandwidth
	double Bandwidth;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.BandWidth", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
	EXPECT_EQ((double)0, Bandwidth);

	// Set/Get Resolution
	double Resolution;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Resolution", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
	EXPECT_EQ((double)0, Resolution);

	// Set/Get Impedance
	std::int32_t Impedance;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Impedance", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Impedance);

	// Set/Get Coupling
	std::int32_t Coupling;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Coupling", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Coupling);

	// Set/Get SignalRef
	std::int32_t SignalRef;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.SignalRefType", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, SignalRef);

	// Set/Get Ground
	std::int32_t Ground;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Ground", timestamp, (std::int32_t)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, Ground);

	// Set/Get Ground
	std::int32_t decimation;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Decimation", timestamp, (std::int32_t)10);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Decimation",&readTimestamp,&decimation); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)10, decimation);

	// Check initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	std::int32_t startTimestamp = 200;
	pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

	//Initialize comparison vector
	std::vector<double> pushData(128);
	const std::vector<double>* pRetrievedPushedValues;
	const timespec* pTime;
	double valueData=0;
	std::int32_t pushCounter=0;
	std::int32_t decimationCounter=0;
	size_t scanVector(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){
			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = valueData;
			}
			++valueData;
			if((std::int32_t)(decimationCounter+1) % decimation ==0){
				pInterface->getPushedVectorDouble("/rootNode-DataAcquisitionNode.Data", pTime, pRetrievedPushedValues);
				++pushCounter;
				ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
				for(size_t compare(0); compare != pushData.size(); ++compare)
				{
					EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
				}
			}
			++decimationCounter;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cout << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}
	EXPECT_EQ(startTimestamp, pTime->tv_sec);
	EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	++startTimestamp;

	factory.destroyDevice("rootNode");


}

TEST(testDataAcquisition, testDMAParameters)
{

	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0}, readTimestamp{0,0};

	nds::Factory factory("test");

	factory.createDevice("Device", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Check Device initial state (OFF)
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change Device state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(10);
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Get DMABufferSize initial value
	double DMABufferSize;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMABufferSize",&readTimestamp,&DMABufferSize); // PVVariables are thread safe
	EXPECT_EQ((double)4194304, DMABufferSize);

	// Get DMANumChannels initial value
	std::int32_t DMANumChannels;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMANumChannels",&readTimestamp,&DMANumChannels); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)4, DMANumChannels);

	// Get DMAFrameType initial value
	std::int32_t DMAFrameType;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAFrameType",&readTimestamp,&DMAFrameType); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1, DMAFrameType);

	// Get DMASampleSize initial value
	std::int32_t DMASampleSize;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMASampleSize",&readTimestamp,&DMASampleSize); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)4, DMASampleSize);

	// Get DMASamplingRate initial value
	std::int32_t DMASamplingRate;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMASamplingRate",&readTimestamp,&DMASamplingRate); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1000, DMASamplingRate);

	// Get DMAEnable initial value
	std::int32_t DMAEnable;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable",&readTimestamp,&DMAEnable); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)0, DMAEnable);

	// Check DAQ Node initial state (OFF)
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change DAQ Node state:  OFF -> (initializing) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change DAQ Node state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	// Get DMAEnable initial value
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.DMAEnable", timestamp, (std::int32_t)1);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.DMAEnable_RBV",&readTimestamp,&DMAEnable); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)1, DMAEnable);

	//Change DAQ Node state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change DAQ Node state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Change Device state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(2);
	pInterface->getPushedInt32("/rootNode-StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	factory.destroyDevice("rootNode");

}
