#include <gtest/gtest.h>
#include <nds3/nds.h>
#include <functional>
#include "../include/ndsTestInterface.h"
#include "../include/ndsTestFactory.h"
#include <unistd.h>


#define ROOT_NAME _Device
#define CHILD1_NAME _Child1
#define CHILD2_NAME _Child2
#define CHILD3_NAME _Child3

/*
 * STM0
 */
#define STM(NAME) void switchOn##NAME(){\
	std::cout<<"I'm on "<<__func__<<std::endl;\
}\
void starting##NAME(){\
	std::cout<<"I'm on "<<__func__<<std::endl;\
}\
void switchOff##NAME(){\
	std::cout<<"I'm on "<<__func__<<std::endl;\
}\
void stopping##NAME(){\
	std::cout<<"I'm on "<<__func__<<std::endl;\
}\
void recover##NAME(){\
	std::cout<<"I'm on "<<__func__<<std::endl;\
}\

STM(ROOT_NAME);
STM(CHILD1_NAME);
STM(CHILD2_NAME);
STM(CHILD3_NAME);

#define STM_NODE(PARENT,NAME,RET,AUTOENABLE) PARENT.addChild(nds::StateMachine(false,\
                                                                          std::bind(&switchOn##NAME),\
                                                                          std::bind(&switchOff##NAME),\
                                                                          std::bind(&starting##NAME),\
                                                                          std::bind(&stopping##NAME),\
                                                                          std::bind(&recover##NAME),\
                                                                          std::bind(&RET, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),\
																		  AUTOENABLE));\

bool alwaysReturnTrue(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}

bool alwaysReturnFalse(const nds::state_t,const nds::state_t , const nds::state_t)
{
    return false;
}


TEST(testStateMachineHierarchichal, testSuccesfulTransitionState)
{
    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachineRN = STM_NODE(rootNode,ROOT_NAME,alwaysReturnTrue,nds::autoEnable_t::off);

    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));
    nds::Node ch2 = rootNode.addChild(nds::Node("ch2"));
    nds::Node ch3 = rootNode.addChild(nds::Node("ch3"));

    nds::StateMachine stateMachine1 = STM_NODE(ch1,CHILD1_NAME,alwaysReturnTrue,nds::autoEnable_t::running);
    nds::StateMachine stateMachine2 = STM_NODE(ch2,CHILD2_NAME,alwaysReturnTrue,nds::autoEnable_t::on);
    nds::StateMachine stateMachine3 = STM_NODE(ch3,CHILD3_NAME,alwaysReturnTrue,nds::autoEnable_t::off);

    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Check the local and global state: should be 0 (off)
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getLocalState());

    // Switch on state machine of the RootNode
    stateMachineRN.setState(nds::state_t::on);
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 & STM1 go automatically to ON state and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine3.getLocalState());


    // Starting state machine of the RootNode
    stateMachineRN.setState(nds::state_t::running);
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 go automatically to RUNNING state, STM1 stays in ON and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::running, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::on, (int)stateMachine2.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine3.getLocalState());

    factory.destroyDevice("");
}


TEST(testStateMachineHierarchichal, testErrorTransitionState)
{

    nds::Port rootNode("rootNode");
    nds::StateMachine stateMachineRN = STM_NODE(rootNode,ROOT_NAME,alwaysReturnTrue,nds::autoEnable_t::off);

    nds::Node ch1 = rootNode.addChild(nds::Node("ch1"));
    nds::Node ch2 = rootNode.addChild(nds::Node("ch2"));

    nds::StateMachine stateMachine1 = STM_NODE(ch1,CHILD1_NAME,alwaysReturnTrue,nds::autoEnable_t::running);
    nds::StateMachine stateMachine2 = STM_NODE(ch2,CHILD2_NAME,alwaysReturnFalse,nds::autoEnable_t::on);


    nds::Factory factory("test");

    rootNode.initialize(0, factory);

    // Check the local and global state: should be 0 (off)
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getGlobalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getLocalState());

    // Switch on state machine of the RootNode
    try{
    stateMachineRN.setState(nds::state_t::on);
    }catch(nds::StateMachineTransitionDenied& e){
    	std::cout<<e.what()<<std::endl;
    }
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachineRN.getLocalState());

    //Expected behavior: STM0 & STM1 go automatically to ON state and STM2 stays in OFF
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine1.getLocalState());
    EXPECT_EQ((int)nds::state_t::off, (int)stateMachine2.getLocalState());

    factory.destroyDevice("");
}


