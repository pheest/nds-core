#include <gtest/gtest.h>
#include <nds3/nds.h>
#include "nds3/exceptions.h"
#include <math.h>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"

TEST(testDataGeneration, testStateMachine)
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
    ::sleep(2);

    //TODO: Check generated data

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

TEST(testDataGeneration, testPushDataGeneratedVDBL)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Amplitude", timestamp, (double)5);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)5, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
	 *	Spline=1 (disabled)
	 *	DC=2
	 *	Sin=3
	 *	Square=4
	 *	Triangle=5(disabled)
	 *	Pulse=6(disabled)
     *	Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

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

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

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

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataGenerationNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<double> pushData(128);
	size_t last_sample(0);
	const std::vector<double>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t pushCounter=0;
	size_t scanVector(0);
	std::int64_t angle(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			switch(auxsignalType){

			case 0:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 1:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 2:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 3:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
				}
				break;
			case 4:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
				}
				break;
			case 5:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 6:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 7:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;

				// Fill the vector with a DC wave of maxamplitude
			default:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			}
			last_sample+=scanVector;

			pInterface->getPushedVectorDouble("/rootNode-DataGenerationNode.data", pTime, pRetrievedPushedValues);
			++pushCounter;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}

		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}
		EXPECT_EQ(startTimestamp, pTime->tv_sec);
		EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
		++startTimestamp;

    factory.destroyDevice("rootNode");

}

TEST(testDataGeneration, testPushDataGeneratedVI8)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("DeviceVectorI8", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Amplitude", timestamp, (double)127);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)127, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
	 *	Spline=1 (disabled)
	 *	DC=2
	 *	Sin=3
	 *	Square=4
	 *	Triangle=5(disabled)
	 *	Pulse=6(disabled)
     *	Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

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

    ::sleep(2);//Data is being generated and pushed to Control system

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

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataGenerationNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<std::int8_t> pushData(128);
	size_t last_sample(0);
	const std::vector<std::int8_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t pushCounter=0;
	size_t scanVector(0);
	std::int64_t angle(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			switch(auxsignalType){

			case 0:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 1:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 2:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 3:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
				}
				break;
			case 4:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
				}
				break;
			case 5:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 6:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 7:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;

			default:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			}
			last_sample+=scanVector;

			pInterface->getPushedVectorInt8("/rootNode-DataGenerationNode.data", pTime, pRetrievedPushedValues);
			++pushCounter;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

    factory.destroyDevice("rootNode");

}

TEST(testDataGeneration, testPushDataGeneratedVUI8)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("DeviceVectorUI8", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Amplitude", timestamp, (double)255);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)255, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
	 *	Spline=1 (disabled)
	 *	DC=2
	 *	Sin=3
	 *	Square=4
	 *	Triangle=5(disabled)
	 *	Pulse=6(disabled)
     *	Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

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

    ::sleep(2);//Data is being generated and pushed to Control system

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

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataGenerationNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<std::uint8_t> pushData(128);
	size_t last_sample(0);
	const std::vector<std::uint8_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t pushCounter=0;
	size_t scanVector(0);
	std::int64_t angle(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			switch(auxsignalType){

			case 0:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 1:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 2:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 3:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
				}
				break;
			case 4:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
				}
				break;
			case 5:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 6:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 7:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;

				// Fill the vector with a DC wave of maxamplitude
			default:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			}
			last_sample+=scanVector;

			pInterface->getPushedVectorUint8("/rootNode-DataGenerationNode.data", pTime, pRetrievedPushedValues);
			++pushCounter;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

    factory.destroyDevice("rootNode");

}

TEST(testDataGeneration, testPushDataGeneratedVI32)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("DeviceVectorI32", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Amplitude", timestamp, (double)1024);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)1024, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
	 *	Spline=1 (disabled)
	 *	DC=2
	 *	Sin=3
	 *	Square=4
	 *	Triangle=5(disabled)
	 *	Pulse=6(disabled)
     *	Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

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

    ::sleep(2);//Data is being generated and pushed to Control system

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

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataGenerationNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<std::int32_t> pushData(128);
	size_t last_sample(0);
	const std::vector<std::int32_t>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t pushCounter=0;
	size_t scanVector(0);
	std::int64_t angle(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			switch(auxsignalType){

			case 0:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 1:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 2:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 3:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
				}
				break;
			case 4:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
				}
				break;
			case 5:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 6:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 7:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;

				// Fill the vector with a DC wave of maxamplitude
			default:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			}
			last_sample+=scanVector;

			pInterface->getPushedVectorInt32("/rootNode-DataGenerationNode.data", pTime, pRetrievedPushedValues);
			++pushCounter;
			ASSERT_EQ(pushData.size(), pRetrievedPushedValues->size());
			for(size_t compare(0); compare != pushData.size(); ++compare)
			{
				EXPECT_EQ(pushData[compare], (*pRetrievedPushedValues)[compare]);
			}
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

    factory.destroyDevice("rootNode");

}

