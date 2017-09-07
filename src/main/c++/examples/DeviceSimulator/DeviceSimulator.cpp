/*
 * device_simulator.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: ebernal
 *      GMV & UPM
 */
#include "DeviceSimulator.h"
//#define DAQ

DeviceSimulator::DeviceSimulator(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t & /* parameters */){
    // Here we declare the root node.
    // It is a good practice to name it with the device name.
    // Also, for simplicity we declare it as a "Port": this means that the root node will be responsible for the communication with the underlying control system.
    // It is possible to have the root node as a simple Node and promote one or more of its children to "Port": each port will interface with a different
    // control system thread.
    nds::Port rootNode(deviceName);

    // Let's add info PV's to manage a cRIO or FlexRIO FPGA based device
    m_ndsversion = nds::PVVariableIn<std::string>("ndsversion");
    m_ndsversion.setDescription("nds framework version" );
    m_ndsversion.setScanType((nds::scanType_t)0, (double)1);
    m_ndsversion.setUnits("N/A");
    m_ndsversion.setMaxElements(10);
    m_ndsversion.setValue("V3.0.2");
    rootNode.addChild(m_ndsversion);

    m_irioversion = nds::PVVariableIn<std::string>("irioversion");
    m_irioversion.setDescription("irio lib version" );
    m_irioversion.setScanType((nds::scanType_t)0, (double)1);
    m_irioversion.setUnits("N/A");
    m_irioversion.setMaxElements(10);
    m_irioversion.setValue("V1.2.0");
    rootNode.addChild(m_irioversion);

    m_fpgaVIversion = nds::PVVariableIn<std::string>("fpgaVIversion");
    m_fpgaVIversion.setDescription("fpga VI version" );
    m_fpgaVIversion.setScanType((nds::scanType_t)0, (double)1);
    m_fpgaVIversion.setUnits("N/A");
    m_fpgaVIversion.setMaxElements(10);
    m_fpgaVIversion.setValue("V1.1");
    rootNode.addChild(m_fpgaVIversion);

    m_infostatus = nds::PVVariableIn<std::string>("infostatus");
    m_infostatus.setDescription("infostatus" );
    m_infostatus.setScanType((nds::scanType_t)0, (double)1);
    m_infostatus.setUnits("N/A");
    m_infostatus.setMaxElements(100);
    m_infostatus.setValue("Device initializing");
    rootNode.addChild(m_infostatus);

    m_debugmode = nds::PVVariableOut<std::int32_t>("debugmode");
    m_debugmode.setDescription("debugmode" );
    m_debugmode.setScanType((nds::scanType_t)0, (double)1);
    m_debugmode.setUnits("N/A");
    m_debugmode.setMaxElements(1);
    rootNode.addChild(m_debugmode);


    // Let's add 1 acquisition channelgroup to the root node. This channel group acquire data from FPGA. This data are packages of data channels. So ,after acquire them
    // they must be unpacked and send to proper data acquisition channels.
    for(size_t numChannelGroup(0); numChannelGroup < 1; ++numChannelGroup)
    {
        std::ostringstream channelgroupName;
        channelgroupName << "CHGR" << numChannelGroup;
        m_channelgroup.push_back(std::make_shared<ChannelGroup>(channelgroupName.str(), rootNode));
    }

    // Let's add 4 acquisition channels to the root node. This channels read data acquired by the channelgroup.
    for(size_t numChannel(0); numChannel != 4; ++numChannel)
    {
        std::ostringstream channelName;
        channelName << "CH" << numChannel;
        m_channel.push_back(std::make_shared<Channel>(channelName.str(), rootNode));
    }
    // We have declared all the nodes and PVs in our device: now we register them
    //  with the control system that called this constructor.

    rootNode.initialize(this, factory);

    //Set log level of rootNode: for example -> 0:debug;1:info;2:warning;3:error;4:none;
    nds::logLevel_t rootNodeloglevel=(nds::logLevel_t)0;
    rootNode.setLogLevel(rootNodeloglevel);
    if(rootNode.isLogLevelEnabled(rootNodeloglevel)){
    	switch(rootNodeloglevel){
    	case 0:
        	std::cout << "LogLevel of " << rootNode.getComponentName() << " is set to debug" << std::endl;
    		break;

    	case 1:
        	std::cout << "LogLevel of " << rootNode.getComponentName() << " is set to info" << std::endl;
    		break;

    	case 2:
        	std::cout << "LogLevel of " << rootNode.getComponentName() << " is set to warning" << std::endl;
    		break;

    	case 3:
        	std::cout << "LogLevel of " << rootNode.getComponentName() << " is set to error" << std::endl;
    		break;

    	default:
        	std::cout << "LogLevel of " << rootNode.getComponentName() << " is set to none" << std::endl;
    		break;
    	}
    }else{
    	std::cout << "LogLevel of " << rootNode.getComponentName() << " NOT enabled" << std::endl;
    }
}

