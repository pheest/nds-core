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
class DeviceTimestamping
{
public:
  /**
   * @brief Constructor.
   *
   * @param factory    the control system factory that requested the creation of the device
   * @param device     the name given to the device
   * @param parameters optional parameters passed to the device
   */
  DeviceTimestamping(nds::Factory& factory, const std::string& deviceName,
      const nds::namedParameters_t& );
  ~DeviceTimestamping();

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
  static DeviceTimestamping* getInstance(const std::string& deviceName);


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
   * Methods to control the Timestamping state machine
   */
  void switchOn_Timestamping();  ///< Called to switch on the Timestamping node.
  void switchOff_Timestamping(); ///< Called to switch off the Timestamping node.
  void start_Timestamping();     ///< Called to start the Timestamping node.
  void stop_Timestamping();      ///< Called to stop the Timestamping node.
  void recover_Timestamping();   ///< Called to recover the Timestamping node from a failure.
  ///< Called to verify if a state change is allowed
  bool allow_Device_Change(const nds::state_t,
                           const nds::state_t, const nds::state_t);

	/**
	 * @brief Function that emulates the changes of status of the Timestamping node.
	 *        It is launched by start_Timestamping() in a separate thread.
	 */
	void Timestamping_thread_body();



  /**
   * Timestamping setters
   */
  void PV_Enable_Writer(const timespec& timestamp, const std::int32_t& value);
  void PV_Edge_Writer(const timespec& timestamp, const std::int32_t& value);

  /**
   * @brief A thread that runs Timestamping_thread_body().
   */
  std::thread m_Timestamping_Thread;

  /**
   * @brief A boolean flag that stop the Timestamping loop in Timestamping_thread_body()
   *        when true.
   */
  volatile bool m_bStop_Timestamping;

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