TEST(testDataGeneration, testPushDataGeneratedDBL)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("DeviceDBL", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Amplitude", timestamp, (double)5);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)5, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
	 *	Spline=1 (disabled)
	 *	DC=2
	 *	Sin=3
	 *	Square=4
	 *	Triangle=5(disabled)
	 *	Pulse=6(disabled)
     *	Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

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

    ::sleep(2);//Data is being generated and pushed to Control system

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

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataGenerationNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    double pushData(0);
	size_t last_sample(0);
	const double* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t pushCounter=0;
	std::int64_t angle(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			switch(auxsignalType){

			case 0:
					pushData = amplitude;

				break;
			case 1:
					pushData = amplitude;

				break;
			case 2:
					pushData = amplitude;

				break;
			case 3:
					pushData = (double)amplitude * sin((2*M_PI*(last_sample)*frequency)/updateRate + phase) + offset;
				break;
			case 4:
					pushData = ((angle & 0xff) < 128) ? amplitude : - amplitude;
				break;
			case 5:
					pushData = amplitude;
				break;
			case 6:
					pushData = amplitude;
				break;
			case 7:
					pushData = amplitude;
				break;

				// Fill the vector with a DC wave of maxamplitude
			default:
					pushData = amplitude;
				break;
			}
			last_sample++;

			pInterface->getPushedDouble("/rootNode-DataGenerationNode.data", pTime, pRetrievedPushedValues);
			++pushCounter;
			EXPECT_EQ(pushData, (*pRetrievedPushedValues));
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

    factory.destroyDevice("rootNode");

}

TEST(testDataGeneration, testPushDataGeneratedI32)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("DeviceI32", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Amplitude", timestamp, (double)1024);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)1024, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
	 *	Spline=1 (disabled)
	 *	DC=2
	 *	Sin=3
	 *	Square=4
	 *	Triangle=5(disabled)
	 *	Pulse=6(disabled)
     *	Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

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

    ::sleep(2);//Data is being generated and pushed to Control system

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

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataGenerationNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::int32_t pushData(0);
	size_t last_sample(0);
	const std::int32_t* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t pushCounter=0;
		std::int64_t angle(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			switch(auxsignalType){

			case 0:
					pushData = amplitude;

				break;
			case 1:
					pushData = amplitude;

				break;
			case 2:
					pushData = amplitude;

				break;
			case 3:
					pushData = (double)amplitude * sin((2*M_PI*(last_sample)*frequency)/updateRate + phase) + offset;
				break;
			case 4:
					pushData = ((angle & 0xff) < 128) ? amplitude : - amplitude;
				break;
			case 5:
					pushData = amplitude;
				break;
			case 6:
					pushData = amplitude;
				break;
			case 7:
					pushData = amplitude;
				break;

				// Fill the vector with a DC wave of maxamplitude
			default:
					pushData = amplitude;
				break;
			}
			last_sample++;

			pInterface->getPushedInt32("/rootNode-DataGenerationNode.data", pTime, pRetrievedPushedValues);
			++pushCounter;
			EXPECT_EQ(pushData, (*pRetrievedPushedValues));
		}
	}
	catch(const std::runtime_error& e)
	{

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}

    factory.destroyDevice("rootNode");

}

