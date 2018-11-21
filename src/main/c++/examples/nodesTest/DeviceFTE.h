#ifndef DEVICEFTE_H_
#define DEVICEFTE_H_

#include <memory>

#include <functional>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with a Future Time Event node for testing purposes of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceFTE
{
public:
	/**
	 * @brief Constructor.
	 *
	 * @param factory    the control system factory that requested the creation of the device
	 * @param device     the name given to the device
	 * @param parameters optional parameters passed to the device
	 */
	DeviceFTE(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
	~DeviceFTE();

#ifndef EPICS
	/**
	 * Allocation/deallocation
	 *
	 *******************************************************/
	static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
	static void deallocateDevice(void* deviceName);
#endif

	/**
	 * For test purposes we make it possible to retrieve running instances of
	 *  the device
	 */
	static DeviceFTE* getInstance(const std::string& deviceName);


private:

	/**
	 * @brief name of the device
	 */
	std::string m_name;

	timespec timestamp_device, readtimeStamp;

	/**
	 * @brief FTE node
	 */
	nds::FTE<std::string> m_FTE;

	/**
	 * Methods to control DigitalIO state machine
	 */
	void switchOn_FTE();  ///< Called to switch on the FTE node.
	void switchOff_FTE(); ///< Called to switch off the FTE node.
	void start_FTE();     ///< Called to start the FTE node.
	void stop_FTE();      ///< Called to stop the FTE node.
	void recover_FTE();   ///< Called to recover the FTE node from a failure.

	bool allow_FTE_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

	nds::PVVariableOut<std::int32_t> m_setCurrentTime;
	timespec getCurrentTime();

	/**
	 * FTE setters
	 */
	void PV_FTE_Set_Writer(const timespec& timestamp, const std::int32_t& value);
	void PV_FTE_Suppress_Writer(const timespec& timestamp, const std::int32_t& value);
	void PV_FTE_ChgPeriod_Writer(const timespec& timestamp, const std::int32_t& value);
	void PV_FTE_PendingValue_Writer(const timespec& timestamp, const std::int32_t& value);

	/**
	 * FTE initializers
	 */
	void PV_FTE_Set_Initializer(timespec* timestamp, int32_t* value);
	void PV_FTE_Suppress_Initializer(timespec* timestamp, int32_t* value);
	void PV_FTE_ChgPeriod_Initializer(timespec* timestamp, int32_t* value);
	void PV_FTE_PendingValue_Initializer(timespec* timestamp, int32_t* value);
};

#endif // DeviceFTE_H_