//
// Constructor for an acquisition ChannelGroup
//
ChannelGroup::ChannelGroup(const std::string &name, nds::Node &parentNode)
{
    nds::Node channelgroup = parentNode.addChild(nds::Node(name));
    samples_per_channelgroup=100;
    // We create an acquisition node with the requested name...
#ifdef DAQ
    m_dataAcquisition = nds::DataAcquisition<std::vector<std::int32_t> >("DAQGroup",
    																 samples_per_channelgroup,
                                                                     std::bind(&ChannelGroup::switchOn, this),
                                                                     std::bind(&ChannelGroup::switchOff, this),
                                                                     std::bind(&ChannelGroup::start, this),
                                                                     std::bind(&ChannelGroup::stop, this),
                                                                     std::bind(&ChannelGroup::recover, this),
                                                                     std::bind(&ChannelGroup::allowChange, this,
                                                                               std::placeholders::_1,
                                                                               std::placeholders::_2,
                                                                               std::placeholders::_3));
    // ...and we add it to the root
    channelgroup.addChild(m_dataAcquisition);
#endif
    // We also add to the acquisition node a PV that specifies the amplitude of the
    //  generated wave
    m_amplitude = nds::PVVariableOut<std::int32_t>("Amplitude");
    m_amplitude.setDescription("Amplitude of the sine signal" );
    m_amplitude.setScanType((nds::scanType_t)0, (double)1);
    m_amplitude.setUnits("Volts");
    channelgroup.addChild(m_amplitude);


}

