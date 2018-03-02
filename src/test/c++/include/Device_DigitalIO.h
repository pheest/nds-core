#ifndef DEVICEI_DIGITALIO_H_
#define DEVICEI_DIGITALIO_H_

#include <memory>

#include <functional>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>


/**
 * @brief Class that declares and implement a fictional device for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceDigitalIO
{
	/**
	 * @brief Constructor.
	 *
	 * @param factory    the control system factory that requested the creation of the device
	 * @param device     the name given to the device
	 * @param parameters optional parameters passed to the device
	 */
	DeviceDigitalIO(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
	~DeviceDigitalIO();

	/**
	 * Allocation/deallocation
	 *
	 *******************************************************/
	static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
	static void deallocateDevice(void* deviceName);

	/**
	 * For test purposes we make it possible to retrieve running instances of
	 *  the device
	 */
	static DeviceDigitalIO* getInstance(const std::string& deviceName);


private:

	/**
	 * @brief name of the device
	 */
	std::string m_name;

	timespec timestamp_device, readtimeStamp;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST DEVICE STATE MACHINE
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief DeviceDigitalIO state machine
	 */
	nds::StateMachine m_DeviceDigitalIO_stateMachine;

	/**
	 * Methods to control DeviceDigitalIO state machine
	 */
	void switchOn_Device();  ///< Called to switch on the DeviceDigitalIO (rootnode).
	void switchOff_Device(); ///< Called to switch off the DeviceDigitalIO (rootnode).
	void start_Device();     ///< Called to start the DeviceDigitalIO (rootnode).
	void stop_Device();      ///< Called to stop the DeviceDigitalIO (rootnode).
	void recover_Device();   ///< Called to recover the DeviceDigitalIO (rootnode) from a failure.
	bool allow_Device_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

	nds::PVVariableOut<std::int32_t> m_setCurrentTime;
	timespec getCurrentTime();

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//  DIGITAL I/O
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief DigitalIO node
	 */
	nds::DigitalIO<std::vector<bool> > m_DigitalIO_Bool;
	nds::DigitalIO<std::vector<std::uint8_t> > m_DigitalIO_Uint8_t;
	nds::DigitalIO<std::vector<std::uint16_t> > m_DigitalIO_Uint16_t;
	nds::DigitalIO<std::vector<std::uint32_t> > m_DigitalIO_Uint32_t;

	/**
	 * Methods to control DigitalIO state machine
	 */
	void switchOn_DigitalIO();  ///< Called to switch on the DigitalIO node.
	void switchOff_DigitalIO(); ///< Called to switch off the DigitalIO node.
	void start_DigitalIO();     ///< Called to start the DigitalIO node.
	void stop_DigitalIO();      ///< Called to stop the DigitalIO node.
	void recover_DigitalIO();   ///< Called to recover the DigitalIO node from a failure.

	bool allow_DigitalIO_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

	/**
	 * DigitalIO setters
	 */
	void PV_DigitalIO_dataOutMask_Writer(const timespec& timestamp, const std::vector<bool>& value);
	void PV_DigitalIO_voltLevelHigh_Writer(const timespec& timestamp, const double& value);
	void PV_DigitalIO_voltLevelLow_Writer(const timespec& timestamp, const double& value);
	void PV_DigitalIO_ChannelDir_Writer(const timespec& timestamp, const std::vector<bool>& value);

	/**
	 * @brief Function that continuously acquires digital IO data.
	 *        It is launched by start_DigitalIO() in a separate thread.
	 */
	void DigitalIO_thread_body_Bool();

	/**
	 * @brief Function that continuously acquires digital IO data.
	 *        It is launched by start_DigitalIO() in a separate thread.
	 */
	void DigitalIO_thread_body_U8();

	/**
	 * @brief Function that continuously acquires digital IO data.
	 *        It is launched by start_DigitalIO() in a separate thread.
	 */
	void DigitalIO_thread_body_U16();

	/**
	 * @brief Function that continuously acquires digital IO data.
	 *        It is launched by start_DigitalIO() in a separate thread.
	 */
	void DigitalIO_thread_body_U32();

	/**
	 * @brief A thread that runs DataProcessing_thread_body().
	 */
	std::thread m_DigitalIO_Thread_Bool;

	/**
	 * @brief A thread that runs DataProcessing_thread_body().
	 */
	std::thread m_DigitalIO_Thread_U8;

	/**
	 * @brief A thread that runs DataProcessing_thread_body().
	 */
	std::thread m_DigitalIO_Thread_U16;

	/**
	 * @brief A thread that runs DataProcessing_thread_body().
	 */
	std::thread m_DigitalIO_Thread_U32;

	/**
	 * @brief A boolean flag that stop the DigitalIO loop in DigitalIO_thread_body()
	 *        when true.
	 */
	volatile bool m_bStop_DigitalIO;

};

#endif // DEVICE_DIGITALIO_H_
