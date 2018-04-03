#include <nds3/nds.h>
#include <mutex>
#include <unistd.h>
#include <functional>

#include "DeviceTimestamping.h"

#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceTimestamping*> m_DevicesMap;
static std::mutex m_lockDevicesMap;


DeviceTimestamping::DeviceTimestamping(nds::Factory &factory,
				       const std::string &DeviceName,
				       const nds::namedParameters_t &parameters):
  m_Name(DeviceName), m_Ntimestamps(0),
  m_bStop_Timestamping(true){

  //Verify that there is no devices of this type with the same name
  {
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    if(m_DevicesMap.find(DeviceName) != m_DevicesMap.end()) {

      throw std::logic_error("Device with the same name already allocated. "
			     "This should not happen");
    }
    m_DevicesMap[DeviceName] = this;
  }

  /**
   * Here we declare the root node.
   * It is a good practice to name it with the Device name.
   *
   * Also, for simplicity we declare it as a "Port": this means that
   * the root node will be responsible for the communication with
   * the underlying control system.
   *
   * It is possible to have the root node as a simple Node and promote one or
   * more of its children to "Port": each port will interface with a different
   * control system thread.
   */
  nds::Port rootNode(DeviceName);

  //Add a PV to set the current time
  m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));
  m_setCurrentTime.setDescription("Set timestamp (in seconds)");
  // For testing purposes the current time is set to a constant bigger
  // than 1 of January of 1990 which is the EPICS epoch.
  timespec timestamp = {0, 0};
  m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);


  // Add Timestamping node
  m_Timestamping = rootNode.addChild(nds::Timestamping<std::vector<std::int32_t>>(
        "Timestamping", 4,
        std::bind(&DeviceTimestamping::switchOn_timestamping, this),
        std::bind(&DeviceTimestamping::switchOff_timestamping, this),
        std::bind(&DeviceTimestamping::start_timestamping, this),
        std::bind(&DeviceTimestamping::stop_timestamping, this),
        std::bind(&DeviceTimestamping::recover_timestamping, this),
        std::bind(&DeviceTimestamping::allow_timestamping_change, this,
          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&DeviceTimestamping::pv_enable_writer, this,
          std::placeholders::_1, std::placeholders::_2),
        std::bind(&DeviceTimestamping::pv_edge_writer, this,
          std::placeholders::_1, std::placeholders::_2)));

  m_Timestamping.setStartTimestampDelegate(std::bind(&DeviceTimestamping::getCurrentTime, this));

  // We have declared all the nodes and PVs in our Device: now we register them
  // with the control system that called this constructor.
  //////////////////////////////////////////////////////////////////////////////
  rootNode.initialize(this, factory);
  rootNode.setTimestampDelegate(std::bind(&DeviceTimestamping::getCurrentTime, this));

  //Stream information for debugging purposes
  rootNode.setLogLevel(nds::logLevel_t::debug);
  rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger: "
					     << "The DeviceTimestamping is created"
					     << std::endl;
  ndsDebugStream(rootNode) << "This is the ndsDebugStream: "
			   << "The DeviceTimestamping named "
			   << rootNode.getFullName() << " is created"
			   << std::endl;

}

DeviceTimestamping::~DeviceTimestamping() {

  std::lock_guard<std::mutex> lock(m_lockDevicesMap);
  m_DevicesMap.erase(m_Name);

}

DeviceTimestamping* DeviceTimestamping::getInstance(const std::string& DeviceName) {

  std::lock_guard<std::mutex> lock(m_lockDevicesMap);
  std::map<std::string, DeviceTimestamping*>::const_iterator findDevice =
    m_DevicesMap.find(DeviceName);
  if(findDevice == m_DevicesMap.end()){

    return 0;
  }

  return findDevice->second;
}

////////////////////////////////////////////////////////////////////////////////
// TIMESTAMPING STATE MACHINE FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief called when the Timestamping node has to be switched on
 */
void  DeviceTimestamping::switchOn_timestamping() {

  m_Timestamping.setEnable(getCurrentTime(), 0 /* OFF */);
  m_Timestamping.setEdge(getCurrentTime(), 1 /* RISING */);
  m_Timestamping.setMaxTimestamps(getCurrentTime(), 5);
  m_Timestamping.setOverflow(getCurrentTime(), 0 /* NO */);
}


/**
 * @brief called when the Timestamping node has to be switched off
 */
void DeviceTimestamping::switchOff_timestamping() {

  m_Timestamping.setEnable(getCurrentTime(), 0 /* OFF */);
}

/**
 * @brief called withn the Timestamping node has to start working.
 *        we start the Timestamping thread.
 */
void DeviceTimestamping::start_timestamping() {

  m_bStop_Timestamping = false; //< We will set to true to stop the Timestamping
				//  thread.

  /**
   *  Start the Timestamping thread.
   *  We don't need to check if the thread was already started because the state
   *  machine guarantees that the start handler is called only while the state
   *  is ON.
   */
  m_timestamping_thread =
    std::thread(std::bind(&DeviceTimestamping::timestamping_thread_body, this));
}

