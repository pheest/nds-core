#ifndef DEVICE_FIRM_H_
#define DEVICE_FIRM_H_

#include <memory>

#include <functional>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a Firmware node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceFirmware
{
public:
	/**
	 * @brief Constructor.
	 *
	 * @param factory    the control system factory that requested the creation of the device
	 * @param device     the name given to the device
	 * @param parameters optional parameters passed to the device
	 */
	DeviceFirmware(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
	~DeviceFirmware();

	/*
	 * Allocation/deallocation
	 *
	 *******************************************************/
	static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
	static void deallocateDevice(void* deviceName);

	/*
	 * For test purposes we make it possible to retrieve running instances of
	 *  the device
	 */
	static DeviceFirmware* getInstance(const std::string& deviceName);


private:

	/*
	 * @brief name of the device
	 */
	std::string m_Name;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST DEVICE STATE MACHINE
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief Device state machine
	 */
	nds::StateMachine m_StateMachine;

	/**
	 * Methods to control Device state machine
	 */
	void switchOn_Device();  ///< Called to switch on the Device (rootnode).
	void switchOff_Device(); ///< Called to switch off the Device (rootnode).
	void start_Device();     ///< Called to start the Device (rootnode).
	void stop_Device();      ///< Called to stop the Device (rootnode).
	void recover_Device();   ///< Called to recover the Device (rootnode) from a failure.
	bool allow_Device_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// TEST FIRMWARE NODE
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief Firmware node
	 */
	nds::FirmwareSup<std::string > m_Firmware;

	/**
	 * Methods to control the Firmware state machine
	 */
	void switchOn_Firmware();  ///< Called to switch on the Firmware node.
	void switchOff_Firmware(); ///< Called to switch off the Firmware node.
	void start_Firmware();     ///< Called to start the Firmware node.
	void stop_Firmware();      ///< Called to stop the Firmware node.
	void recover_Firmware();   ///< Called to recover the Firmware node from a failure.

	bool allow_Firmware_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

	/**
	 * Firmware setters
	 */
	void PV_Firmware_Path_Writer(const timespec& timestamp, const std::string& value);

	/**
	 * @brief Function that emulates the changes of status of the Firmware node.
	 *        It is launched by start_Firmware() in a separate thread.
	 */
	void Firmware_thread_body();

	/**
	 * @brief A thread that runs Firmware_thread_body().
	 */
	std::thread m_Firmware_Thread;

	/**
	 * @brief A boolean flag that stop the Firmware loop in Firmware_thread_body()
	 *        when true.
	 */
	volatile bool m_bStop_Firmware;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// TIMESTAMP HANDLING
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * @brief PV to set the timestamp of the device, in seconds
	 */
	nds::PVVariableOut<std::int32_t> m_setCurrentTime;

	/**
	 * @brief Function to get the timestamp of the device
	 */
	timespec getCurrentTime();

};

#endif // DEVICE_FIRM_H_
