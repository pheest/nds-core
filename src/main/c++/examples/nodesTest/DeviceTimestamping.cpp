
#include <nds3/nds.h>
#include <mutex>
#include <unistd.h>
#include <functional>

#include "../include/DeviceTimeStamping.h"
#define NDS_EPOCH 1514764800 /* 00:00 of 1/1/2018 in UTC format. */

static std::map<std::string, DeviceTimeStamping*> m_DevicesMap;
static std::mutex m_lockDevicesMap;



//MODIFICAR!!!
DeviceTimeStamping::DeviceTimeStamping(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t &parameters):
            m_Name(DeviceName),
            m_bStop_TimeStamping(true)
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
  m_setCurrentTime.setDescription("Set timestamp (in secodns)");
  // For testing purposes the current time is set to a constant bigger
  // than 1 of January of 1990 which is the EPICS epoch.
  timespec timestamp = {0, 0};
  m_setCurrentTime.write(timestamp, (std::int32_t)NDS_EPOCH);


  // Add TimeStamping node
  m_TimeStamping = rootNode.addChild(nds::TimeStamping<std::vector<std::int32_t>>("Firm",
        std::bind(&DeviceTimeStamping::switchOn_TimeStamping, this),
        std::bind(&DeviceTimeStamping::switchOff_TimeStamping, this),
        std::bind(&DeviceTimeStamping::start_TimeStamping, this),
        std::bind(&DeviceTimeStamping::stop_TimeStamping, this),
        std::bind(&DeviceTimeStamping::recover_TimeStamping, this),
        std::bind(&DeviceTimeStamping::allow_TimeStamping_Change, this,
          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&DeviceTimeStamping::PV_Enable_Writer, this,
          std::placeholders::_1, std::placeholders::_2),
        std::bind(&DeviceTimeStamping::PV_Edge_Writer, this,
          std::placeholders::_1, std::placeholders::_2)));

  m_TimeStamping.setTimestampDelegate(std::bind(&DeviceTimeStamping::getCurrentTime,this));

  // We have declared all the nodes and PVs in our Device: now we register them
  // with the control system that called this constructor.
  ////////////////////////////////////////////////////////////////////////////////
  rootNode.initialize(this, factory);
  rootNode.setTimestampDelegate(std::bind(&DeviceTimeStamping::getCurrentTime,this));

  //Stream information for debugging purposes
  rootNode.setLogLevel(nds::logLevel_t::debug);
  rootNode.getLogger(nds::logLevel_t::debug) <<
      "This is the debugging logger:The DeviceTimeStamping is created" << std::endl;
  ndsDebugStream(rootNode) <<
      "This is the ndsDebugStream: The DeviceTimeStamping named "
      << rootNode.getFullName() << " is created" << std::endl;
}

DeviceTimeStamping::~DeviceTimeStamping()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_DevicesMap.erase(m_Name);
}

DeviceTimeStamping* DeviceTimeStamping::getInstance(const std::string& DeviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    std::map<std::string, DeviceTimeStamping*>::const_iterator findDevice =
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
//UNFINISHED 
void  DeviceTimestamping::switchOn_DeviceTimestamping() {
  // set Timestamping status to 1
  m_Timestamping.setEnable(getCurrentTime(), 1);
  // set 
  m_Timestamping.setEdge(getCurrentTime(), 1);
}

