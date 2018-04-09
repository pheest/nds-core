#include <functional>
#include <sstream>
#include <iostream>

#include "simulated_signal.h"

#include <nds3/nds.h>
#include "oscilloscope.h"

oscilloscope::oscilloscope(nds::Factory& factory,
			   const std::string& device,
			   const nds::namedParameters_t& paramters){

  nds::Port rootNode(device);

  /* Initializing data acquisition channels. */
  size_t maxCh = 2; /* Number of channels. */
  for(size_t i = 0; i < maxCh; i++){

    std::ostringstream chName;
    chName << "CH" << i;
    m_channels.push_back(std::make_shared<channel>(chName.str(), rootNode));
  }

  /* Initializing terminal with time and timestamping functionalities. */
  m_terminals.push_back(std::make_shared<terminal>("TR", rootNode));

  /* Initializing firmware node. */
  m_firmware = rootNode.addChild(nds::Firmware("Firmware", /* Node name*/
	 256, /* Max. string length. */
	 std::bind(&oscilloscope::switchon_firmware, this),
	 std::bind(&oscilloscope::switchoff_firmware, this),
	 std::bind(&oscilloscope::start_firmware, this),
	 std::bind(&oscilloscope::stop_firmware, this),
	 std::bind(&oscilloscope::recover_firmware, this),
	 std::bind(&oscilloscope::allow_firmware_change,
		   this, std::placeholders::_1,
		   std::placeholders::_2, std::placeholders::_3),
	 std::bind(&oscilloscope::pv_path_writer, this,
		   std::placeholders::_1, std::placeholders::_2)));

  rootNode.initialize(this, factory);

}



oscilloscope::~oscilloscope(){

}

/* Firmware methods. */

void oscilloscope::switchon_firmware(){

  /*Call to HW to get a firmware description. */
  hw_firm firmware = firmware_description();

  // Call API HW to retrieve FirmwareVersion
  m_firmware.setFirmwareVersion(m_firmware.getTimestamp(),
				firmware.version);
  // Call API HW to retrieve FirmwareStatus
  m_firmware.setFirmwareStatus(m_firmware.getTimestamp(),
			       firmware.status);
  // Call API HW to retrieve HardwareRevision
  m_firmware.setHardwareRevision(m_firmware.getTimestamp(),
				 firmware.revision);
  // Call API HW to retrieve SerialNumber
  m_firmware.setSerialNumber(m_firmware.getTimestamp(),
			     firmware.serialNumber);
  // Call API HW to retrieve DeviceModel
  m_firmware.setDeviceModel(m_firmware.getTimestamp(),
			    firmware.deviceModel);
  // Call API HW to retrieve DeviceType
  m_firmware.setDeviceType(m_firmware.getTimestamp(),
			   firmware.deviceType);
  // Call API HW to retrieve DriverVersion
  m_firmware.setDriverVersion(m_firmware.getTimestamp(),
			      firmware.driverVersion);
  // Call API HW to retrieve ChassisNumber
  m_firmware.setChassisNumber(m_firmware.getTimestamp(),
			      firmware.chassisNum);
  // Call API HW to retrieve SlotNumber
  m_firmware.setSlotNumber(m_firmware.getTimestamp(),
			   firmware.slotNum);
  // Call API HW to retrieve FirmwarePath
  m_firmware.setFirmwarePath(m_firmware.getTimestamp(),
			     firmware.firmPath);
}



void oscilloscope::switchoff_firmware(){

}



void oscilloscope::start_firmware(){

  m_stop_firmware = false; //< We will set to true to stop the Firmware thread
  /**
   *  Start the Firmware thread.
   *  We don't need to check if the thread was already started because the state
   *  machine guarantees that the start handler is called only while the state
   *  is ON.
   */
  m_firmware_thread = std::thread(std::bind(&oscilloscope::firmware_thread_body,
					    this));
}



void oscilloscope::stop_firmware(){
  m_stop_firmware = true;
  m_firmware_thread.join();
}



void oscilloscope::recover_firmware(){
  throw nds::StateMachineRollBack("Cannot recover");
}



bool oscilloscope::allow_firmware_change(const nds::state_t,
					 const nds::state_t,
					 const nds::state_t){
  return true;
}



void oscilloscope::pv_path_writer(const timespec& timestamp,
				  const std::string& value){


  // Call to function programming the hardware. This function returns the
  // real firmware path programmed. This value has to be set to the readback attribute.
  hw_firm new_firmware = change_hw_firmware();

  std::string new_firmwarePath = new_firmware.firmPath;

  // firmwarePath has the new firmware path to be programmed on the hardware.
  m_firmware.setFirmwarePath(timestamp, new_firmwarePath);
}



void oscilloscope::firmware_thread_body(){

  // Get FirmwareVersion
  std::string FirmwareVersion = m_firmware.getFirmwareVersion();
  // Get FirmwareStatus
  std::int32_t FirmwareStatus = m_firmware.getFirmwareStatus();
  // Get HardwareRevision
  std::string HardwareRevision = m_firmware.getHardwareRevision();
  // Get SerialNumber
  std::string SerialNumber = m_firmware.getSerialNumber();
  // Get DeviceModel
  std::string DeviceModel = m_firmware.getDeviceModel();
  // Get DeviceType
  std::string DeviceType = m_firmware.getDeviceType();
  // Get DriverVersion
  std::string DriverVersion = m_firmware.getDriverVersion();
  // Get ChassisNumber
  std::int32_t ChassisNumber = m_firmware.getChassisNumber();
  // Get SlotNumber
  std::int32_t SlotNumber = m_firmware.getSlotNumber();
  // Get FirmwarePath
  std::string FirmwarePath = m_firmware.getFirmwarePath();
  std::string FirmwarePathOld = m_firmware.getFirmwarePath();


  std::cout << "Firmware support information:" << std::endl;
  std::cout << "\tFirmwareVersion = " << FirmwareVersion<<std::endl;
  std::cout << "\tFirmwareStatus = " << FirmwareStatus << std::endl;
  std::cout << "\tHardwareRevision = " << HardwareRevision << std::endl;
  std::cout << "\tSerialNumber = " << SerialNumber << std::endl;
  std::cout << "\tDeviceModel = " << DeviceModel << std::endl;
  std::cout << "\tDeviceType = " << DeviceType << std::endl;
  std::cout << "\tDriverVersion = " << DriverVersion << std::endl;
  std::cout << "\tChassisNumber = " << ChassisNumber << std::endl;
  std::cout << "\tSlotNumber = " << SlotNumber << std::endl;
  std::cout << "\tFirmwarePath = " << FirmwarePath << std::endl;

  // Run until the state machine stops us
  while(!m_stop_firmware){


    // Get FirmwarePath
    std::string FirmwarePath = m_firmware.getFirmwarePath();
    if(FirmwarePath.compare(FirmwarePathOld) != 0){
      // Push the Firmware data to the control system
      m_firmware.push(m_firmware.getTimestamp(), FirmwarePath);
      FirmwarePathOld = FirmwarePath;
    }
    // Rest for a while
    ::usleep(1000000);
  }
}

/* End firmware methods. */

#ifdef EPICS
NDS_DEFINE_DRIVER(oscilloscope, oscilloscope)
#else
/*
 * Allocation function
 *********************/
void* oscilloscope::allocateDevice(nds::Factory& factory,
					 const std::string& DeviceName,
					 const nds::namedParameters_t& parameters){

  return new oscilloscope(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void oscilloscope::deallocateDevice(void* DeviceName){

  delete (oscilloscope*)DeviceName;
}
#endif
