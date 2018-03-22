#ifndef DEVICE_TIMESTAMPING_H_
#define DEVICE_TIMESTAMPING_H_

#include <memory>

#include <functional>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a Timestamping node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceTimeStamping
{
public:
  /**
   * @brief Constructor.
   *
   * @param factory    the control system factory that requested the creation of the device
   * @param device     the name given to the device
   * @param parameters optional parameters passed to the device
   */
  DeviceTimeStamping(nds::Factory& factory, const std::string& deviceName, 
      const nds::namedParameters_t& );
  ~DeviceTimeStamping();

#ifndef EPICS
  /*
   * Allocation/deallocation
   *
   *******************************************************/
  static void* allocateDevice(nds::Factory& factory, 
      const std::string& deviceName, 
      const nds::namedParameters_t& parameters);
  static void deallocateDevice(void* deviceName);
#endif

  /**
   * For test purposes we make it possible to retrieve running instances of
   *  the device
   */
  static DeviceTimeStamping* getInstance(const std::string& deviceName);


private:
  /**
   * @brief name of the device
   */
  std::string m_Name;

  /**
   * @brief number of generated timestamps
   * */
  std::int32_t m_Ntimestamps;

///////////////////////////////////////////////////////////////////////////////
// TEST TIMING NODE 
//////////////////////////////////////////////////////////////////////////////
  /**
   * Methods to control the TimeStamping state machine
   */
  void switchOn_TimeStamping();  ///< Called to switch on the TimeStamping node.
  void switchOff_TimeStamping(); ///< Called to switch off the TimeStamping node.
  void start_TimeStamping();     ///< Called to start the TimeStamping node.
  void stop_TimeStamping();      ///< Called to stop the TimeStamping node.
  void recover_TimeStamping();   ///< Called to recover the TimeStamping node from a failure.

  bool allow_Device_Change(const nds::state_t,
      const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed


  /**
   * TimeStamping setters
   */
  void PV_Enable_Writer(const timespec& timestamp, const std::int32_t& value);
  void PV_Edge_Writer(const timespec& timestamp, const std::int32_t& value);

  /**
   * @brief A thread that runs TimeStamping_thread_body().
   */
  std::thread m_TimeStamping_Thread;

  /**
   * @brief A boolean flag that stop the TimeStamping loop in TimeStamping_thread_body()
   *        when true.
   */
  volatile bool m_bStop_TimeStamping;

///////////////////////////////////////////////////////////////////////////////
// TIMESTAMP HANDLING
//////////////////////////////////////////////////////////////////////////////
  /**
   * @brief PV to set the timestamp of the device, in seconds
   */
  nds::PVVariableOut<std::int32_t> m_setCurrentTime;

  /**
   * @brief Function to get the timestamp of the device
   */
  timespec getCurrentTime();

};


#endif // DEVICE_TIMESTAMPING_H
