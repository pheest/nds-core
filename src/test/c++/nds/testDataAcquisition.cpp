
#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "../include/Device_Vector_DBL.h"
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDataAcquisition, testStateMachine)
{
	const timespec* pStateMachineSwitchTime;
	const std::int32_t* pStateMachineState;
	timespec timestamp = {0, 0};

	//Create factory
	nds::Factory factory("test");

	// Create test device of type DeviceVectorDBL and named rootNode
	factory.createDevice("DeviceVectorDBL", "rootNode", nds::namedParameters_t());

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
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
	::sleep(2);

	//TODO: Check generated data

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(1);
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

	factory.createDevice("DeviceVectorDBL", "rootNode", nds::namedParameters_t());

	nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

	// Set/Get Gain
	double Gain;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ((double)0, Gain);

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
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	//std::int32_t startTimestamp = 200;
	//pInterface->writeCSValue("/rootNode-DataAcquisitionNode.setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);
	std::cout<<"NumberOfPushedDataBlocks="<<NumberOfPushedDataBlocks<<std::endl;

	//Initialize comparison vector
	std::vector<double> pushData(128);
	const std::vector<double>* pRetrievedPushedValues;
	const timespec* pTime;
	double readCount=0;
	size_t scanVector(0);
	try{
		while(readCount<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = readCount;
			}

			pInterface->getPushedVectorDouble("/rootNode-DataAcquisitionNode.data", pTime, pRetrievedPushedValues);
			++readCount;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
				std::cout<<"pRetrievedPushedValues="<<(*pRetrievedPushedValues)[compare]<<" ";
			}
			std::cout<<std::endl;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
	}
	//EXPECT_EQ(startTimestamp, pTime->tv_sec);
	//EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	//++startTimestamp;

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
	double Gain;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ((double)0, Gain);

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
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	//std::int32_t startTimestamp = 200;
	//pInterface->writeCSValue("/rootNode-DataAcquisitionNode.setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);
	std::cout<<"NumberOfPushedDataBlocks="<<NumberOfPushedDataBlocks<<std::endl;

	//Initialize comparison vector
	std::vector<std::int32_t> pushData(128);
	const std::vector<std::int32_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t readCount=0;
	size_t scanVector(0);
	try{
		while(readCount<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = readCount;
			}

			pInterface->getPushedVectorInt32("/rootNode-DataAcquisitionNode.data", pTime, pRetrievedPushedValues);
			++readCount;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
				std::cout<<"pRetrievedPushedValues="<<(*pRetrievedPushedValues)[compare]<<" ";
			}
			std::cout<<std::endl;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
	}
	//EXPECT_EQ(startTimestamp, pTime->tv_sec);
	//EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	//++startTimestamp;

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
	double Gain;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ((double)0, Gain);

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
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	//std::int32_t startTimestamp = 200;
	//pInterface->writeCSValue("/rootNode-DataAcquisitionNode.setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);
	std::cout<<"NumberOfPushedDataBlocks="<<NumberOfPushedDataBlocks<<std::endl;

	//Initialize comparison vector
	std::vector<std::int8_t> pushData(128);
	const std::vector<std::int8_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int8_t readCount=0;
	size_t scanVector(0);
	try{
		while(readCount<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = readCount;
			}

			pInterface->getPushedVectorInt8("/rootNode-DataAcquisitionNode.data", pTime, pRetrievedPushedValues);
			++readCount;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
				std::cout<<"pRetrievedPushedValues="<<(*pRetrievedPushedValues)[compare]<<" ";
			}
			std::cout<<std::endl;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<(std::int32_t)readCount << std::endl;
	}
	//EXPECT_EQ(startTimestamp, pTime->tv_sec);
	//EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	//++startTimestamp;

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
	double Gain;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ((double)0, Gain);

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
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	//std::int32_t startTimestamp = 200;
	//pInterface->writeCSValue("/rootNode-DataAcquisitionNode.setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);
	std::cout<<"NumberOfPushedDataBlocks="<<NumberOfPushedDataBlocks<<std::endl;

	//Initialize comparison vector
	std::vector<std::uint8_t> pushData(128);
	const std::vector<std::uint8_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::uint8_t readCount=0;
	size_t scanVector(0);
	try{
		while(readCount<=NumberOfPushedDataBlocks){

			for(scanVector=0; scanVector != pushData.size(); ++scanVector){
				pushData[scanVector] = readCount;
			}

			pInterface->getPushedVectorUint8("/rootNode-DataAcquisitionNode.data", pTime, pRetrievedPushedValues);
			++readCount;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
				std::cout<<"pRetrievedPushedValues="<<(*pRetrievedPushedValues)[compare]<<" ";
			}
			std::cout<<std::endl;
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<(std::int32_t)readCount << std::endl;
	}
	//EXPECT_EQ(startTimestamp, pTime->tv_sec);
	//EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	//++startTimestamp;

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
	double Gain;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ((double)0, Gain);

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
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	//std::int32_t startTimestamp = 200;
	//pInterface->writeCSValue("/rootNode-DataAcquisitionNode.setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);
	std::cout<<"NumberOfPushedDataBlocks="<<NumberOfPushedDataBlocks<<std::endl;

	//Initialize comparison vector
	double pushData(0);
	const double* pRetrievedPushedValues;
	const timespec* pTime;
	double readCount=0;

	try{
		while(readCount<=NumberOfPushedDataBlocks){

			pushData = readCount;


			pInterface->getPushedDouble("/rootNode-DataAcquisitionNode.data", pTime, pRetrievedPushedValues);
			++readCount;

			EXPECT_EQ(pushData, (*pRetrievedPushedValues));
			std::cout<<"pRetrievedPushedValues="<<*pRetrievedPushedValues<<" "<<std::endl;


		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
	}
	//EXPECT_EQ(startTimestamp, pTime->tv_sec);
	//EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	//++startTimestamp;

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
	double Gain;
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.Gain", timestamp, (double)0);
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
	EXPECT_EQ((double)0, Gain);

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
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	// Set the start time
	/////////////////////
	//std::int32_t startTimestamp = 200;
	//pInterface->writeCSValue("/rootNode-DataAcquisitionNode.setCurrentTime", timestamp, startTimestamp);

	//Change state:  ON -> (starting) -> RUNNING
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

	::sleep(2);//Data is being generated and pushed to Control system

	//Change state:  RUNNING -> (stopping) -> ON
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);

	//Change state:  ON -> (switchingOff) -> OFF
	pInterface->writeCSValue("/rootNode-DataAcquisitionNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::off);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::switchingOff, *pStateMachineState);
	::sleep(1);
	pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
	EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);

	//Get number of pushed vectors by the Acquisition node.
	std::int32_t NumberOfPushedDataBlocks;
	pInterface->readCSValue("/rootNode-DataAcquisitionNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);
	std::cout<<"NumberOfPushedDataBlocks="<<NumberOfPushedDataBlocks<<std::endl;

	//Initialize comparison vector
	std::int32_t pushData(0);
	const std::int32_t* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t readCount=0;
	try{
		while(readCount<=NumberOfPushedDataBlocks){

			pushData = readCount;

			pInterface->getPushedInt32("/rootNode-DataAcquisitionNode.data", pTime, pRetrievedPushedValues);
			++readCount;

			EXPECT_EQ(pushData, (*pRetrievedPushedValues));
			std::cout<<"pRetrievedPushedValues="<<*pRetrievedPushedValues<<" "<<std::endl;

		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<readCount << std::endl;
	}
	//EXPECT_EQ(startTimestamp, pTime->tv_sec);
	//EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
	//++startTimestamp;

	factory.destroyDevice("rootNode");

}
//TEST(testDataAcquisition, testDecimation)
//{
//    nds::Factory factory("test");
//
//    factory.createDevice("DeviceVectorDBL", "rootNode", nds::namedParameters_t());
//
//    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode-Channel1");
//
//    const timespec* pStateMachineSwitchTime;
//    const std::int32_t* pStateMachineState;
//    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::off, *pStateMachineState);
//
//    timespec timestamp = {0, 0};
//    pInterface->writeCSValue("/rootNode-Channel1.data.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
//    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::initializing, *pStateMachineState);
//
//    // Wait for the switch on state (it should take one second)
//    ///////////////////////////////////////////////////////////
//    ::sleep(2);
//    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::on, *pStateMachineState);
//
//    // Start the data acquisition
//    /////////////////////////////
//    pInterface->writeCSValue("/rootNode-Channel1.data.decimation", timestamp, (std::int32_t)2);
//    pInterface->writeCSValue("/rootNode-Channel1.numAcquisitions", timestamp, (std::int32_t)100);
//
//    // Start the acquisition via node command
//    /////////////////////////////////////////
//    nds::parameters_t emptyParameters;
//    nds::tests::TestControlSystemFactoryImpl::getInstance()->executeCommand("start", "rootNode-Channel1-data", emptyParameters);
//    ::sleep(1);
//    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState); // Retrieve the starting state
//    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);
//
//    ::sleep(1);
//
//    // Initialize comparison vector
//    std::vector<std::int32_t> pushData(10000);
//    for(size_t initializeVectors(0); initializeVectors != 10000; ++initializeVectors)
//    {
//        pushData[initializeVectors] = 10000- initializeVectors;
//    }
//
//    const std::vector<std::int32_t>* pRetrievedPushedValues;
//    const timespec* pTime;
//    for(size_t numAcquisitions(0); numAcquisitions != 100; ++numAcquisitions)
//    {
//        if((numAcquisitions & 1) == 1)
//        {
//            pInterface->getPushedVectorInt32("/rootNode-Channel1.data.Data", pTime, pRetrievedPushedValues);
//            ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
//            for(size_t compare(0); compare != pushData.size(); ++compare)
//            {
//                EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
//            }
//        }
//    }
//
//    pInterface->writeCSValue("/rootNode-Channel1.data.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::on);
//    pInterface->getPushedInt32("/rootNode-Channel1.data.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
//    EXPECT_EQ((std::int32_t)nds::state_t::stopping, *pStateMachineState);
//
//    factory.destroyDevice("rootNode");
//
//}