TEST(testDataGeneration, testdecimation)
{

    const timespec* pStateMachineSwitchTime;
    const std::int32_t* pStateMachineState;
    timespec timestamp = {0, 0}, readTimestamp{0,0};

    nds::Factory factory("test");

    factory.createDevice("Device", "rootNode", nds::namedParameters_t());

    nds::tests::TestControlSystemInterfaceImpl* pInterface = nds::tests::TestControlSystemInterfaceImpl::getInstance("rootNode");

    // Set/Get Amplitude = 5
    double amplitude;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Amplitude", timestamp, (double)5);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Amplitude_RBV",&readTimestamp,&amplitude); // PVVariables are thread safe
    EXPECT_EQ((double)5, amplitude);

    // Set/Get Frequency = 1000
    double frequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Frequency", timestamp, (double)1000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Frequency_RBV",&readTimestamp,&frequency); // PVVariables are thread safe
    EXPECT_EQ((double)1000, frequency);

    // Set/Get updateRate
    double updateRate;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.UpdateRate", timestamp, (double)128000);
    pInterface->readCSValue("/rootNode-DataGenerationNode.UpdateRate_RBV",&readTimestamp,&updateRate); // PVVariables are thread safe
    EXPECT_EQ((double)128000, updateRate);

    // Set/Get offset
    double offset;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Offset", timestamp, (double)1);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Offset_RBV",&readTimestamp,&offset); // PVVariables are thread safe
    EXPECT_EQ((double)1, offset);

    // Set/Get phase
    double phase;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Phase", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Phase_RBV",&readTimestamp,&phase); // PVVariables are thread safe
    EXPECT_EQ((double)0, phase);

    // Set/Get RefFrequency
    double RefFrequency;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.RefFrequency", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.RefFrequency_RBV",&readTimestamp,&RefFrequency); // PVVariables are thread safe
    EXPECT_EQ((double)0, RefFrequency);

    // Set/Get DutyCycle
    double DutyCycle;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.DutyCycle", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.DutyCycle_RBV",&readTimestamp,&DutyCycle); // PVVariables are thread safe
    EXPECT_EQ((double)0, DutyCycle);

    // Set/Get Gain
    double Gain;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Gain", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Gain_RBV",&readTimestamp,&Gain); // PVVariables are thread safe
    EXPECT_EQ((double)0, Gain);

    // Set/Get Bandwidth
    double Bandwidth;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.BandWidth", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.BandWidth_RBV",&readTimestamp,&Bandwidth); // PVVariables are thread safe
    EXPECT_EQ((double)0, Bandwidth);

    // Set/Get Resolution
    double Resolution;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Resolution", timestamp, (double)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Resolution_RBV",&readTimestamp,&Resolution); // PVVariables are thread safe
    EXPECT_EQ((double)0, Resolution);

    // Set/Get Impedance
    std::int32_t Impedance;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Impedance", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Impedance_RBV",&readTimestamp,&Impedance); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Impedance);

    // Set/Get Coupling
    std::int32_t Coupling;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Coupling", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Coupling_RBV",&readTimestamp,&Coupling); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Coupling);

    // Set/Get SignalRef
    std::int32_t SignalRef;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalRefType", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalRefType_RBV",&readTimestamp,&SignalRef); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, SignalRef);

    // Set/Get Ground
    std::int32_t Ground;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.Ground", timestamp, (std::int32_t)0);
    pInterface->readCSValue("/rootNode-DataGenerationNode.Ground_RBV",&readTimestamp,&Ground); // PVVariables are thread safe
    EXPECT_EQ((std::int32_t)0, Ground);

    /*  SignalType enum
     *  WaveForm=0 (disabled)
	 *	Spline=1 (disabled)
	 *	DC=2
	 *	Sin=3
	 *	Square=4
	 *	Triangle=5(disabled)
	 *	Pulse=6(disabled)
     *	Sawtooth=7(disabled)
     */

    // Set/Get SignalType generated
    //nds::enumerationStrings_t signalType;
    std::int32_t auxsignalType;
    pInterface->writeCSValue("/rootNode-DataGenerationNode.SignalType", timestamp, (std::int32_t)3);
    pInterface->readCSValue("/rootNode-DataGenerationNode.SignalType_RBV",&readTimestamp,&auxsignalType); // PVVariables are thread safe

	// Set/Get Ground
	std::int32_t decimation;
	pInterface->writeCSValue("/rootNode-DataGenerationNode.Decimation", timestamp, (std::int32_t)10);
	pInterface->readCSValue("/rootNode-DataGenerationNode.Decimation",&readTimestamp,&decimation); // PVVariables are thread safe
	EXPECT_EQ((std::int32_t)10, decimation);

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

    // Set the start time
    /////////////////////
    std::int32_t startTimestamp = 200;
    pInterface->writeCSValue("/rootNode-setCurrentTime", timestamp, startTimestamp);

    //Change state:  ON -> (starting) -> RUNNING
    pInterface->writeCSValue("/rootNode-DataGenerationNode.StateMachine.setState", timestamp, (std::int32_t)nds::state_t::running);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::starting, *pStateMachineState);
    ::sleep(1);
    pInterface->getPushedInt32("/rootNode-DataGenerationNode.StateMachine.getState", pStateMachineSwitchTime, pStateMachineState);
    EXPECT_EQ((std::int32_t)nds::state_t::running, *pStateMachineState);

    ::sleep(2);//Data is being generated and pushed to Control system

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

    //Get number of pushed vectors by the generation node.
    std::int32_t NumberOfPushedDataBlocks;
    pInterface->readCSValue("/rootNode-DataGenerationNode.NumberOfPushedDataBlocks", &readTimestamp,&NumberOfPushedDataBlocks);
	EXPECT_NE((std::int32_t)0, NumberOfPushedDataBlocks);

     //Initialize comparison vector
    std::vector<double> pushData(128);
	size_t last_sample(0);
	const std::vector<double>* pRetrievedPushedValues;
	const timespec* pTime;
	std::int32_t pushCounter=0;
	std::int32_t decimationCounter=0;
	size_t scanVector(0);
	std::int64_t angle(0);
	try{
		while(pushCounter<=NumberOfPushedDataBlocks){

			switch(auxsignalType){

			case 0:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 1:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 2:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 3:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
				}
				break;
			case 4:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
				}
				break;
			case 5:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 6:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			case 7:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;

				// Fill the vector with a DC wave of maxamplitude
			default:
				for(scanVector=0; scanVector != pushData.size(); ++scanVector){
					pushData[scanVector] = amplitude;
				}
				break;
			}
			last_sample+=scanVector;
			if((std::int32_t)(decimationCounter+1) % decimation ==0){
				pInterface->getPushedVectorDouble("/rootNode-DataGenerationNode.data", pTime, pRetrievedPushedValues);
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

		std::cerr << e.what() << ". Number of pushed data blocks is: " <<pushCounter << std::endl;
	}
		EXPECT_EQ(startTimestamp, pTime->tv_sec);
		EXPECT_EQ(startTimestamp + 10, pTime->tv_nsec);
		++startTimestamp;

    factory.destroyDevice("rootNode");

}
