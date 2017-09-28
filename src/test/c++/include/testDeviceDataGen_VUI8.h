#ifndef TESTDEVICEDATAGENVUI8_H_
#define TESTDEVICEDATAGENVUI8_H_

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
class testDeviceDataGenVUI8
{
public:
	/**
	 * @brief Constructor.
	 *
	 * @param factory    the control system factory that requested the creation of the device
	 * @param device     the name given to the device
	 * @param parameters optional parameters passed to the device
	 */
	testDeviceDataGenVUI8(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
	~testDeviceDataGenVUI8();

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
	static testDeviceDataGenVUI8* getInstance(const std::string& deviceName);


private:

	/*
	 * @brief name of the device
	 */
	std::string m_name;

	timespec timestamp_device, readtimeStamp;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//  TEST DEVICE STATE MACHINE
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief testDeviceDataGenVUI8 state machine
	 */
	nds::StateMachine m_testDeviceDataGenVUI8_stateMachine;

	/**
	 * Methods to control testDeviceDataGenVUI8 state machine
	 */
	void switchOn_testDeviceDataGenVUI8();  ///< Called to switch on the testDeviceDataGenVUI8 (rootnode).
	void switchOff_testDeviceDataGenVUI8(); ///< Called to switch off the testDeviceDataGenVUI8 (rootnode).
	void start_testDeviceDataGenVUI8();     ///< Called to start the testDeviceDataGenVUI8 (rootnode).
	void stop_testDeviceDataGenVUI8();      ///< Called to stop the testDeviceDataGenVUI8 (rootnode).
	void recover_testDeviceDataGenVUI8();   ///< Called to recover the testDeviceDataGenVUI8 (rootnode) from a failure.

	bool allow__testDeviceDataGenVUI8_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//  DATA GENERATION
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief DataGeneration node
	 */
	nds::DataGeneration<std::vector<std::uint8_t>> m_DataGeneration;

	/**
	 * Methods to control DataGeneration state machine
	 */
	void switchOn_DataGeneration();  ///< Called to switch on the DataGeneration node.
	void switchOff_DataGeneration(); ///< Called to switch off the DataGeneration node.
	void start_DataGeneration();     ///< Called to start the DataGeneration node.
	void stop_DataGeneration();      ///< Called to stop the DataGeneration node.
	void recover_DataGeneration();   ///< Called to recover the DataGeneration node from a failure.

	bool allow_DataGeneration_Change(const nds::state_t, const nds::state_t, const nds::state_t); ///< Called to verify if a state change is allowed

	/**
	 * DataGeneration setters
	 */
	void PV_DataGeneration_Frequency_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_RefFrequency_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_Amp_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_Phase_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_UpdateRate_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_DutyCycle_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_Gain_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_Offset_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_Bandwidth_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_Resolution_Writer(const timespec& timestamp, const double& value);
	void PV_DataGeneration_Impedance_Writer(const timespec& timestamp, const std::int32_t& value);
	void PV_DataGeneration_Coupling_Writer(const timespec& timestamp, const std::int32_t& value);
	void PV_DataGeneration_SignalRef_Writer(const timespec& timestamp, const std::int32_t& value);
	void PV_DataGeneration_SignalType_Writer(const timespec& timestamp, const std::int32_t& value);
	void PV_DataGeneration_Ground_Writer(const timespec& timestamp, const std::int32_t& value);

	 /**
	  * @brief Function that continuously generates a sinusoidal wave to m_DataGeneration.
	  *        It is launched by start_DataGeneration() in a separate thread.
	  */
	 void DataGeneration_thread_body();

	 /**
	  * @brief A thread that runs DataGeneration_thread_body().
	  */
	 std::thread m_DataGeneration_Thread;

	 /**
	  * @brief A boolean flag that stop the DataGeneration loop in DataGeneration_thread_body()
	  *        when true.
	  */
	 volatile bool m_bStop_DataGeneration;

};

#endif // TESTDEVICEDATAGENVUI8_H_