// Stop the Timestamping node thread
void DeviceTimestamping::stop_timestamping(){

  m_bStop_Timestamping = true;
  m_timestamping_thread.join();
}

// A failure during a state transition will cause the state machine to switch to
// the failure state. For now we don't plan for this and every time the
// state machine wants to recover we throw StateMachineRollBack to force the
// state machine to stay on the failure state.
void DeviceTimestamping::recover_timestamping(){

  throw nds::StateMachineRollBack("Cannot recover");
}

// Always allow for change state.
bool DeviceTimestamping::allow_timestamping_change(nds::state_t, nds::state_t, nds::state_t){

  return true;
}

void DeviceTimestamping::timestamping_thread_body() {

  // Get status.
  std::int32_t enable = m_Timestamping.getEnable();
  // Get edge status.
  std::int32_t edge = m_Timestamping.getEdge();
  // Get maximum number of timestamps.
  std::int32_t max_tstamps = m_Timestamping.getMaxTimestamps();
  // Get overflow.
  std::int32_t overflow = m_Timestamping.getOverflow();

  std::cout << "Timestamping support information:" << std::endl;
  std::cout << "\tEnable = " << enable << std::endl;
  std::cout << "\tEdge = " << edge << std::endl;
  std::cout << "\tMaximum number of timestamps = " << max_tstamps << std::endl;
  std::cout << "\tOverflow state = " << overflow << std::endl;

  // Run until the state machine stops us
  while(!m_bStop_Timestamping){

    enable = m_Timestamping.getEnable();

    if (enable == 1 /* Enabled */) {

      // Six timestamps are  going to be pushed:
      // First timestamp
      std::vector<std::int32_t> pushed_timestamp = {0, 0, 0 /* RISING */,
						    ++m_Ntimestamps /* ID */};
      push_timestamp(max_tstamps, pushed_timestamp);
      // m_Timestamping.push(m_Timestamping.getTimestamp(), pushed_timestamp);

      // Second timestamp
      pushed_timestamp[1] = 10; /* nsec */
      pushed_timestamp[3] = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);

      // Third Timestamp
      pushed_timestamp[3] = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);

      // Fourth Timestamp
      pushed_timestamp[2] = 1; /* FALLING */
      pushed_timestamp[3] = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);

      // Fifth Timestamp
      pushed_timestamp[3] = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);

      // Sixth Timestamp
      pushed_timestamp[2] = 0; /* ID */
      pushed_timestamp[3] = ++m_Ntimestamps; /* ID */
      push_timestamp(max_tstamps, pushed_timestamp);

      // TODO: ClearOverflow needs to be implemented. For the moment we clear
      // the overflow state manually:
      if (pushed_timestamp[3] >= max_tstamps) {
	m_Ntimestamps = 0; /* ID resetted */
	m_Timestamping.setOverflow(getCurrentTime(), 0 /* NO */);
      }

      ::usleep(1000000);
    }
  }
}

/*
 * Enable writer.
 */
void DeviceTimestamping::pv_enable_writer(const timespec& timestamp,
					  const std::int32_t& value){

  // TODO: This function shall interact with hardware api to enable or disbale.

  m_Timestamping.setEnable(timestamp, value);

}

/*
 * Edge writer.
 */
void DeviceTimestamping::pv_edge_writer(const timespec& timestamp,
					const std::int32_t& value){

  // This function shall interact with api hardware.
  m_Timestamping.setEdge(timestamp, value);

}

void DeviceTimestamping::push_timestamp(std::int32_t max_tstamps,
					std::vector<std::int32_t> pushed_tstamp){

  // Here we check manually the amount of timestamps in the queue.
  if(pushed_tstamp[3] > max_tstamps) {

    m_Timestamping.setOverflow(getCurrentTime(), 1 /* OVERFLOWED */);
  } else if (pushed_tstamp[3] == max_tstamps){

    m_Timestamping.setOverflow(getCurrentTime(), 2 /* FULL */);
  }

  /* The timestamp is always pushed, even if there is an overflow. */
  m_Timestamping.push(m_Timestamping.getTimestamp(), pushed_tstamp);

}

timespec DeviceTimestamping::getCurrentTime() {
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

#ifdef EPICS
NDS_DEFINE_DRIVER(DeviceTimestamping, DeviceTimestamping)
#else
/*
 * Allocation function
 *********************/
void* DeviceTimestamping::allocateDevice(nds::Factory& factory,
				   const std::string& DeviceName,
				   const nds::namedParameters_t& parameters){

  return new DeviceTimestamping(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceTimestamping::deallocateDevice(void* DeviceName){

  delete (DeviceTimestamping*)DeviceName;
}
#endif