//
// Called when the acquisition node has to be switched on.
// Here we do nothing (in our case no special operations are needed to switch
//  on the node).
//
void ChannelGroup::switchOn()
{
#ifdef DAQ
	ndsDebugStream(this->m_dataAcquisition) << "The ChannelGroup " << this->m_dataAcquisition.getFullName() << " has been switched ON." << std::endl;
#endif
}
//
// Called when the acquisition node has to be switched off.
// Here we do nothing (in our case no special operations are needed to switch
//  off the node).
//
void ChannelGroup::switchOff()
{
#ifdef DAQ
	ndsDebugStream(this->m_dataAcquisition) << "The ChannelGroup " << this->m_dataAcquisition.getFullName() << " has been switched OFF." << std::endl;
#endif

}
//
// Called when the acquisition node has to start acquiring.
// We start the data acquisition thread for the sinusoidal wave
//
void ChannelGroup::start()
{
    m_bStopDataAcquisition = false; //< We will set to true to stop the acquisition thread
    // Start the acquisition thread.
    // We don't need to check if the thread was already started because the state
    //  machine guarantees that the start handler is called only while the state
    //  is ON.
#ifdef DAQ
    m_dataAcquisitionThread = m_dataAcquisition.runInThread("DAQGroupThread", std::bind(&ChannelGroup::DataAcquisitionLoop, this));
	ndsDebugStream(this->m_dataAcquisition) << "The ChannelGroup " << this->m_dataAcquisition.getFullName() << " has been started." << std::endl;
#endif

}
//
// Stop the acquisition thread
//
void ChannelGroup::stop()
{
    m_bStopDataAcquisition = true;
#ifdef DAQ
    m_dataAcquisitionThread.join();
	ndsDebugStream(this->m_dataAcquisition) << "The ChannelGroup " << this->m_dataAcquisition.getFullName() << " has been stopped." << std::endl;
#endif

}
//
// A failure during a state transition will cause the state machine to switch
//  to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force
//  the state machine to stay on the failure state.
//
void ChannelGroup::recover()
{
    throw nds::StateMachineRollBack("Cannot recover");
}
//
// We always allow the state machine to switch state.
// Before calling this function the state machine has already verified that the
//  requested state transition is legal.
//
bool ChannelGroup::allowChange(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}
//
// Acquisition function. It is launched in a separate thread by start().
//
void ChannelGroup::DataAcquisitionLoop()
{
#ifdef DAQ
	ndsDebugStream(this->m_dataAcquisition) << "Entering in the ChannelGroup " << this->m_dataAcquisition.getFullName() << " DataAcquisitionLoop" << std::endl;
    // Let's allocate a vector that will contain the data that we will push to the
    //  control system
    std::vector<std::int32_t> outputData(m_dataAcquisition.getMaxElements());
    // A counter for the angle in the sin() operation
    std::int64_t angle(0);

    //Channel group number
    std::string ch_name = m_dataAcquisition.getFullName();
    boost::regex pattern(".*?CHGR(\\d+).*");
    boost::smatch result;
    bool match = boost::regex_match(ch_name, result, pattern);
    int chgr_number=0;
    if(match){
    	chgr_number=boost::lexical_cast<int>(result[1]);
    }else{
    	std::cout << "CHGR NOT found"<< std::endl;
    }
    // Run until the state machine stops us
    while(!m_bStopDataAcquisition)
    {
        // Fill the vector with a sin wave
        size_t maxAmplitude = m_amplitude.getValue(); // PVVariables are thread safe
        for(size_t scanVector(0); scanVector != outputData.size(); ++scanVector)
        {
            outputData[scanVector] = chgr_number+1;
            //outputData[scanVector] = (double)maxAmplitude * sin((double)(angle++) / 10.0f);
        }
        // Push the vector to the control system
        m_dataAcquisition.push(m_dataAcquisition.getTimestamp(), outputData);
        // Rest for a while
        ::usleep(100000);
    }
#endif
}

//
// Constructor for an acquisition Channel
//
Channel::Channel(const std::string &name, nds::Node &parentNode)
{
    nds::Node Channel = parentNode.addChild(nds::Node(name));
    samples_per_channel=100;
    // We create an acquisition node with the requested name...
#ifdef DAQ
    m_dataAcquisition = nds::DataAcquisition<std::vector<std::int32_t> >("DAQChannel",
    																 samples_per_channel,
                                                                     std::bind(&Channel::switchOn, this),
                                                                     std::bind(&Channel::switchOff, this),
                                                                     std::bind(&Channel::start, this),
                                                                     std::bind(&Channel::stop, this),
                                                                     std::bind(&Channel::recover, this),
                                                                     std::bind(&Channel::allowChange, this,
                                                                               std::placeholders::_1,
                                                                               std::placeholders::_2,
                                                                               std::placeholders::_3));
    // ...and we add it to the root
    Channel.addChild(m_dataAcquisition);
#endif
    // We also add to the acquisition node a PV that specifies the amplitude of the
    //  generated wave
    m_amplitude = nds::PVVariableOut<std::int32_t>("Amplitude");
    m_amplitude.setDescription("Amplitude of the sine signal" );
    m_amplitude.setScanType((nds::scanType_t)0, (double)1);
    m_amplitude.setUnits("Volts");
    Channel.addChild(m_amplitude);

}

