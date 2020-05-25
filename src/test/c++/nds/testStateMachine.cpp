#include <unistd.h>
#include <functional>
#include <iostream>

#include <gtest/gtest.h>
#include <nds3/nds.h>

#include "Device.h"
#include "nds3/ndsTestInterface.h"
#include "nds3/ndsTestFactory.h"

std::string stateToString(nds::state_t state){
	switch(state){
	case nds::state_t::unknown:
		return "unknown";
		break;
	case nds::state_t::off:
		return "off";
		break;
	case nds::state_t::switchingOff:
		return "switchingOff";
		break;
	case nds::state_t::initializing:
		return "initializing";
		break;
	case nds::state_t::on:
		return "on";
		break;
	case nds::state_t::stopping:
		return "stopping";
		break;
	case nds::state_t::starting:
		return "starting";
		break;
	case nds::state_t::running:
		return "running";
		break;
	case nds::state_t::fault:
		return "fault";
		break;
	default:
		return "wrong state";
		break;
	}
}

void wait1sec()
{
    ::sleep(1);
}

void rollback()
{
    ::usleep(100000);
    throw nds::StateMachineRollBack("rolling back");
}


bool returnTrue(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}

TEST(testStateMachine, testLocalGlobalState)
{
    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachine0 = rootNode.addChild(nds::StateMachine(true,
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Node ch0 = rootNode.addChild(nds::Node("ch0"));
    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));

    nds::StateMachine stateMachine1 = ch0.addChild(nds::StateMachine(true,
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));


    nds::StateMachine stateMachine2 = ch1.addChild(nds::StateMachine(true,
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&rollback),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&wait1sec),
                                                                     std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Check the local and global state: should be 0
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    // Switch on state machine 1
    stateMachine1.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::initializing, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());

    // Switch state machine 2 to on and then to running.
    // Should go back to on because of the rollback
    stateMachine2.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::initializing, (int)stateMachine2.getLocalState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());

    stateMachine2.setState(nds::state_t::running);
    EXPECT_EQ((int)nds::state_t::starting, (int)stateMachine0.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine0.getLocalState());
    EXPECT_EQ((int)nds::state_t::starting, (int)stateMachine2.getLocalState());
    ::sleep(1);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());

    stateMachine2.setState(nds::state_t::off);
    EXPECT_EQ((int)nds::state_t::switchingOff, (int)stateMachine2.getLocalState());
    ::sleep(2);
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine2.getLocalState());

    factory.destroyDevice("");
}



/*
 * This is the tested structure
 *
 * rootNode
 * 		| - STM
 * 		| - CH0
 * 				| - STM
 * 				| - CH0A
 * 						| - STM
 * 				| - CH0B
 * 		| - CH1
 * 				| - STM
 * 				| - CH1A
 * 						| -CH1AA
 *
 */

TEST(testStateMachine, testChildrenStates)
{
    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachineRN0 = rootNode.addChild(nds::StateMachine(false,
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Node ch0 = rootNode.addChild(nds::Node("ch0"));
    nds::StateMachine stateMachineCH0 = ch0.addChild(nds::StateMachine(false,
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&wait1sec),
                                                                          std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
    nds::Node ch0A = ch0.addChild(nds::Node("ch0A"));
    nds::StateMachine stateMachineCH0A = ch0A.addChild(nds::StateMachine(false,
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

    nds::Node ch0B = ch0.addChild(nds::Node("ch0B"));

    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));

    nds::StateMachine stateMachineCH1 = ch1.addChild(nds::StateMachine(false,
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&wait1sec),
                                                                              std::bind(&returnTrue, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
    nds::Node ch1A = ch1.addChild(nds::Node("ch1A"));
    nds::Node ch1AA = ch1A.addChild(nds::Node("ch1AA"));

    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    std::cout<<"-----------------------------------------------"<<std::endl;
    std::cout<<"- INIT states"<<std::endl;
    std::cout<<"-----------------------------------------------"<<std::endl;
    std::cout<<"RootNode state: "<<stateToString(stateMachineRN0.getLocalState())<<std::endl;
    std::cout<<"CH0 state: "<<stateToString(stateMachineCH0.getLocalState())<<std::endl;
    std::cout<<"CH0A state: "<<stateToString(stateMachineCH0A.getLocalState())<<std::endl;
    std::cout<<"CH1 state: "<<stateToString(stateMachineCH1.getLocalState())<<std::endl<<std::endl;

    //All STMs default state is off
    stateMachineRN0.setState(nds::state_t::on); //Turn on the rootNode (The rest of the STM stay at off state)
    stateMachineRN0.setState(nds::state_t::running); //Start the rootNode (The rest of the STM stay at off state)

    std::cout<<"-----------------------------------------------"<<std::endl;
	std::cout<<"- RootNode running "<<std::endl;
	std::cout<<"-----------------------------------------------"<<std::endl;
	std::cout<<"RootNode state: "<<stateToString(stateMachineRN0.getLocalState())<<std::endl;
	std::cout<<"CH0 state: "<<stateToString(stateMachineCH0.getLocalState())<<std::endl;
	std::cout<<"CH0A state: "<<stateToString(stateMachineCH0A.getLocalState())<<std::endl;
	std::cout<<"CH1 state: "<<stateToString(stateMachineCH1.getLocalState())<<std::endl<<std::endl;

    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN0.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN0.getHighestChildState());

    //Start one of the children
    stateMachineCH0A.setState(nds::state_t::on);
    stateMachineCH0A.setState(nds::state_t::running);

    std::cout<<"-----------------------------------------------"<<std::endl;
	std::cout<<"- One child node running "<<std::endl;
	std::cout<<"-----------------------------------------------"<<std::endl;
	std::cout<<"RootNode state: "<<stateToString(stateMachineRN0.getLocalState())<<std::endl;
	std::cout<<"CH0 state: "<<stateToString(stateMachineCH0.getLocalState())<<std::endl;
	std::cout<<"CH0A state: "<<stateToString(stateMachineCH0A.getLocalState())<<std::endl;
	std::cout<<"CH1 state: "<<stateToString(stateMachineCH1.getLocalState())<<std::endl<<std::endl;

    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN0.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN0.getHighestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineCH0.getLowestChildState());

    //Turn ON the rest of the chlidren
    stateMachineCH0.setState(nds::state_t::on);
    stateMachineCH1.setState(nds::state_t::on);

    std::cout<<"-----------------------------------------------"<<std::endl;
	std::cout<<"- One child node running, the others in ON state "<<std::endl;
	std::cout<<"-----------------------------------------------"<<std::endl;
	std::cout<<"RootNode state: "<<stateToString(stateMachineRN0.getLocalState())<<std::endl;
	std::cout<<"CH0 state: "<<stateToString(stateMachineCH0.getLocalState())<<std::endl;
	std::cout<<"CH0A state: "<<stateToString(stateMachineCH0A.getLocalState())<<std::endl;
	std::cout<<"CH1 state: "<<stateToString(stateMachineCH1.getLocalState())<<std::endl<<std::endl;

    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN0.getLowestChildState());
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN0.getHighestChildState());

    factory.destroyDevice("");

}

