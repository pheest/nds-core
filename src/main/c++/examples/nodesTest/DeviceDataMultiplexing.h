#ifndef DEVICE_DATAMUX_H_
#define DEVICE_DATAMUX_H_

#include <memory>

#include <functional>
#include <math.h>
#include <unistd.h>
#include <iostream>
#include <thread>

#include <nds3/nds.h>

/**
 * @brief Class that declares and implement a fictional device with several channels for testing data multiplexing of nds-core V3.
 *
 *
 * The class does not need to be derived from any special class, but its constructor must
 *  accept few mandatory parameters and should register the root node via Node::initialize().
 */
class DeviceDataMultiplexing
{
public:
	/**
	 * @brief Constructor.
	 *
	 * @param factory    the control system factory that requested the creation of the device
	 * @param device     the name given to the device
	 * @param parameters optional parameters passed to the device
	 */
	DeviceDataMultiplexing(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& );
	~DeviceDataMultiplexing();

#ifndef EPICS
	/*
	 * Allocation/deallocation
	 *
	 *******************************************************/
	static void* allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters);
	static void deallocateDevice(void* deviceName);
#endif

	/*
	 * For test purposes we make it possible to retrieve running instances of
	 *  the device
	 */
	static DeviceDataMultiplexing* getInstance(const std::string& deviceName);


private:

	/*
	 * @brief name of the device
	 */
	std::string m_Name;

	std::int32_t nChannels = 4;
	std::int32_t dataType = 4; //float by default
        std::string dataTypeTxt = "float";
        std::int32_t maxElements = 20;
        std::int32_t lastValue = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// TEST DATAMULTIPLEXING NODE
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief DataMultiplexing node
	 */
	nds::DataMultiplexing<std::vector<float>> m_DataMultiplexing;


	nds::PVVariableIn<std::vector<float>> m_PV_Source_0;
        nds::PVVariableIn<std::vector<float>> m_PV_Source_1;
        nds::PVVariableIn<std::vector<float>> m_PV_Source_2;
        nds::PVVariableIn<std::vector<float>> m_PV_Source_3;


        nds::PVDelegateOut<std::int32_t>  m_PV_IncreaseSources;



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

	void setFloatArrayPV(nds::PVVariableIn<std::vector<float>> &pv, const timespec& timestamp, const std::vector<float>& data);

public:

	void increaseSourcePVs(const timespec &timestamp, const std::int32_t &doIt);



};

#endif // DEVICE_DATAMUX_H_
