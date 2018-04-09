#include <functional>
#include <sstream>

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
	 std::bind(&oscilloscope::switchOn_firmware, this),
	 std::bind(&oscilloscope::switchOff_firmware, this),
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

void oscilloscope::switchOn_firmware(){

}



void oscilloscope::switchOff_firmware(){

}



void oscilloscope::start_firmware(){

}



void oscilloscope::stop_firmware(){

}



void oscilloscope::recover_firmware(){

}



bool oscilloscope::allow_firmware_change(const nds::state_t,
					 const nds::state_t,
					 const nds::state_t){

}



void oscilloscope::pv_path_writer(const timespec& timestamp, const std::string& value){

}



void oscilloscope::firmware_thread_body(){
// RELLENAR AQUI TODO EL CURRO!!
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