//
// Called when the acquisition node has to be switched on.
// Here we do nothing (in our case no special operations are needed to switch
//  on the node).
//
void Channel::switchOn()
{
	ndsDebugStream(this->m_dataAcquisition) << "The Channel " << this->m_dataAcquisition.getFullName() << " has been switched ON." << std::endl;

}
//
// Called when the acquisition node has to be switched off.
// Here we do nothing (in our case no special operations are needed to switch
//  off the node).
//
void Channel::switchOff()
{
	ndsDebugStream(this->m_dataAcquisition) << "The Channel " << this->m_dataAcquisition.getFullName() << " has been switched ON." << std::endl;

}
//
// Called when the acquisition node has to start acquiring.
// We start the data acquisition thread for the sinusoidal wave
//
void Channel::start()
{
    m_bStopDataAcquisition = false; //< We will set to true to stop the acquisition thread
    // Start the acquisition thread.
    // We don't need to check if the thread was already started because the state
    //  machine guarantees that the start handler is called only while the state
    //  is ON.
#ifdef DAQ
    m_dataAcquisitionThread = m_dataAcquisition.runInThread("DAQChannelThread", std::bind(&Channel::DataAcquisitionLoop, this));
	ndsDebugStream(this->m_dataAcquisition) << "The Channel " << this->m_dataAcquisition.getFullName() << " has been switched ON." << std::endl;
#endif

}
//
// Stop the acquisition thread
//
void Channel::stop()
{
    m_bStopDataAcquisition = true;
    m_dataAcquisitionThread.join();
	ndsDebugStream(this->m_dataAcquisition) << "The Channel " << this->m_dataAcquisition.getFullName() << " has been switched ON." << std::endl;
}
//
// A failure during a state transition will cause the state machine to switch
//  to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force
//  the state machine to stay on the failure state.
//
void Channel::recover()
{
    throw nds::StateMachineRollBack("Cannot recover");
}
//
// We always allow the state machine to switch state.
// Before calling this function the state machine has already verified that the
//  requested state transition is legal.
//
bool Channel::allowChange(const nds::state_t, const nds::state_t, const nds::state_t)
{
    return true;
}
//
// Acquisition function. It is launched in a separate thread by start().
//
void Channel::DataAcquisitionLoop()
{
#ifdef DAQ
	ndsDebugStream(this->m_dataAcquisition) << "Entering in the Channel " << this->m_dataAcquisition.getFullName() << " DataAcquisitionLoop" << std::endl;

    // Let's allocate a vector that will contain the data that we will push to the
    //  control system
    std::vector<std::int32_t> outputData(m_dataAcquisition.getMaxElements());
    // A counter for the angle in the sin() operation
    std::int64_t angle(0);

    //Channel number
    std::string ch_name = m_dataAcquisition.getFullName();
    boost::regex pattern(".*?CH(\\d+).*");
    boost::smatch result;
    bool match = boost::regex_match(ch_name, result, pattern);
    int ch_number=0;
    if(match){
    	ch_number=boost::lexical_cast<int>(result[1]);
    }else{
    	std::cout << "CH NOT found"<< std::endl;
    }

    // Run until the state machine stops us
    while(!m_bStopDataAcquisition)
    {
        // Fill the vector with a sin wave
        size_t maxAmplitude = m_amplitude.getValue(); // PVVariables are thread safe
        for(size_t scanVector(0); scanVector != outputData.size(); ++scanVector)
        {
           	//outputData[scanVector] = (double)maxAmplitude * sin((double)(angle++) / 10.0f);
        	outputData[scanVector] = ch_number;
        }
        // Push the vector to the control system
        m_dataAcquisition.push(m_dataAcquisition.getTimestamp(), outputData);

        // Rest for a while
        ::usleep(100000);
    }
#endif
}
// The following MACRO defines the function to be exported in order
//  to allow the dynamic loading of the shared module
///////////////////////////////////////////////////////////////////
NDS_DEFINE_DRIVER("DeviceSimulator", DeviceSimulator)
