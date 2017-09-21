
#include "../include/testDevice.h"
#include <nds3/nds.h>
#include <mutex>
#include <unistd.h>
#include <functional>

static std::map<std::string, testDevice*> m_devicesMap;
static std::mutex m_lockDevicesMap;

testDevice::testDevice(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t &parameters):
	m_name(deviceName),

	PVVariable_value_I32(0),PVDelegate_value_I32(0),PVVariable_value_DBL(0),PVDelegate_value_DBL(0),PVVariable_vector_I8(2,0),PVDelegate_vector_I8(2,0),
	PVVariable_vector_UI8(2,0),	PVDelegate_vector_UI8(2,0),PVVariable_vector_I32(2,0),PVDelegate_vector_I32(2,0),PVVariable_vector_DBL(2,0),PVDelegate_vector_DBL(2,0),
	PVVariable_value_string{""},PVDelegate_value_string{""},

	timestamp_device{0,0},readtimeStamp{0,0},

	m_int32_DelegateIn("int32_DelegateIn",std::bind(&testDevice::read_I32_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_int32_DelegateOut("int32_DelegateOut",std::bind(&testDevice::write_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_int32_DelegateOut_init("int32_DelegateOut_init",std::bind(&testDevice::write_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&testDevice::init_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_double_DelegateIn("double_DelegateIn",std::bind(&testDevice::read_DBL_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_double_DelegateOut("double_DelegateOut",std::bind(&testDevice::write_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_double_DelegateOut_init("double_DelegateOut_init",std::bind(&testDevice::write_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&testDevice::init_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorI8_DelegateIn("vectorI8_DelegateIn",std::bind(&testDevice::read_vectorI8_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI8_DelegateOut("vectorI8_DelegateOut",std::bind(&testDevice::write_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI8_DelegateOut_init("vectorI8_DelegateOut_init",std::bind(&testDevice::write_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&testDevice::init_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorUI8_DelegateIn("vectorUI8_DelegateIn",std::bind(&testDevice::read_vectorUI8_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorUI8_DelegateOut("vectorUI8_DelegateOut",std::bind(&testDevice::write_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorUI8_DelegateOut_init("vectorUI8_DelegateOut_init",std::bind(&testDevice::write_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&testDevice::init_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorI32_DelegateIn("vectorI32_DelegateIn",std::bind(&testDevice::read_vectorI32_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI32_DelegateOut("vectorI32_DelegateOut",std::bind(&testDevice::write_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI32_DelegateOut_init("vectorI32_DelegateOut_init",std::bind(&testDevice::write_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&testDevice::init_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorDBL_DelegateIn("vectorDBL_DelegateIn",std::bind(&testDevice::read_vectorDBL_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorDBL_DelegateOut("vectorDBL_DelegateOut",std::bind(&testDevice::write_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorDBL_DelegateOut_init("vectorDBL_DelegateOut_init",std::bind(&testDevice::write_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&testDevice::init_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_string_DelegateIn("string_DelegateIn",std::bind(&testDevice::read_string_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_string_DelegateOut("string_DelegateOut",std::bind(&testDevice::write_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_string_DelegateOut_init("string_DelegateOut_init",std::bind(&testDevice::write_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&testDevice::init_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2))
{
	//TODO:Study this.
	{
		std::lock_guard<std::mutex> lock(m_lockDevicesMap);
		if(m_devicesMap.find(deviceName) != m_devicesMap.end())
		{
			throw std::logic_error("Device with the same name already allocated. This should not happen");
		}
		m_devicesMap[deviceName] = this;
	}

	/**
	 * Here we declare the root node.
	 * It is a good practice to name it with the device name.
	 *
	 * Also, for simplicity we declare it as a "Port": this means that
	 * the root node will be responsible for the communication with
	 * the underlying control system.
	 *
	 * It is possible to have the root node as a simple Node and promote one or
	 * more of its children to "Port": each port will interface with a different
	 * control system thread.
	 */
	nds::Port rootNode(deviceName);

	// Add state machine
	m_testDevice_stateMachine = rootNode.addChild(nds::StateMachine(true,
			std::bind(&testDevice::switchOn_testDevice, this),
			std::bind(&testDevice::switchOff_testDevice, this),
			std::bind(&testDevice::start_testDevice, this),
			std::bind(&testDevice::stop_testDevice, this),
			std::bind(&testDevice::recover_testDevice, this),
			std::bind(&testDevice::allow__testDevice_Change,this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

	/**
	 * We add children to the root node:
	 * - a variable that the user can set with the max. amplitude of the data generation node.
	 *
	 * We save the variable in a class' member so later we can access it to retrieve its value
	 */
	m_maxAmplitude = rootNode.addChild(nds::PVVariableOut<double>("maxAmplitude"));

//	/**
//	 * Add a DataAcquisition node: it acquires data generated by the DataGeneration node and supplies an input PV on which we can push the
//	 * acquired data, it also adds a state machine that allows to start and stop the acquisition.
//	 */
	m_DataAcquisition = rootNode.addChild(nds::DataAcquisition<std::vector<double> >(
			"DataAcquisitionNode",
			128,
			std::bind(&testDevice::switchOn_DataAcquisition, this),
			std::bind(&testDevice::switchOff_DataAcquisition, this),
			std::bind(&testDevice::start_DataAcquisition, this),
			std::bind(&testDevice::stop_DataAcquisition, this),
			std::bind(&testDevice::recover_DataAcquisition, this),
			std::bind(&testDevice::allow_DataAcquisition_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&testDevice::PV_DataAcquisition_Gain_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Gain_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Offset_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Offset_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Bw_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Bw_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Resolution_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Resolution_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Impedance_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Impedance_Reader,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Coupling_Writer,this,   std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Coupling_Reader,this,   std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_SignalRef_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_SignalRef_Reader,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Ground_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataAcquisition_Ground_Reader,this, std::placeholders::_1, std::placeholders::_2)
	));
//
//	/**
//	 * Add a DataGeneration node: it acquires data generated by the DataGeneration node and supplies an input PV on which we can push the
//	 * acquired data, it also adds a state machine that allows to start and stop the acquisition.
//	 */
	m_DataGeneration = rootNode.addChild(nds::DataGeneration<std::vector<double> >(
			"DataGenerationNode",
			128,
			std::bind(&testDevice::switchOn_DataGeneration, this),
			std::bind(&testDevice::switchOff_DataGeneration, this),
			std::bind(&testDevice::start_DataGeneration, this),
			std::bind(&testDevice::stop_DataGeneration, this),
			std::bind(&testDevice::recover_DataGeneration, this),
			std::bind(&testDevice::allow_DataGeneration_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&testDevice::PV_DataGeneration_Frequency_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Frequency_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_RefFrequency_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_RefFrequency_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Amp_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Amp_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Phase_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Phase_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_UpdateRate_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_UpdateRate_Reader,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_DutyCycle_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_DutyCycle_Reader,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Gain_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Gain_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Offset_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Offset_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Bw_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Bw_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Resolution_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Resolution_Reader,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Impedance_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Impedance_Reader,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Coupling_Writer,this,   std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Coupling_Reader,this,   std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_SignalRef_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_SignalRef_Reader,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_SignalType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_SignalType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Ground_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&testDevice::PV_DataGeneration_Ground_Reader,this, std::placeholders::_1, std::placeholders::_2)
	));

	/**
	      * Add a DataProcessing node:
	      */
	    m_DataProcessing = rootNode.addChild(nds::DataProcessing<std::vector<int32_t> >(
	     		"DataProcessingNode",
	 			128,
	 			std::bind(&testDevice::switchOn_DataProcessing, this),
	 			std::bind(&testDevice::switchOff_DataProcessing, this),
	 			std::bind(&testDevice::start_DataProcessing, this),
	 			std::bind(&testDevice::stop_DataProcessing, this),
	 			std::bind(&testDevice::recover_DataProcessing, this),
	 			std::bind(&testDevice::allow_DataProcessing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
	 			std::bind(&testDevice::PV_DataProcessing_EnableFilter_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_EnableFilter_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FilterType_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FilterType_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FilterParams_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FilterParams_Reader,this, std::placeholders::_1, std::placeholders::_2),
				128,
	 			std::bind(&testDevice::PV_DataProcessing_EnableFFT_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_EnableFFT_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_EnableSwFFT_Writer,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_EnableSwFFT_Reader,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FFTwindowType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FFTwindowType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FFTOverlap_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FFTOverlap_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FFTFrameSize_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FFTFrameSize_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FFTSmooth_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_FFTSmooth_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_EnableDecimation_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_EnableDecimation_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_DecimationType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_DecimationType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_DecimationOffset_Writer,this,   std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_DecimationOffset_Reader,this,   std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_RAW2Eng_Writer,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DataProcessing_RAW2Eng_Reader,this,  std::placeholders::_1, std::placeholders::_2)
	     ));

	    /**
	     * Add a Digital I/O node:
	     */
	    m_DigitalIO = rootNode.addChild(nds::DigitalIO<std::vector<int32_t> >(
	    		"DigitalIONode",
				128,
				std::bind(&testDevice::switchOn_DigitalIO, this),
				std::bind(&testDevice::switchOff_DigitalIO, this),
				std::bind(&testDevice::start_DigitalIO, this),
				std::bind(&testDevice::stop_DigitalIO, this),
				std::bind(&testDevice::recover_DigitalIO, this),
				std::bind(&testDevice::allow_DigitalIO_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&testDevice::PV_DigitalIO_voltLevelHigh_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_DigitalIO_voltLevelHigh_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_DigitalIO_voltLevelLow_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_DigitalIO_voltLevelLow_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_DigitalIO_ChannelDir_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_DigitalIO_ChannelDir_Reader,this, std::placeholders::_1, std::placeholders::_2)
	    ));

	    /**
	      * Add a DMA Support node:
	      */
	    m_DMASupport = rootNode.addChild(nds::DMASupport<std::vector<int32_t> >(
	     		"DMASupportNode",
	 			128,
	 			std::bind(&testDevice::switchOn_DMASupport, this),
	 			std::bind(&testDevice::switchOff_DMASupport, this),
	 			std::bind(&testDevice::start_DMASupport, this),
	 			std::bind(&testDevice::stop_DMASupport, this),
	 			std::bind(&testDevice::recover_DMASupport, this),
	 			std::bind(&testDevice::allow_DMASupport_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
	 			std::bind(&testDevice::PV_DMASupport_BufferSize_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DMASupport_EnableDMA_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DMASupport_EnableDMA_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DMASupport_NumDMAChannels_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DMASupport_DMAFrameType_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_DMASupport_DMASampleSize_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_DMASupport_DMASamplingRate_Reader,this, std::placeholders::_1, std::placeholders::_2)
	     ));

	    /**
	      * Add a Streaming Config node:
	      */
	    m_StreamingConf = rootNode.addChild(nds::StreamingConf<std::vector<int32_t> >(
	     		"StreamingConfNode",
	 			128,
	 			std::bind(&testDevice::switchOn_StreamingConf, this),
	 			std::bind(&testDevice::switchOff_StreamingConf, this),
	 			std::bind(&testDevice::start_StreamingConf, this),
	 			std::bind(&testDevice::stop_StreamingConf, this),
	 			std::bind(&testDevice::recover_StreamingConf, this),
	 			std::bind(&testDevice::allow_StreamingConf_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
	 			std::bind(&testDevice::PV_StreamingConf_StreamingDataFormat_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_StreamingConf_StreamingType_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&testDevice::PV_StreamingConf_StreamingType_Reader,this, std::placeholders::_1, std::placeholders::_2)
	     ));

	    /**
	     * Add a HealthMonitSup node.
	     */
	    m_HealthMonitSup = rootNode.addChild(nds::HealthMonitSup<std::vector<std::int32_t> >(
	    		"HealthMonitSupNode",
				std::bind(&testDevice::switchOn_HealthMonitSup, this),
				std::bind(&testDevice::switchOff_HealthMonitSup, this),
				std::bind(&testDevice::start_HealthMonitSup, this),
				std::bind(&testDevice::stop_HealthMonitSup, this),
				std::bind(&testDevice::recover_HealthMonitSup, this),
				std::bind(&testDevice::allow_HealthMonitSup_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&testDevice::PV_HealthMonitSup_DevicePower_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_DeviceTemp_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_DeviceVoltage_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_DeviceCurrent_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableSEU_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableSEU_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableMonitorDAQ_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableMonitorDAQ_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableShelfTest_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableShelfTest_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_ShelfTestType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_ShelfTestType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_VerboseShelfTest_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_VerboseShelfTest_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableShelfTestId_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableShelfTestId_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableShelfTestText_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_EnableShelfTestText_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_SignalQualityFlag_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_HealthMonitSup_SignalQualityFlagLevel_Reader,this, std::placeholders::_1, std::placeholders::_2)
	    ));


	    /**
	     * Add a imageAcquisition node.
	     */
	    m_imageAcquisition = rootNode.addChild(nds::imageAcquisition<std::vector<double> >(
	    		"imageAcquisitionNode",
				128,
				std::bind(&testDevice::switchOn_imageAcquisition, this),
				std::bind(&testDevice::switchOff_imageAcquisition, this),
				std::bind(&testDevice::start_imageAcquisition, this),
				std::bind(&testDevice::stop_imageAcquisition, this),
				std::bind(&testDevice::recover_imageAcquisition, this),
				std::bind(&testDevice::allow_imageAcquisition_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&testDevice::PV_imageAcquisition_MaxSizeX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_MaxSizeY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_BinX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_BinX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_BinY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_BinY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_MinX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_MinX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_MinY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_MinY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_SizeX_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_SizeX_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_SizeY_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_SizeY_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ReverseX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ReverseX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ReverseY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ReverseY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Resolution_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Resolution_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_SamplesPerPixel_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_SamplesPerPixel_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_AcquireTime_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_AcquireTime_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_AcquirePeriod_Writer,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_AcquirePeriod_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_TimeRemaining_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Gain_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Gain_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_FrameType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_FrameType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_LostFrames_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_LostFrames_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ImageMode_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ImageMode_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_TriggerMode_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_TriggerMode_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_NumExposures_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_NumExposures_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_NumExposuresCounter_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Exposure_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Exposure_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_minExposure_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_minExposure_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_maxExposure_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_maxExposure_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ExposureStep_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ExposureStep_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_BlackLevel_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_BlackLevel_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_NumImages_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_NumImages_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_NumImagesCounter_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Acquire_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Acquire_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_DetectorState_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_StatusMessage_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_StringToServer_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_StringFromServer_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ReadStatus_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMode_Writer,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMode_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterControlMode_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterControlMode_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterStatus_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_DelayStep_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_DelayStep_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterOpenDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterOpenDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMinOpenDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMinOpenDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMaxOpenDelay_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMaxOpenDelay_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterCloseDelay_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterCloseDelay_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMinCloseDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMinCloseDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMaxCloseDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ShutterMaxCloseDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_HotPixels_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_HotPixels_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_HotPixelsCorr_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_HotPixelsCorr_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Temperature_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_Temperature_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&testDevice::PV_imageAcquisition_ActualTemperature_Reader,this,  std::placeholders::_1, std::placeholders::_2)
	    ));


	    /**
	     * Test PV variables:
	     * 		- PVVariableIn: input variable to CS. The device support sets its value using setValue, and the CS reads this value.
	     * 		- PVVariableOut: output variable from CS. The device support uses getValue()to retrieve the PV's value,
	     * 		                 and the control system uses read() and write() to read and set the value.
	     *
	     * For testing purposes, we use:
	     * 		- setValue(const T& value) and setValue(const timespec& timestamp, const T& value), over PVVariableIn
	     *        but we do not modify the initial value.
	     *      - getValue() and getValue(timespec* pTime, T* pValue) to retrieve the initial value and timestamp.
	     */

	    m_int32_VariableIn = rootNode.addChild(nds::PVVariableIn<std::int32_t>("int32_VariableIn"));
	    m_int32_VariableIn.setValue((std::int32_t)0);
	    m_int32_VariableIn.setValue(timestamp_device,(std::int32_t)0);
	    m_int32_VariableOut = rootNode.addChild(nds::PVVariableOut<std::int32_t>("int32_VariableOut"));
	    m_int32_VariableOut.getValue();
	    m_int32_VariableOut.getValue(&readtimeStamp,&PVVariable_value_I32);

	    m_double_VariableIn = rootNode.addChild(nds::PVVariableIn<double>("double_VariableIn"));
	    m_double_VariableIn.setValue((double)0);
	    m_double_VariableIn.setValue(timestamp_device,(double)0);
	    m_double_VariableOut = rootNode.addChild(nds::PVVariableOut<double>("double_VariableOut"));
	    m_double_VariableOut.getValue();
	    m_double_VariableOut.getValue(&readtimeStamp,&PVVariable_value_DBL);

	    m_vectorI8_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<std::int8_t> >("vectorI8_VariableIn"));
	    m_vectorI8_VariableIn.setMaxElements(2);
	    m_vectorI8_VariableIn.setValue(std::vector<int8_t>(2,0));
	    m_vectorI8_VariableIn.setValue(timestamp_device,std::vector<int8_t>(2,0));
	    m_vectorI8_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<std::int8_t> >("vectorI8_VariableOut"));
	    m_vectorI8_VariableOut.getValue();
	    m_vectorI8_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_I8);

	    m_vectorUI8_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<std::uint8_t> >("vectorUI8_VariableIn"));
	    m_vectorUI8_VariableIn.setMaxElements(2);
	    m_vectorUI8_VariableIn.setValue(std::vector<uint8_t>(2,0));
	    m_vectorUI8_VariableIn.setValue(timestamp_device,std::vector<uint8_t>(2,0));
	    m_vectorUI8_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<std::uint8_t> >("vectorUI8_VariableOut"));
	    m_vectorUI8_VariableOut.getValue();
	    m_vectorUI8_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_UI8);

	    m_vectorI32_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<std::int32_t> >("vectorI32_VariableIn"));
	    m_vectorI32_VariableIn.setMaxElements(2);
	    m_vectorI32_VariableIn.setValue(std::vector<int32_t>(2,0));
	    m_vectorI32_VariableIn.setValue(timestamp_device,std::vector<int32_t>(2,0));
	    m_vectorI32_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<std::int32_t> >("vectorI32_VariableOut"));
	    m_vectorI32_VariableOut.getValue();
	    m_vectorI32_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_I32);

	    m_vectorDBL_VariableIn = rootNode.addChild(nds::PVVariableIn<std::vector<double> >("vectorDBL_VariableIn"));
	    m_vectorDBL_VariableIn.setMaxElements(2);
	    m_vectorDBL_VariableIn.setValue(std::vector<double>(2,0));
	    m_vectorDBL_VariableIn.setValue(timestamp_device,std::vector<double>(2,0));
	    m_vectorDBL_VariableOut = rootNode.addChild(nds::PVVariableOut<std::vector<double> >("vectorDBL_VariableOut"));
	    m_vectorDBL_VariableOut.getValue();
	    m_vectorDBL_VariableOut.getValue(&readtimeStamp,&PVVariable_vector_DBL);

	    m_string_VariableIn= rootNode.addChild(nds::PVVariableIn<std::string>("string_VariableIn"));
	    m_string_VariableIn.setValue("");
	    m_string_VariableIn.setValue(timestamp_device,"");
	    m_string_VariableOut= rootNode.addChild(nds::PVVariableOut<std::string>("string_VariableOut"));
	    m_string_VariableOut.getValue();
	    m_string_VariableOut.getValue(&readtimeStamp,&PVVariable_value_string);

	    /**
	     * Test PV Delegates: input Delegate to CS. The device support sets its value, and the CS reads this value
	     * The device support uses getValue()to retrieve the PV's value and the control system use read() and write() to read and set the value.
	     */

	    rootNode.addChild(m_int32_DelegateIn);
	    rootNode.addChild(m_int32_DelegateOut);
	    rootNode.addChild(m_int32_DelegateOut_init);

	    rootNode.addChild(m_double_DelegateIn);
	    rootNode.addChild(m_double_DelegateOut);
	    rootNode.addChild(m_double_DelegateOut_init);

	    m_vectorI8_DelegateIn.setMaxElements(2);
	    rootNode.addChild(m_vectorI8_DelegateIn);
	    rootNode.addChild(m_vectorI8_DelegateOut);
	    rootNode.addChild(m_vectorI8_DelegateOut_init);

	    m_vectorUI8_DelegateIn.setMaxElements(2);
	    rootNode.addChild(m_vectorUI8_DelegateIn);
	    rootNode.addChild(m_vectorUI8_DelegateOut);
	    rootNode.addChild(m_vectorUI8_DelegateOut_init);

	    m_vectorI32_DelegateIn.setMaxElements(2);
	    rootNode.addChild(m_vectorI32_DelegateIn);
	    rootNode.addChild(m_vectorI32_DelegateOut);
	    rootNode.addChild(m_vectorI32_DelegateOut_init);

	    m_vectorDBL_DelegateIn.setMaxElements(2);
	    rootNode.addChild(m_vectorDBL_DelegateIn);
	    rootNode.addChild(m_vectorDBL_DelegateOut);
	    rootNode.addChild(m_vectorDBL_DelegateOut_init);

	    rootNode.addChild(m_string_DelegateIn);
	    rootNode.addChild(m_string_DelegateOut);
	    rootNode.addChild(m_string_DelegateOut_init);

	// We have declared all the nodes and PVs in our device: now we register them
	//  with the control system that called this constructor.
	////////////////////////////////////////////////////////////////////////////////
	rootNode.initialize(this, factory);
}



testDevice::~testDevice()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_devicesMap.erase(m_name);

}

testDevice* testDevice::getInstance(const std::string& deviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, testDevice*>::const_iterator findDevice = m_devicesMap.find(deviceName);
    if(findDevice == m_devicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

/*
 * Allocation function
 *********************/
void* testDevice::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters)
{
    return new testDevice(factory, deviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void testDevice::deallocateDevice(void* deviceName)
{
    delete (testDevice*)deviceName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  TEST DEVICE STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control testDevice state machine
 */
void testDevice::switchOn_testDevice(){

}
void testDevice::switchOff_testDevice(){

}
void testDevice::start_testDevice(){

}
void testDevice::stop_testDevice(){

}
void testDevice::recover_testDevice(){

}

bool testDevice::allow__testDevice_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA ACQUISITION NODE*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* DataAcquisition State Machine
*/

// Called when the DataAcquisition node has to be switched on.
void testDevice::switchOn_DataAcquisition(){

}

// Called when the DataAcquisition node has to be switched off.
void testDevice::switchOff_DataAcquisition(){

}

// Called when the DataAcquisition node has to start acquiring. We start the data acquisition thread.
void testDevice::start_DataAcquisition(){

	m_bStop_DataAcquisition = false; //< We will set to true to stop the acquisition thread
	/**
	 *   Start the acquisition thread.
	 *   We don't need to check if the thread was already started because the state
	 *   machine guarantees that the start handler is called only while the state
	 *   is ON.
	 */
	m_DataAcquisition_Thread = std::thread(std::bind(&testDevice::DataAcquisition_thread_body, this));
}

// Stop the DataAcquisition node thread
void testDevice::stop_DataAcquisition(){
	m_bStop_DataAcquisition = true;
	m_DataAcquisition_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.
void testDevice::recover_DataAcquisition(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.
bool testDevice::allow_DataAcquisition_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/*
* DataAcquisition setters
*/
void testDevice::PV_DataAcquisition_Gain_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataAcquisition_Offset_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataAcquisition_Bw_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataAcquisition_Resolution_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataAcquisition_Impedance_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataAcquisition_Coupling_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataAcquisition_SignalRef_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataAcquisition_Ground_Writer(const timespec& timestamp, const int32_t& value){

}

/**
* DataAcquisition getters
*/
void testDevice::PV_DataAcquisition_Gain_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataAcquisition_Offset_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataAcquisition_Bw_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataAcquisition_Resolution_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataAcquisition_Impedance_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataAcquisition_Coupling_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataAcquisition_SignalRef_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataAcquisition_Ground_Reader(timespec* timestamp, int32_t* value){

}

/*
* Body of function to acquire data
*/
void testDevice::DataAcquisition_thread_body(){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA GENERATION NODE*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* DataGeneration State Machine
*/

void testDevice::switchOn_DataGeneration(){

}

void testDevice::switchOff_DataGeneration(){

}

void testDevice::start_DataGeneration(){
	m_bStop_DataGeneration = false; //< We will set to true to stop the acquisition thread
	/**
	 *   Start the acquisition thread.
	 *   We don't need to check if the thread was already started because the state
	 *   machine guarantees that the start handler is called only while the state
	 *   is ON.
	 */
	m_DataGeneration_Thread = std::thread(std::bind(&testDevice::DataGeneration_thread_body, this));
}

void testDevice::stop_DataGeneration(){
	m_bStop_DataGeneration = true;
	m_DataGeneration_Thread.join();
}

void testDevice::recover_DataGeneration(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

bool testDevice::allow_DataGeneration_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/*
* DataGeneration setters
*/
void testDevice::PV_DataGeneration_Frequency_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_RefFrequency_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_Amp_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_Phase_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_UpdateRate_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_DutyCycle_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_Gain_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_Offset_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_Bw_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_Resolution_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_Impedance_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_DataGeneration_Coupling_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataGeneration_SignalRef_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataGeneration_SignalType_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataGeneration_Ground_Writer(const timespec& timestamp, const int32_t& value){

}



/**
* DataGeneration getters
*/
void testDevice::PV_DataGeneration_Frequency_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_RefFrequency_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_Amp_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_Phase_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_UpdateRate_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_DutyCycle_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_Gain_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_Offset_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_Bw_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_Resolution_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_Impedance_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DataGeneration_Coupling_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataGeneration_SignalRef_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataGeneration_SignalType_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataGeneration_Ground_Reader(timespec* timestamp, int32_t* value){

}

/*
* Body of function to generate data. In this example we are going to generate a sine wave.
*/
void testDevice::DataGeneration_thread_body(){

	// Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
	std::vector<double> outputData(m_DataGeneration.getMaxElements());

	// A counter for the angle in the sin() operation
	std::int64_t angle(0);
	// Run until the state machine stops us
	while(!m_bStop_DataGeneration){

		// Check Max amplitude
		size_t maxAmplitude = m_maxAmplitude.getValue(); // PVVariables are thread safe
		// Check wave type
		size_t signalType = m_DataGeneration.getSignalType(); // PVVariables are thread safe

		switch(signalType){

			// Fill the vector with a sin wave
			case 0:
				for(size_t scanVector(0); scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = (double)maxAmplitude * sin((double)(angle++) / 10.0f);
				}
				break;

			// Fill the vector with a square wave
			case 1:
				for(size_t scanVector(0); scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = ((angle & 0xff) < 128) ? maxAmplitude : - maxAmplitude;
				}
				break;

			// Fill the vector with a DC wave of maxAmplitude/2
			default:
				for(size_t scanVector(0); scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = maxAmplitude/2;
				}
				break;
		}

	// Push the vector to the control system
	//m_DataGeneration.push(m_DataGeneration.getTimestamp(), outputData);

	//TODO: Send values to data acquisition node.

	// Rest for a while
	::usleep(100000);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA PROCESSING
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control DataProcessing state machine
*/
void testDevice::switchOn_DataProcessing(){

}
void testDevice::switchOff_DataProcessing(){

}
void testDevice::start_DataProcessing(){

}
void testDevice::stop_DataProcessing(){

}
void testDevice::recover_DataProcessing(){

}

bool testDevice::allow_DataProcessing_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
* DataProcessing setters
*/
void testDevice::PV_DataProcessing_EnableFilter_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_FilterType_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_FilterParams_Writer(const timespec& timestamp, const std::vector<std::int32_t>& params){

}
void testDevice::PV_DataProcessing_EnableFFT_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_EnableSwFFT_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_FFTwindowType_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_FFTOverlap_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_FFTFrameSize_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_FFTSmooth_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_EnableDecimation_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_DecimationType_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_DecimationOffset_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DataProcessing_RAW2Eng_Writer(const timespec& timestamp, const int32_t& value){

}

/**
* DataProcessing getters
*/
void testDevice::PV_DataProcessing_EnableFilter_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_FilterType_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_FilterParams_Reader(timespec* timestamp, std::vector<std::int32_t>* value){

}
void testDevice::PV_DataProcessing_EnableFFT_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_EnableSwFFT_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_FFTwindowType_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_FFTOverlap_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_FFTFrameSize_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_FFTSmooth_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_EnableDecimation_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_DecimationType_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_DecimationOffset_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DataProcessing_RAW2Eng_Reader(timespec* timestamp, int32_t* value){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  DIGITAL I/O
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control DigitalIO state machine
*/
void testDevice::switchOn_DigitalIO(){

}
void testDevice::switchOff_DigitalIO(){

}
void testDevice::start_DigitalIO(){

}
void testDevice::stop_DigitalIO(){

}
void testDevice::recover_DigitalIO(){

}

bool testDevice::allow_DigitalIO_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
* DigitalIO setters
*/
void testDevice::PV_DigitalIO_voltLevelHigh_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DigitalIO_voltLevelLow_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_DigitalIO_ChannelDir_Writer(const timespec& timestamp, const int32_t& value){

}

/**
* DigitalIO getters
*/
void testDevice::PV_DigitalIO_voltLevelHigh_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DigitalIO_voltLevelLow_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DigitalIO_ChannelDir_Reader(timespec* timestamp, int32_t* value){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  DMA support
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control DMASupport state machine
 */
void testDevice::switchOn_DMASupport(){

}
void testDevice::switchOff_DMASupport(){

}
void testDevice::start_DMASupport(){

}
void testDevice::stop_DMASupport(){

}
void testDevice::recover_DMASupport(){

}

bool testDevice::allow_DMASupport_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * DMASupport setters
 */
void testDevice::PV_DMASupport_EnableDMA_Writer(const timespec& timestamp, const int32_t& value){

}

/**
 * DMASupport getters
 */
void testDevice::PV_DMASupport_BufferSize_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_DMASupport_EnableDMA_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DMASupport_NumDMAChannels_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DMASupport_DMAFrameType_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DMASupport_DMASampleSize_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_DMASupport_DMASamplingRate_Reader(timespec* timestamp, int32_t* value){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  STREAMING CONFIGURATION
///////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* Methods to control StreamingConf state machine
*/
void testDevice::switchOn_StreamingConf(){

}
void testDevice::switchOff_StreamingConf(){

}
void testDevice::start_StreamingConf(){

}
void testDevice::stop_StreamingConf(){

}
void testDevice::recover_StreamingConf(){

}

bool testDevice::allow_StreamingConf_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * StreamingConf setters
 */
void testDevice::PV_StreamingConf_StreamingType_Writer(const timespec& timestamp, const int32_t& value){

}

/**
 * StreamingConf getters
 */
void testDevice::PV_StreamingConf_StreamingDataFormat_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_StreamingConf_StreamingType_Reader(timespec* timestamp, int32_t* value){

}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//  IMAGE ACQUISITION
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control imageAcquisition state machine
 */
void testDevice::switchOn_imageAcquisition(){

}
void testDevice::switchOff_imageAcquisition(){

}
void testDevice::start_imageAcquisition(){

}
void testDevice::stop_imageAcquisition(){

}
void testDevice::recover_imageAcquisition(){

}

bool testDevice::allow_imageAcquisition_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * imageAcquisition setters
 */
void testDevice::PV_imageAcquisition_BinX_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_BinY_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_MinX_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_MinY_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_SizeX_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_SizeY_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_ReverseX_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_ReverseY_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_Resolution_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_SamplesPerPixel_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_AcquireTime_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_AcquirePeriod_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_Gain_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_FrameType_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_LostFrames_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_ImageMode_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_TriggerMode_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_NumExposures_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_Exposure_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_minExposure_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_maxExposure_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_ExposureStep_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_BlackLevel_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_NumImages_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_Acquire_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_ReadStatus_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_ShutterMode_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_ShutterControlMode_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_DelayStep_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_imageAcquisition_ShutterOpenDelay_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_ShutterMinOpenDelay_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_ShutterMaxOpenDelay_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_ShutterCloseDelay_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_ShutterMinCloseDelay_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_ShutterMaxCloseDelay_Writer(const timespec& timestamp, const double& value){

}
void testDevice::PV_imageAcquisition_HotPixels_Writer(const timespec& timestamp, const std::vector<std::int32_t>& value){

}
void testDevice::PV_imageAcquisition_HotPixelsCorr_Writer(const timespec& timestamp, const std::vector<std::int32_t>& value){

}
void testDevice::PV_imageAcquisition_Temperature_Writer(const timespec& timestamp, const double& value){

}

/**
 * imageAcquisition getters
 */
void testDevice::PV_imageAcquisition_MaxSizeX_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_MaxSizeY_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_BinX_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_BinY_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_MinX_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_MinY_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_SizeX_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_SizeY_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_ReverseX_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_ReverseY_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_Resolution_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_SamplesPerPixel_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_AcquireTime_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_AcquirePeriod_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_TimeRemaining_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_Gain_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_FrameType_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_LostFrames_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_ImageMode_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_TriggerMode_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_NumExposures_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_NumExposuresCounter_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_Exposure_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_minExposure_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_maxExposure_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_ExposureStep_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_BlackLevel_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_NumImages_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_NumImagesCounter_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_Acquire_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_DetectorState_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_StatusMessage_Reader(timespec* timestamp, std::string* value){

}
void testDevice::PV_imageAcquisition_StringToServer_Reader(timespec* timestamp, std::string* value){

}
void testDevice::PV_imageAcquisition_StringFromServer_Reader(timespec* timestamp, std::string* value){

}
void testDevice::PV_imageAcquisition_ShutterMode_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_ShutterControlMode_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_ShutterStatus_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_DelayStep_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_imageAcquisition_ShutterOpenDelay_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_ShutterMinOpenDelay_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_ShutterMaxOpenDelay_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_ShutterCloseDelay_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_ShutterMinCloseDelay_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_ShutterMaxCloseDelay_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_HotPixels_Reader(timespec* timestamp, std::vector<std::int32_t>* value){

}
void testDevice::PV_imageAcquisition_HotPixelsCorr_Reader(timespec* timestamp, std::vector<std::int32_t>* value){

}
void testDevice::PV_imageAcquisition_Temperature_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_imageAcquisition_ActualTemperature_Reader(timespec* timestamp, double* value){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  HEALTH MONITORING SUPPORT
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control HealthMonitSup state machine
 */
void testDevice::switchOn_HealthMonitSup(){

}
void testDevice::switchOff_HealthMonitSup(){

}
void testDevice::start_HealthMonitSup(){

}
void testDevice::stop_HealthMonitSup(){

}
void testDevice::recover_HealthMonitSup(){

}

bool testDevice::allow_HealthMonitSup_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * HealthMonitSup setters
 */
void testDevice::PV_HealthMonitSup_EnableSEU_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_HealthMonitSup_EnableMonitorDAQ_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_HealthMonitSup_EnableShelfTest_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_HealthMonitSup_ShelfTestType_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_HealthMonitSup_VerboseShelfTest_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_HealthMonitSup_EnableShelfTestId_Writer(const timespec& timestamp, const int32_t& value){

}
void testDevice::PV_HealthMonitSup_EnableShelfTestText_Writer(const timespec& timestamp, const int32_t& value){

}

/**
 * HealthMonitSup getters
 */
void testDevice::PV_HealthMonitSup_DevicePower_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_HealthMonitSup_DeviceTemp_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_HealthMonitSup_DeviceVoltage_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_HealthMonitSup_DeviceCurrent_Reader(timespec* timestamp, double* value){

}
void testDevice::PV_HealthMonitSup_EnableSEU_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_HealthMonitSup_EnableMonitorDAQ_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_HealthMonitSup_EnableShelfTest_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_HealthMonitSup_ShelfTestType_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_HealthMonitSup_VerboseShelfTest_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_HealthMonitSup_EnableShelfTestId_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_HealthMonitSup_EnableShelfTestText_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_HealthMonitSup_SignalQualityFlag_Reader(timespec* timestamp, int32_t* value){

}
void testDevice::PV_HealthMonitSup_SignalQualityFlagLevel_Reader(timespec* timestamp, double* value){

}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// EXTRA PVDELEGATE IN/OUT FOR TESTING PURPOSES
///////////////////////////////////////////////////////////////////////////////////////////////////

void testDevice::read_I32_DelegateIn(timespec* timestamp, std::int32_t* value){
	*value=PVDelegate_value_I32;
	*timestamp=timestamp_device;
}
void testDevice::read_DBL_DelegateIn(timespec* timestamp, double* value){
	*value=PVDelegate_value_DBL;
	*timestamp=timestamp_device;
}
void testDevice::read_vectorI8_DelegateIn(timespec* timestamp, std::vector<std::int8_t>* value){
	*value=PVDelegate_vector_I8;
	*timestamp=timestamp_device;
}
void testDevice::read_vectorUI8_DelegateIn(timespec* timestamp, std::vector<std::uint8_t>* value){
	*value=PVDelegate_vector_UI8;
	*timestamp=timestamp_device;
}
void testDevice::read_vectorI32_DelegateIn(timespec* timestamp, std::vector<std::int32_t>* value){
	*value=PVDelegate_vector_I32;
	*timestamp=timestamp_device;
}
void testDevice::read_vectorDBL_DelegateIn(timespec* timestamp, std::vector<double>* value){
	*value=PVDelegate_vector_DBL;
	*timestamp=timestamp_device;
}
void testDevice::read_string_DelegateIn(timespec* timestamp, std::string* value){
	*value=PVDelegate_value_string;
	*timestamp=timestamp_device;
}

void testDevice::write_I32_DelegateOut(const timespec& timestamp, const std::int32_t& value){
	PVDelegate_value_I32=value;
	timestamp_device=timestamp;
}
void testDevice::write_DBL_DelegateOut(const timespec& timestamp,const double& value){
	PVDelegate_value_DBL=value;
	timestamp_device=timestamp;
}
void testDevice::write_vectorI8_DelegateOut(const timespec& timestamp,const std::vector<std::int8_t>& value){
	PVDelegate_vector_I8=value;
	timestamp_device=timestamp;
}
void testDevice::write_vectorUI8_DelegateOut(const timespec& timestamp,const std::vector<std::uint8_t>& value){
	PVDelegate_vector_UI8=value;
	timestamp_device=timestamp;
}
void testDevice::write_vectorI32_DelegateOut(const timespec& timestamp,const std::vector<std::int32_t>& value){
	PVDelegate_vector_I32=value;
	timestamp_device=timestamp;
}
void testDevice::write_vectorDBL_DelegateOut(const timespec& timestamp,const std::vector<double>& value){
	PVDelegate_vector_DBL=value;
	timestamp_device=timestamp;
}
void testDevice::write_string_DelegateOut(const timespec& timestamp,const std::string& value){
	PVDelegate_value_string=value;
	timestamp_device=timestamp;
}

void testDevice::init_I32_DelegateOut(timespec* timestamp,  std::int32_t* value){
	*value=PVDelegate_value_I32;
	*timestamp=timestamp_device;
}
void testDevice::init_DBL_DelegateOut(timespec* timestamp, double* value){
	*value=PVDelegate_value_DBL;
	*timestamp=timestamp_device;
}
void testDevice::init_vectorI8_DelegateOut(timespec* timestamp, std::vector<std::int8_t>* value){
	*value=PVDelegate_vector_I8;
	*timestamp=timestamp_device;
}
void testDevice::init_vectorUI8_DelegateOut(timespec* timestamp, std::vector<std::uint8_t>* value){
	*value=PVDelegate_vector_UI8;
	*timestamp=timestamp_device;
}
void testDevice::init_vectorI32_DelegateOut(timespec* timestamp, std::vector<std::int32_t>* value){
	*value=PVDelegate_vector_I32;
	*timestamp=timestamp_device;
}
void testDevice::init_vectorDBL_DelegateOut(timespec* timestamp, std::vector<double>* value){
	*value=PVDelegate_vector_DBL;
	*timestamp=timestamp_device;
}
void testDevice::init_string_DelegateOut(timespec* timestamp, std::string* value){
	*value=PVDelegate_value_string;
	*timestamp=timestamp_device;
}




























//TODO: Study this methods

//void TestDevice::acquire(size_t numAcquisition, size_t numSamples)
//{
//    std::vector<std::int32_t> pushData0(numSamples);
//    std::vector<std::int32_t> pushData1(numSamples);
//
//    timespec timestamp = m_dataAcquisition.getStartTimestamp();
//
//    // Initialize vectors
//    for(size_t initializeVectors(0); initializeVectors != numSamples; ++initializeVectors)
//    {
//        pushData0[initializeVectors] = initializeVectors;
//        pushData1[initializeVectors] = numSamples - initializeVectors;
//    }
//    for(size_t acquisitionNumber(0); acquisitionNumber != numAcquisition; ++acquisitionNumber)
//    {
//        if((acquisitionNumber & 0x1) == 0)
//        {
//            m_dataAcquisition.push(timestamp, pushData0);
//        }
//        else
//        {
//            m_dataAcquisition.push(timestamp, pushData1);
//        }
//        timestamp.tv_sec++;
//        timestamp.tv_nsec++;
//    }
//}
//
//void TestDevice::readDelegate(timespec* pTimestamp, std::string* pValue)
//{
//    *pTimestamp = m_timestamp;
//    *pValue = m_writtenByDelegate;
//}
//
//void TestDevice::writeDelegate(const timespec& timestamp, const std::string& value)
//{
//    m_timestamp = timestamp;
//    m_writtenByDelegate = value;
//}
//
//void TestDevice::writeTestVariableIn(const timespec& timestamp, const std::string& value)
//{
//    m_testVariableIn.setValue(timestamp, value);
//}
//
//void TestDevice::pushTestVariableIn(const timespec& timestamp, const std::string& value)
//{
//    m_testVariableIn.push(timestamp, value);
//}
//
//void TestDevice::readTestVariableOut(timespec* pTimestamp, std::string* pValue)
//{
//    m_testVariableOut.getValue(pTimestamp, pValue);
//}


//TODO: Study this method
//timespec TestDevice::getCurrentTime()
//{
//    timespec time;
//    time.tv_sec = m_setCurrentTime.getValue();
//    time.tv_nsec = time.tv_sec + 10;
//    return time;
//}

