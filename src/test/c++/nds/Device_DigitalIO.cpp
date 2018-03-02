
#include <nds3/nds.h>
#include <mutex>
#include <unistd.h>
#include <functional>

#include "../include/Device_DigitalIO.h"

static std::map<std::string, DeviceDigitalIO*> m_devicesMap;
static std::mutex m_lockDevicesMap;

DeviceDigitalIO::DeviceDigitalIO(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t &parameters):
m_name(deviceName),	timestamp_device{0,0},readtimeStamp{0,0}{
	//TODO:Study this.
	{
		std::lock_guard<std::mutex> lock(m_lockDevicesMap);
		if(m_devicesMap.find(deviceName) != m_devicesMap.end())
		{
			throw std::logic_error("Device with the same name already allocated. This should not happen");
		}
		m_devicesMap[deviceName] = this;
	}

	/**
	 * Here we declare the root node.
	 * It is a good practice to name it with the device name.
	 *
	 * Also, for simplicity we declare it as a "Port": this means that
	 * the root node will be responsible for the communication with
	 * the underlying control system.
	 *
	 * It is possible to have the root node as a simple Node and promote one or
	 * more of its children to "Port": each port will interface with a different
	 * control system thread.
	 */
	nds::Port rootNode(deviceName);

	// Add state machine
	m_DeviceDigitalIO_stateMachine = rootNode.addChild(nds::StateMachine(true,
			std::bind(&DeviceDigitalIO::switchOn_Device, this),
			std::bind(&DeviceDigitalIO::switchOff_Device, this),
			std::bind(&DeviceDigitalIO::start_Device, this),
			std::bind(&DeviceDigitalIO::stop_Device, this),
			std::bind(&DeviceDigitalIO::recover_Device, this),
			std::bind(&DeviceDigitalIO::allow_Device_Change,this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

	/**
	 * Add a Digital I/O node for boolean PV:
	 */
	m_DigitalIO_Bool = rootNode.addChild(nds::DigitalIO<std::vector<bool>>(
			"DigitalIOBoolNode",
			128,
			std::bind(&DeviceDigitalIO::switchOn_DigitalIO, this),
			std::bind(&DeviceDigitalIO::switchOff_DigitalIO, this),
			std::bind(&DeviceDigitalIO::start_DigitalIO, this),
			std::bind(&DeviceDigitalIO::stop_DigitalIO, this),
			std::bind(&DeviceDigitalIO::recover_DigitalIO, this),
			std::bind(&DeviceDigitalIO::allow_DigitalIO_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer,this, std::placeholders::_1, std::placeholders::_2)
	));
	m_DigitalIO_Bool.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
	m_DigitalIO_Bool.getStartTimestamp();

	/**
	 * Add a Digital I/O node for uint8_t PV:
	 */
	m_DigitalIO_Uint8_t = rootNode.addChild(nds::DigitalIO<std::vector<std::uint8_t>>(
			"DigitalIOUint8_tNode",
			128,
			std::bind(&DeviceDigitalIO::switchOn_DigitalIO, this),
			std::bind(&DeviceDigitalIO::switchOff_DigitalIO, this),
			std::bind(&DeviceDigitalIO::start_DigitalIO, this),
			std::bind(&DeviceDigitalIO::stop_DigitalIO, this),
			std::bind(&DeviceDigitalIO::recover_DigitalIO, this),
			std::bind(&DeviceDigitalIO::allow_DigitalIO_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer,this, std::placeholders::_1, std::placeholders::_2)
	));
	m_DigitalIO_Uint8_t.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
	m_DigitalIO_Uint8_t.getStartTimestamp();

	/**
	 * Add a Digital I/O node for uint16_t PV:
	 */
	m_DigitalIO_Uint16_t = rootNode.addChild(nds::DigitalIO<std::vector<std::uint16_t>>(
			"DigitalIOUint16_tNode",
			128,
			std::bind(&DeviceDigitalIO::switchOn_DigitalIO, this),
			std::bind(&DeviceDigitalIO::switchOff_DigitalIO, this),
			std::bind(&DeviceDigitalIO::start_DigitalIO, this),
			std::bind(&DeviceDigitalIO::stop_DigitalIO, this),
			std::bind(&DeviceDigitalIO::recover_DigitalIO, this),
			std::bind(&DeviceDigitalIO::allow_DigitalIO_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer,this, std::placeholders::_1, std::placeholders::_2)
	));
	m_DigitalIO_Uint16_t.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
	m_DigitalIO_Uint16_t.getStartTimestamp();

	/**
	 * Add a Digital I/O node for uint32_t PV:
	 */
	m_DigitalIO_Uint32_t = rootNode.addChild(nds::DigitalIO<std::vector<std::uint32_t>>(
			"DigitalIOUint32_tNode",
			128,
			std::bind(&DeviceDigitalIO::switchOn_DigitalIO, this),
			std::bind(&DeviceDigitalIO::switchOff_DigitalIO, this),
			std::bind(&DeviceDigitalIO::start_DigitalIO, this),
			std::bind(&DeviceDigitalIO::stop_DigitalIO, this),
			std::bind(&DeviceDigitalIO::recover_DigitalIO, this),
			std::bind(&DeviceDigitalIO::allow_DigitalIO_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer,this, std::placeholders::_1, std::placeholders::_2)
	));
	m_DigitalIO_Uint32_t.setStartTimestampDelegate(std::bind(&DeviceDigitalIO::getCurrentTime,this));
	m_DigitalIO_Uint32_t.getStartTimestamp();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Device STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control Device state machine
 */
void DeviceDigitalIO::switchOn_Device(){

}
void DeviceDigitalIO::switchOff_Device(){

}
void DeviceDigitalIO::start_Device(){

}
void DeviceDigitalIO::stop_Device(){

}
void DeviceDigitalIO::recover_Device(){

}

bool DeviceDigitalIO::allow_Device_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}


/**
* Methods to control DigitalIO state machine
*/
void DeviceDigitalIO::switchOn_DigitalIO(){

}
void DeviceDigitalIO::switchOff_DigitalIO(){

}
void DeviceDigitalIO::start_DigitalIO(){
	m_bStop_DigitalIO = false; //< We will set to true to stop the acquisition thread
		/**
		 *   Start the acquisition thread.
		 *   We don't need to check if the thread was already started because the state
		 *   machine guarantees that the start handler is called only while the state
		 *   is ON.
		 */
	m_DigitalIO_Thread_Bool = std::thread(std::bind(&DeviceDigitalIO::DigitalIO_thread_body_Bool, this));
	m_DigitalIO_Thread_U8	= std::thread(std::bind(&DeviceDigitalIO::DigitalIO_thread_body_U8, this));
	m_DigitalIO_Thread_U16 	= std::thread(std::bind(&DeviceDigitalIO::DigitalIO_thread_body_U16, this));
	m_DigitalIO_Thread_U32 	= std::thread(std::bind(&DeviceDigitalIO::DigitalIO_thread_body_U32, this));

}
void DeviceDigitalIO::stop_DigitalIO(){
	m_bStop_DigitalIO = true;
	m_DigitalIO_Thread_Bool.join();
	m_DigitalIO_Thread_U8.join();
	m_DigitalIO_Thread_U16.join();
	m_DigitalIO_Thread_U32.join();
}
void DeviceDigitalIO::recover_DigitalIO(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

bool DeviceDigitalIO::allow_DigitalIO_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

timespec DeviceDigitalIO::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

/**
* DigitalIO setters
*/
void DeviceDigitalIO::PV_DigitalIO_dataOutMask_Writer(const timespec& timestamp, const std::vector<bool>& value){
	std::vector<bool> HW_value;
	//Value has the dataOutMask to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value = value;
	m_DigitalIO_Bool.setDataOutMask(timestamp,value);
	m_DigitalIO_Uint8_t.setDataOutMask(timestamp,value);
	m_DigitalIO_Uint16_t.setDataOutMask(timestamp,value);
	m_DigitalIO_Uint32_t.setDataOutMask(timestamp,value);
}

void DeviceDigitalIO::PV_DigitalIO_voltLevelHigh_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the voltLevelHigh to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real voltLevelHigh programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DigitalIO_Bool.setVoltLevelHigh(timestamp,HW_value);
	m_DigitalIO_Uint8_t.setVoltLevelHigh(timestamp,HW_value);
	m_DigitalIO_Uint16_t.setVoltLevelHigh(timestamp,HW_value);
	m_DigitalIO_Uint32_t.setVoltLevelHigh(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_voltLevelLow_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the voltLevelLow to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real voltLevelLow programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DigitalIO_Bool.setVoltLevelLow(timestamp,HW_value);
	m_DigitalIO_Uint8_t.setVoltLevelLow(timestamp,HW_value);
	m_DigitalIO_Uint16_t.setVoltLevelLow(timestamp,HW_value);
	m_DigitalIO_Uint32_t.setVoltLevelLow(timestamp,HW_value);
}
void DeviceDigitalIO::PV_DigitalIO_ChannelDir_Writer(const timespec& timestamp, const std::vector<bool>& value){
	std::vector<bool> HW_value;
	//Value has the ChannelDir to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real ChannelDir programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DigitalIO_Bool.setChannelDir(timestamp,HW_value);
	m_DigitalIO_Uint8_t.setChannelDir(timestamp,HW_value);
	m_DigitalIO_Uint16_t.setChannelDir(timestamp,HW_value);
	m_DigitalIO_Uint32_t.setChannelDir(timestamp,HW_value);
}

/**
* Body of function DigitalIO thread.
*/
void DeviceDigitalIO::DigitalIO_thread_body_Bool(){

	// Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
	std::vector<bool> outputData(m_DigitalIO_Bool.getMaxElements(),0);

	//Counter for number of pushed data blocks
	std::int32_t NumberOfPushedDataBlocks(0);

	std::uint8_t value(0);

	// Get RefFrequency
	double VoltLevelHigh = m_DigitalIO_Bool.getVoltLevelHigh();
	// Get DutyCycle
	double VoltLevelLow = m_DigitalIO_Bool.getVoltLevelLow();
	// Get Gain
	std::vector<bool> ChannelDir = m_DigitalIO_Bool.getChannelDir();

	std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
	std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
	//std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


	// Run until the state machine stops us
	while(!m_bStop_DigitalIO){

		size_t scanVector(0);

		for(scanVector=0; scanVector != outputData.size(); ++scanVector){
			outputData[scanVector] = value;
		}
		++value;
		// Push the vector to the control system
		m_DigitalIO_Bool.push(m_DigitalIO_Bool.getTimestamp(), outputData);
		++NumberOfPushedDataBlocks;
		//TODO: Send values to data acquisition node.

		// Rest for a while
		::usleep(100000);
	}
	m_DigitalIO_Bool.setNumberOfPushedDataBlocks(m_DigitalIO_Bool.getTimestamp(),NumberOfPushedDataBlocks);
}


/**
* Body of function DigitalIO thread.
*/
void DeviceDigitalIO::DigitalIO_thread_body_U8(){

	// Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
	std::vector<std::uint8_t> outputData(m_DigitalIO_Uint8_t.getMaxElements(),0);

		//Counter for number of pushed data blocks
	std::int32_t NumberOfPushedDataBlocks(0);

	std::uint8_t value(0);

	// Get RefFrequency
	double VoltLevelHigh = m_DigitalIO_Uint8_t.getVoltLevelHigh();
	// Get DutyCycle
	double VoltLevelLow = m_DigitalIO_Uint8_t.getVoltLevelLow();
	// Get Gain
	std::vector<bool> ChannelDir = m_DigitalIO_Uint8_t.getChannelDir();

	std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
	std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
	//std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


	// Run until the state machine stops us
	while(!m_bStop_DigitalIO){

		size_t scanVector(0);

		for(scanVector=0; scanVector != outputData.size(); ++scanVector){
			outputData[scanVector] = value;
		}
		++value;
		// Push the vector to the control system
		m_DigitalIO_Uint8_t.push(m_DigitalIO_Uint8_t.getTimestamp(), outputData);
		++NumberOfPushedDataBlocks;
		//TODO: Send values to data acquisition node.

		// Rest for a while
		::usleep(100000);
	}
	m_DigitalIO_Uint8_t.setNumberOfPushedDataBlocks(m_DigitalIO_Uint8_t.getTimestamp(),NumberOfPushedDataBlocks);
}

/**
* Body of function DigitalIO thread.
*/
void DeviceDigitalIO::DigitalIO_thread_body_U16(){

	// Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
	std::vector<std::uint16_t> outputData(m_DigitalIO_Uint16_t.getMaxElements(),0);

	//Counter for number of pushed data blocks
	std::int32_t NumberOfPushedDataBlocks(0);

	std::uint8_t value(0);

	// Get RefFrequency
	double VoltLevelHigh = m_DigitalIO_Uint16_t.getVoltLevelHigh();
	// Get DutyCycle
	double VoltLevelLow = m_DigitalIO_Uint16_t.getVoltLevelLow();
	// Get Gain
	std::vector<bool> ChannelDir = m_DigitalIO_Uint16_t.getChannelDir();

	std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
	std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
	//std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


	// Run until the state machine stops us
	while(!m_bStop_DigitalIO){

		size_t scanVector(0);

		for(scanVector=0; scanVector != outputData.size(); ++scanVector){
			outputData[scanVector] = value;
		}
		++value;
		// Push the vector to the control system
		m_DigitalIO_Uint16_t.push(m_DigitalIO_Uint16_t.getTimestamp(), outputData);
		++NumberOfPushedDataBlocks;
		//TODO: Send values to data acquisition node.

		// Rest for a while
		::usleep(100000);
	}
	m_DigitalIO_Uint16_t.setNumberOfPushedDataBlocks(m_DigitalIO_Uint16_t.getTimestamp(),NumberOfPushedDataBlocks);
}


/**
* Body of function DigitalIO thread.
*/
void DeviceDigitalIO::DigitalIO_thread_body_U32(){

	// Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
	std::vector<std::uint32_t> outputData(m_DigitalIO_Uint32_t.getMaxElements(),0);

	//Counter for number of pushed data blocks
	std::int32_t NumberOfPushedDataBlocks(0);

	std::uint8_t value(0);

	// Get RefFrequency
	double VoltLevelHigh = m_DigitalIO_Uint32_t.getVoltLevelHigh();
	// Get DutyCycle
	double VoltLevelLow = m_DigitalIO_Uint32_t.getVoltLevelLow();
	// Get Gain
	std::vector<bool> ChannelDir = m_DigitalIO_Uint32_t.getChannelDir();

	std::cout<<"\tVoltLevelHigh = "<<VoltLevelHigh<<std::endl;
	std::cout<<"\tVoltLevelLow = "<<VoltLevelLow<<std::endl;
	//std::cout<<"\tChannelDir = "<<ChannelDir<<std::endl;


	// Run until the state machine stops us
	while(!m_bStop_DigitalIO){

		size_t scanVector(0);

		for(scanVector=0; scanVector != outputData.size(); ++scanVector){
			outputData[scanVector] = value;
		}
		++value;
		// Push the vector to the control system
		m_DigitalIO_Uint32_t.push(m_DigitalIO_Uint32_t.getTimestamp(), outputData);
		++NumberOfPushedDataBlocks;
		//TODO: Send values to data acquisition node.

		// Rest for a while
		::usleep(100000);
	}
	m_DigitalIO_Uint32_t.setNumberOfPushedDataBlocks(m_DigitalIO_Uint32_t.getTimestamp(),NumberOfPushedDataBlocks);
}
