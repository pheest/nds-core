
#include <nds3/nds.h>
#include <mutex>
#include <unistd.h>
#include <functional>

#include "../include/DeviceTimestamping.h"
#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceTimestamping*> m_DevicesMap;
static std::mutex m_lockDevicesMap;



DeviceTimestamping::DeviceTimestamping(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t &parameters):
  m_Name(DeviceName), m_NTimeStamps(0),
  m_bStop_Timestamping(true)
{
  //Verify that there is no devices of this type with the same name
  {
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    if(m_DevicesMap.find(DeviceName) != m_DevicesMap.end())
    {
      throw std::logic_error("Device with the same name already allocated. This should not happen");
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
  m_Timestamping = rootNode.addChild(nds::Timestamping<std::vector<std::int32_t>>("Firm",
        std::bind(&DeviceTimestamping::switchOn_Timestamping, this),
        std::bind(&DeviceTimestamping::switchOff_Timestamping, this),
        std::bind(&DeviceTimestamping::start_Timestamping, this),
        std::bind(&DeviceTimestamping::stop_Timestamping, this),
        std::bind(&DeviceTimestamping::recover_Timestamping, this),
        std::bind(&DeviceTimestamping::allow_Timestamping_Change, this,
          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&DeviceTimestamping::PV_Enable_Writer, this,
          std::placeholders::_1, std::placeholders::_2),
        std::bind(&DeviceTimestamping::PV_Edge_Writer, this,
          std::placeholders::_1, std::placeholders::_2)));

  m_Timestamping.setTimestampDelegate(std::bind(&DeviceTimestamping::getCurrentTime,this));

  // We have declared all the nodes and PVs in our Device: now we register them
  // with the control system that called this constructor.
  ////////////////////////////////////////////////////////////////////////////////
  rootNode.initialize(this, factory);
  rootNode.setTimestampDelegate(std::bind(&DeviceTimestamping::getCurrentTime,this));

  //Stream information for debugging purposes
  rootNode.setLogLevel(nds::logLevel_t::debug);
  rootNode.getLogger(nds::logLevel_t::debug) <<
    "This is the debugging logger:The DeviceTimestamping is created" << std::endl;
  ndsDebugStream(rootNode) <<
    "This is the ndsDebugStream: The DeviceTimestamping named "
    << rootNode.getFullName() << " is created" << std::endl;
}

DeviceTimestamping::~DeviceTimestamping()
{
  std::lock_guard<std::mutex> lock(m_lockDevicesMap);
  m_DevicesMap.erase(m_Name);
}

DeviceTimestamping* DeviceTimestamping::getInstance(const std::string& DeviceName)
{
  std::lock_guard<std::mutex> lock(m_lockDevicesMap);
  std::map<std::string, DeviceTimestamping*>::const_iterator findDevice =
    m_DevicesMap.find(DeviceName);
  if(findDevice == m_DevicesMap.end())
  {
    return 0;
  }
  return findDevice->second;
}

////////////////////////////////////////////////////////////////////////////////
// TIMESTAMPING STATE MACHINE FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief called when the Timestamping node has to be switched on 
 * */
void  DeviceTimestamping::switchOn_Timestamping() {
  // set Timestamping status to 1
  m_Timestamping.setEnable(getCurrentTime(), 1);
  // set Edge to RISING (1) 
  m_Timestamping.setEdge(getCurrentTime(), 1);
  // set maxTimestamps  to 5 
  m_Timestamping.setMaxTimestamps(getCurrentTime(), 5);
  // set Overflow to NO (0)
  m_Timestamping.setOverflow(getCurrentTime(), 0);
}


/**
 * @brief called when the Timestamping node has to be switched off 
 * */
void DeviceTimestamping::switchOff_Timestamping() {
  // set Timestamping status to 0
  m_Timestamping.setEnable(getCurrentTime(), 0);
}

/** 
 * @brief called withn the Timestamping node has to start working. 
 *        we start the Timestamping thread.
 * */
void DeviceTimestamping::start_Timestamping() {

  m_bStop_Timestamping = false; //< We will set to true to stop the Timestamping thread
  /**
   *   Start the Timestamping thread.
   *   We don't need to check if the thread was already started because the state
   *   machine guarantees that the start handler is called only while the state
   *   is ON.
   */
  m_Timestamping_thread = std::thread(std::bind(&DeviceTimestamping::timing_thread_body, this));
 
}
