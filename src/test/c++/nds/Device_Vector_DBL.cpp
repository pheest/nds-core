
#include <nds3/nds.h>
#include <mutex>
#include <unistd.h>
#include <functional>

#include "../include/Device_Vector_DBL.h"

static std::map<std::string, DeviceVectorDBL*> m_devicesMap;
static std::mutex m_lockDevicesMap;

DeviceVectorDBL::DeviceVectorDBL(nds::Factory &factory, const std::string &deviceName, const nds::namedParameters_t &parameters):
	m_name(deviceName),

	PVVariable_value_I32(0),PVDelegate_value_I32(0),PVVariable_value_DBL(0),PVDelegate_value_DBL(0),PVVariable_vector_I8(2,0),PVDelegate_vector_I8(2,0),
	PVVariable_vector_UI8(2,0),	PVDelegate_vector_UI8(2,0),PVVariable_vector_I32(2,0),PVDelegate_vector_I32(2,0),PVVariable_vector_DBL(2,0),PVDelegate_vector_DBL(2,0),
	PVVariable_value_string{""},PVDelegate_value_string{""},

	timestamp_device{0,0},readtimeStamp{0,0},

	m_int32_DelegateIn("int32_DelegateIn",std::bind(&DeviceVectorDBL::read_I32_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_int32_DelegateOut("int32_DelegateOut",std::bind(&DeviceVectorDBL::write_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_int32_DelegateOut_init("int32_DelegateOut_init",std::bind(&DeviceVectorDBL::write_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceVectorDBL::init_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_double_DelegateIn("double_DelegateIn",std::bind(&DeviceVectorDBL::read_DBL_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_double_DelegateOut("double_DelegateOut",std::bind(&DeviceVectorDBL::write_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_double_DelegateOut_init("double_DelegateOut_init",std::bind(&DeviceVectorDBL::write_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceVectorDBL::init_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorI8_DelegateIn("vectorI8_DelegateIn",std::bind(&DeviceVectorDBL::read_vectorI8_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI8_DelegateOut("vectorI8_DelegateOut",std::bind(&DeviceVectorDBL::write_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI8_DelegateOut_init("vectorI8_DelegateOut_init",std::bind(&DeviceVectorDBL::write_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceVectorDBL::init_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorUI8_DelegateIn("vectorUI8_DelegateIn",std::bind(&DeviceVectorDBL::read_vectorUI8_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorUI8_DelegateOut("vectorUI8_DelegateOut",std::bind(&DeviceVectorDBL::write_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorUI8_DelegateOut_init("vectorUI8_DelegateOut_init",std::bind(&DeviceVectorDBL::write_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceVectorDBL::init_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorI32_DelegateIn("vectorI32_DelegateIn",std::bind(&DeviceVectorDBL::read_vectorI32_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI32_DelegateOut("vectorI32_DelegateOut",std::bind(&DeviceVectorDBL::write_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI32_DelegateOut_init("vectorI32_DelegateOut_init",std::bind(&DeviceVectorDBL::write_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceVectorDBL::init_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorDBL_DelegateIn("vectorDBL_DelegateIn",std::bind(&DeviceVectorDBL::read_vectorDBL_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorDBL_DelegateOut("vectorDBL_DelegateOut",std::bind(&DeviceVectorDBL::write_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorDBL_DelegateOut_init("vectorDBL_DelegateOut_init",std::bind(&DeviceVectorDBL::write_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceVectorDBL::init_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_string_DelegateIn("string_DelegateIn",std::bind(&DeviceVectorDBL::read_string_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_string_DelegateOut("string_DelegateOut",std::bind(&DeviceVectorDBL::write_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_string_DelegateOut_init("string_DelegateOut_init",std::bind(&DeviceVectorDBL::write_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&DeviceVectorDBL::init_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_delegateIn("delegateIn", std::bind(&DeviceVectorDBL::readDelegate, this, std::placeholders::_1, std::placeholders::_2)),
	m_delegateOut("delegateOut", std::bind(&DeviceVectorDBL::writeDelegate, this, std::placeholders::_1, std::placeholders::_2)),
	m_writeTestVariableIn("writeTestVariableIn", std::bind(&DeviceVectorDBL::writeTestVariableIn, this, std::placeholders::_1, std::placeholders::_2)),
	m_pushTestVariableIn("pushTestVariableIn", std::bind(&DeviceVectorDBL::pushTestVariableIn, this, std::placeholders::_1, std::placeholders::_2)),
	m_readTestVariableOut("readTestVariableOut", std::bind(&DeviceVectorDBL::readTestVariableOut, this, std::placeholders::_1, std::placeholders::_2))
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
	m_DeviceVectorDBL_stateMachine = rootNode.addChild(nds::StateMachine(true,
			std::bind(&DeviceVectorDBL::switchOn_DeviceVectorDBL, this),
			std::bind(&DeviceVectorDBL::switchOff_DeviceVectorDBL, this),
			std::bind(&DeviceVectorDBL::start_DeviceVectorDBL, this),
			std::bind(&DeviceVectorDBL::stop_DeviceVectorDBL, this),
			std::bind(&DeviceVectorDBL::recover_DeviceVectorDBL, this),
			std::bind(&DeviceVectorDBL::allow__DeviceVectorDBL_Change,this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

	/**
	 * Add a DataAcquisition node: it acquires data generated by the DataGeneration node and supplies an input PV on which we can push the
	 * acquired data, it also adds a state machine that allows to start and stop the acquisition.
	 */
	m_DataAcquisition = rootNode.addChild(nds::DataAcquisition<std::vector<double> >(
			"DataAcquisitionNode",
			128,
			std::bind(&DeviceVectorDBL::switchOn_DataAcquisition, this),
			std::bind(&DeviceVectorDBL::switchOff_DataAcquisition, this),
			std::bind(&DeviceVectorDBL::start_DataAcquisition, this),
			std::bind(&DeviceVectorDBL::stop_DataAcquisition, this),
			std::bind(&DeviceVectorDBL::recover_DataAcquisition, this),
			std::bind(&DeviceVectorDBL::allow_DataAcquisition_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&DeviceVectorDBL::PV_DataAcquisition_Gain_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataAcquisition_Offset_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataAcquisition_Bandwidth_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataAcquisition_Resolution_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataAcquisition_Impedance_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataAcquisition_Coupling_Writer,this,   std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataAcquisition_SignalRef_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataAcquisition_Ground_Writer,this, std::placeholders::_1, std::placeholders::_2)
	));
//
//	/**
//	 * Add a DataGeneration node: it acquires data generated by the DataGeneration node and supplies an input PV on which we can push the
//	 * acquired data, it also adds a state machine that allows to start and stop the acquisition.
//	 */
	m_DataGeneration = rootNode.addChild(nds::DataGeneration<std::vector<double>>(
			"DataGenerationNode",
			128,
			std::bind(&DeviceVectorDBL::switchOn_DataGeneration, this),
			std::bind(&DeviceVectorDBL::switchOff_DataGeneration, this),
			std::bind(&DeviceVectorDBL::start_DataGeneration, this),
			std::bind(&DeviceVectorDBL::stop_DataGeneration, this),
			std::bind(&DeviceVectorDBL::recover_DataGeneration, this),
			std::bind(&DeviceVectorDBL::allow_DataGeneration_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Frequency_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_RefFrequency_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Amp_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Phase_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_UpdateRate_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_DutyCycle_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Gain_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Offset_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Bandwidth_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Resolution_Writer,this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Impedance_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Coupling_Writer,this,   std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_SignalRef_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_SignalType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
			std::bind(&DeviceVectorDBL::PV_DataGeneration_Ground_Writer,this, std::placeholders::_1, std::placeholders::_2)
	));

	/**
	      * Add a DataProcessing node:
	      */
	    m_DataProcessing = rootNode.addChild(nds::DataProcessing<std::vector<int32_t> >(
	     		"DataProcessingNode",
	 			128,
	 			std::bind(&DeviceVectorDBL::switchOn_DataProcessing, this),
	 			std::bind(&DeviceVectorDBL::switchOff_DataProcessing, this),
	 			std::bind(&DeviceVectorDBL::start_DataProcessing, this),
	 			std::bind(&DeviceVectorDBL::stop_DataProcessing, this),
	 			std::bind(&DeviceVectorDBL::recover_DataProcessing, this),
	 			std::bind(&DeviceVectorDBL::allow_DataProcessing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_EnableFilter_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_EnableFilter_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FilterType_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FilterType_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FilterParams_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FilterParams_Reader,this, std::placeholders::_1, std::placeholders::_2),
				128,
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_EnableFFT_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_EnableFFT_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_EnableSwFFT_Writer,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_EnableSwFFT_Reader,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FFTwindowType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FFTwindowType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FFTOverlap_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FFTOverlap_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FFTFrameSize_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FFTFrameSize_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FFTSmooth_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_FFTSmooth_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_EnableDecimation_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_EnableDecimation_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_DecimationType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_DecimationType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_DecimationOffset_Writer,this,   std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_DecimationOffset_Reader,this,   std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_RAW2Eng_Writer,this,  std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DataProcessing_RAW2Eng_Reader,this,  std::placeholders::_1, std::placeholders::_2)
	     ));

	    /**
	     * Add a Digital I/O node:
	     */
	    m_DigitalIO = rootNode.addChild(nds::DigitalIO<std::vector<std::uint8_t> >(
	    		"DigitalIONode",
				128,
				std::bind(&DeviceVectorDBL::switchOn_DigitalIO, this),
				std::bind(&DeviceVectorDBL::switchOff_DigitalIO, this),
				std::bind(&DeviceVectorDBL::start_DigitalIO, this),
				std::bind(&DeviceVectorDBL::stop_DigitalIO, this),
				std::bind(&DeviceVectorDBL::recover_DigitalIO, this),
				std::bind(&DeviceVectorDBL::allow_DigitalIO_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&DeviceVectorDBL::PV_DigitalIO_voltLevelHigh_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_DigitalIO_voltLevelLow_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_DigitalIO_ChannelDir_Writer,this, std::placeholders::_1, std::placeholders::_2)
	    ));

	    /**
	      * Add a DMA Support node:
	      */
	    m_DMASupport = rootNode.addChild(nds::DMASupport<std::vector<int32_t> >(
	     		"DMASupportNode",
	 			128,
	 			std::bind(&DeviceVectorDBL::switchOn_DMASupport, this),
	 			std::bind(&DeviceVectorDBL::switchOff_DMASupport, this),
	 			std::bind(&DeviceVectorDBL::start_DMASupport, this),
	 			std::bind(&DeviceVectorDBL::stop_DMASupport, this),
	 			std::bind(&DeviceVectorDBL::recover_DMASupport, this),
	 			std::bind(&DeviceVectorDBL::allow_DMASupport_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
	 			std::bind(&DeviceVectorDBL::PV_DMASupport_BufferSize_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DMASupport_EnableDMA_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DMASupport_EnableDMA_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DMASupport_NumDMAChannels_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DMASupport_DMAFrameType_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_DMASupport_DMASampleSize_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_DMASupport_DMASamplingRate_Reader,this, std::placeholders::_1, std::placeholders::_2)
	     ));

	    /**
	      * Add a Streaming Config node:
	      */
	    m_StreamingConf = rootNode.addChild(nds::StreamingConf<std::vector<int32_t> >(
	     		"StreamingConfNode",
	 			128,
	 			std::bind(&DeviceVectorDBL::switchOn_StreamingConf, this),
	 			std::bind(&DeviceVectorDBL::switchOff_StreamingConf, this),
	 			std::bind(&DeviceVectorDBL::start_StreamingConf, this),
	 			std::bind(&DeviceVectorDBL::stop_StreamingConf, this),
	 			std::bind(&DeviceVectorDBL::recover_StreamingConf, this),
	 			std::bind(&DeviceVectorDBL::allow_StreamingConf_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
	 			std::bind(&DeviceVectorDBL::PV_StreamingConf_StreamingDataFormat_Reader,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_StreamingConf_StreamingType_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&DeviceVectorDBL::PV_StreamingConf_StreamingType_Reader,this, std::placeholders::_1, std::placeholders::_2)
	     ));

	    /**
	     * Add a HealthMonitSup node.
	     */
	    m_HealthMonitSup = rootNode.addChild(nds::HealthMonitSup<std::vector<std::int32_t> >(
	    		"HealthMonitSupNode",
				std::bind(&DeviceVectorDBL::switchOn_HealthMonitSup, this),
				std::bind(&DeviceVectorDBL::switchOff_HealthMonitSup, this),
				std::bind(&DeviceVectorDBL::start_HealthMonitSup, this),
				std::bind(&DeviceVectorDBL::stop_HealthMonitSup, this),
				std::bind(&DeviceVectorDBL::recover_HealthMonitSup, this),
				std::bind(&DeviceVectorDBL::allow_HealthMonitSup_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_DevicePower_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_DeviceTemp_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_DeviceVoltage_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_DeviceCurrent_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableSEU_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableSEU_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableMonitorDAQ_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableMonitorDAQ_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTest_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTest_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_ShelfTestType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_ShelfTestType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_VerboseShelfTest_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_VerboseShelfTest_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTestId_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTestId_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTestText_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTestText_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_SignalQualityFlag_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_HealthMonitSup_SignalQualityFlagLevel_Reader,this, std::placeholders::_1, std::placeholders::_2)
	    ));


	    /**
	     * Add a imageAcquisition node.
	     */
	    m_imageAcquisition = rootNode.addChild(nds::imageAcquisition<std::vector<double> >(
	    		"imageAcquisitionNode",
				128,
				std::bind(&DeviceVectorDBL::switchOn_imageAcquisition, this),
				std::bind(&DeviceVectorDBL::switchOff_imageAcquisition, this),
				std::bind(&DeviceVectorDBL::start_imageAcquisition, this),
				std::bind(&DeviceVectorDBL::stop_imageAcquisition, this),
				std::bind(&DeviceVectorDBL::recover_imageAcquisition, this),
				std::bind(&DeviceVectorDBL::allow_imageAcquisition_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_MaxSizeX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_MaxSizeY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_BinX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_BinX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_BinY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_BinY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_MinX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_MinX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_MinY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_MinY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_SizeX_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_SizeX_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_SizeY_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_SizeY_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ReverseX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ReverseX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ReverseY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ReverseY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Resolution_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Resolution_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_SamplesPerPixel_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_SamplesPerPixel_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_AcquireTime_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_AcquireTime_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_AcquirePeriod_Writer,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_AcquirePeriod_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_TimeRemaining_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Gain_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Gain_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_FrameType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_FrameType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_LostFrames_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_LostFrames_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ImageMode_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ImageMode_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_TriggerMode_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_TriggerMode_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_NumExposures_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_NumExposures_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_NumExposuresCounter_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Exposure_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Exposure_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_minExposure_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_minExposure_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_maxExposure_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_maxExposure_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ExposureStep_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ExposureStep_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_BlackLevel_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_BlackLevel_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_NumImages_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_NumImages_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_NumImagesCounter_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Acquire_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Acquire_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_DetectorState_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_StatusMessage_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_StringToServer_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_StringFromServer_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ReadStatus_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMode_Writer,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMode_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterControlMode_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterControlMode_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterStatus_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_DelayStep_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_DelayStep_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterOpenDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterOpenDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMinOpenDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMinOpenDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMaxOpenDelay_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMaxOpenDelay_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterCloseDelay_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterCloseDelay_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMinCloseDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMinCloseDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMaxCloseDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ShutterMaxCloseDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_HotPixels_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_HotPixels_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_HotPixelsCorr_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_HotPixelsCorr_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Temperature_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_Temperature_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&DeviceVectorDBL::PV_imageAcquisition_ActualTemperature_Reader,this,  std::placeholders::_1, std::placeholders::_2)
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

	    m_testVariableIn= rootNode.addChild(nds::PVVariableIn<std::string>("testVariableIn"));
	    m_testVariableOut= rootNode.addChild(nds::PVVariableOut<std::string>("testVariableOut"));

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

	    rootNode.addChild(m_delegateIn);
	    rootNode.addChild(m_delegateOut);
	    rootNode.addChild(m_writeTestVariableIn);
	    rootNode.addChild(m_pushTestVariableIn);
	    rootNode.addChild(m_readTestVariableOut);

	    m_setCurrentTime = rootNode.addChild(nds::PVVariableOut<std::int32_t>("setCurrentTime"));

	    rootNode.setTimestampDelegate(std::bind(&DeviceVectorDBL::getCurrentTime,this));




	// We have declared all the nodes and PVs in our device: now we register them
	//  with the control system that called this constructor.
	////////////////////////////////////////////////////////////////////////////////
	rootNode.initialize(this, factory);
}



DeviceVectorDBL::~DeviceVectorDBL()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_devicesMap.erase(m_name);

}

DeviceVectorDBL* DeviceVectorDBL::getInstance(const std::string& deviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, DeviceVectorDBL*>::const_iterator findDevice = m_devicesMap.find(deviceName);
    if(findDevice == m_devicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

/*
 * Allocation function
 *********************/
void* DeviceVectorDBL::allocateDevice(nds::Factory& factory, const std::string& deviceName, const nds::namedParameters_t& parameters)
{
    return new DeviceVectorDBL(factory, deviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void DeviceVectorDBL::deallocateDevice(void* deviceName)
{
    delete (DeviceVectorDBL*)deviceName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  TEST DEVICE STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control DeviceVectorDBL state machine
 */
void DeviceVectorDBL::switchOn_DeviceVectorDBL(){

}
void DeviceVectorDBL::switchOff_DeviceVectorDBL(){

}
void DeviceVectorDBL::start_DeviceVectorDBL(){

}
void DeviceVectorDBL::stop_DeviceVectorDBL(){

}
void DeviceVectorDBL::recover_DeviceVectorDBL(){

}

bool DeviceVectorDBL::allow__DeviceVectorDBL_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA ACQUISITION NODE*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* DataAcquisition State Machine
*/

// Called when the DataAcquisition node has to be switched on.
void DeviceVectorDBL::switchOn_DataAcquisition(){

}

// Called when the DataAcquisition node has to be switched off.
void DeviceVectorDBL::switchOff_DataAcquisition(){

}

// Called when the DataAcquisition node has to start acquiring. We start the data acquisition thread.
void DeviceVectorDBL::start_DataAcquisition(){

	m_bStop_DataAcquisition = false; //< We will set to true to stop the acquisition thread
	/**
	 *   Start the acquisition thread.
	 *   We don't need to check if the thread was already started because the state
	 *   machine guarantees that the start handler is called only while the state
	 *   is ON.
	 */
	m_DataAcquisition_Thread = std::thread(std::bind(&DeviceVectorDBL::DataAcquisition_thread_body, this));
}

// Stop the DataAcquisition node thread
void DeviceVectorDBL::stop_DataAcquisition(){
	m_bStop_DataAcquisition = true;
	m_DataAcquisition_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.
void DeviceVectorDBL::recover_DataAcquisition(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.
bool DeviceVectorDBL::allow_DataAcquisition_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/*
* DataAcquisition setters
*/
void DeviceVectorDBL::PV_DataAcquisition_Gain_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Gain to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Gain programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataAcquisition.setGain(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataAcquisition_Offset_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Offset to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Offset programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataAcquisition.setOffset(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataAcquisition_Bandwidth_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Bandwidth to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Bandwidth programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataAcquisition.setBandwidth(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataAcquisition_Resolution_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Resolution to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Resolution programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataAcquisition.setResolution(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataAcquisition_Impedance_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Impedance to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Impedance programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataAcquisition.setImpedance(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataAcquisition_Coupling_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t HW_value;
	//Value has the Coupling to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Coupling programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataAcquisition.setCoupling(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataAcquisition_SignalRef_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t HW_value;
	//Value has the SignalRef to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real SignalRef programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataAcquisition.setSignalRef(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataAcquisition_Ground_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t HW_value;
	//Value has the Ground to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Ground programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataAcquisition.setGround(timestamp,HW_value);
}

/*
* Body of function to acquire data
*/
void DeviceVectorDBL::DataAcquisition_thread_body(){
	// Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
		std::vector<double> outputData(m_DataAcquisition.getMaxElements(),0);

		double counter(0);

		//Counter for number of pushed data blocks
		std::int32_t NumberOfPushedDataBlocks(0);

		// Get Gain
		double Gain = m_DataAcquisition.getGain();
		// Get Bandwidth
		double Bandwidth = m_DataAcquisition.getBandwidth();
		// Get Resolution
		double Resolution = m_DataAcquisition.getResolution();
		// Get Coupling
		double Coupling = m_DataAcquisition.getCoupling();
		// Get SignalRef
		double SignalRef = m_DataAcquisition.getSignalRef();
		// Get Ground
		double Ground = m_DataAcquisition.getGround();
		// Get offset
		double Offset = m_DataAcquisition.getOffset();
		// Get impedance
		std::int32_t Impedance = m_DataAcquisition.getImpedance();

		std::cout<<"\tGain = "<<Gain<<std::endl;
		std::cout<<"\tBandwidth = "<<Bandwidth<<std::endl;
		std::cout<<"\tResolution = "<<Resolution<<std::endl;
		std::cout<<"\tCoupling = "<<Coupling<<std::endl;
		std::cout<<"\tSignalRef = "<<SignalRef<<std::endl;
		std::cout<<"\tGround = "<<Ground<<std::endl;
		std::cout<<"\tOffset = "<<Offset<<std::endl;
		std::cout<<"\tImpedance = "<<Impedance<<std::endl;
		// Run until the state machine stops us
		while(!m_bStop_DataAcquisition){

			size_t scanVector(0);
			for(scanVector=0; scanVector != outputData.size(); ++scanVector){
				outputData[scanVector] = counter;
			}
			counter++;

		// Push the vector to the control system
		m_DataAcquisition.push(m_DataAcquisition.getTimestamp(), outputData);
		++NumberOfPushedDataBlocks;
		std::cout<<"outputData=";
		for(scanVector=0; scanVector != outputData.size(); ++scanVector){
			std::cout<<outputData[scanVector];
		}
		std::cout<<std::endl;
		// Rest for a while
		::usleep(100000);
		}
		m_DataAcquisition.setNumberOfPushedDataBlocks(m_DataAcquisition.getTimestamp(),NumberOfPushedDataBlocks);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA GENERATION NODE*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* DataGeneration State Machine
*/

void DeviceVectorDBL::switchOn_DataGeneration(){

}

void DeviceVectorDBL::switchOff_DataGeneration(){

}

void DeviceVectorDBL::start_DataGeneration(){
	m_bStop_DataGeneration = false; //< We will set to true to stop the acquisition thread
	/**
	 *   Start the acquisition thread.
	 *   We don't need to check if the thread was already started because the state
	 *   machine guarantees that the start handler is called only while the state
	 *   is ON.
	 */
	m_DataGeneration_Thread = std::thread(std::bind(&DeviceVectorDBL::DataGeneration_thread_body, this));
}

void DeviceVectorDBL::stop_DataGeneration(){
	m_bStop_DataGeneration = true;
	m_DataGeneration_Thread.join();
}

void DeviceVectorDBL::recover_DataGeneration(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

bool DeviceVectorDBL::allow_DataGeneration_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/*
* DataGeneration setters
*/
void DeviceVectorDBL::PV_DataGeneration_Frequency_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the frequency to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real frequency programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setFrequency(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_RefFrequency_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the RefFrequency to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real RefFrequency programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setRefFrequency(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Amp_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//value has the amplitude to be programmed on the hardware
	//call to function programming the hardware. This function should return the real amplitude programmed. This value has to be set to the readback attribute.
	//in the meantime without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setAmplitude(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Phase_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Phase to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Phase programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setPhase(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_UpdateRate_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the UpdateRate to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real UpdateRate programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setUpdateRate(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_DutyCycle_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the DutyCycle to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real DutyCycle programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setDutyCycle(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Gain_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Gain to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Gain programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setGain(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Offset_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Offset to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Offset programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setOffset(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Bandwidth_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Bandwidth to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Bandwidth programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setBandwidth(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Resolution_Writer(const timespec& timestamp, const double& value){
	double HW_value;
	//Value has the Resolution to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Resolution programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setResolution(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Impedance_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t HW_value;
	//Value has the Impedance to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Impedance programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setImpedance(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Coupling_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t HW_value;
	//Value has the Coupling to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real Coupling programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setCoupling(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_SignalRef_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t HW_value;
	//Value has the SignalRef to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real SignalRef programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setSignalRef(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_SignalType_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t HW_value;
	//value has the SignalType to be programmed on the hardware
	//call to function programming the hardware. This function should return the real SignalType programmed. This value has to be set to the readback attribute.
	//in the meantime without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setSignalType(timestamp,HW_value);
}
void DeviceVectorDBL::PV_DataGeneration_Ground_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t HW_value;
	//value has the Ground to be programmed on the hardware
	//call to function programming the hardware. This function should return the real Ground programmed. This value has to be set to the readback attribute.
	//in the meantime without real hardware value and  HW_value are equal.
	HW_value=value;
	m_DataGeneration.setGround(timestamp,HW_value);
}

/*
* Body of function to generate data. In this example we are going to generate a sine wave.
*/
void DeviceVectorDBL::DataGeneration_thread_body(){

	// Let's allocate a vector that will contain the data that we will push to the control system or to the data acquisition node
	std::vector<double> outputData(m_DataGeneration.getMaxElements(),0);

	//Counter for number of pushed data blocks
	std::int32_t NumberOfPushedDataBlocks(0);

	// A counter for the angle in the sin() operation
	std::int64_t angle(0);

	size_t last_sample(0);

	// Get RefFrequency
	double RefFrequency = m_DataGeneration.getRefFrequency();
	// Get DutyCycle
	double DutyCycle = m_DataGeneration.getDutyCycle();
	// Get Gain
	double Gain = m_DataGeneration.getGain();
	// Get Bandwidth
	double Bandwidth = m_DataGeneration.getBandwidth();
	// Get Resolution
	double Resolution = m_DataGeneration.getResolution();
	// Get Coupling
	double Coupling = m_DataGeneration.getCoupling();
	// Get SignalRef
	double SignalRef = m_DataGeneration.getSignalRef();
	// Get Ground
	double Ground = m_DataGeneration.getGround();

	std::cout<<"Signal generator configured with:"<<std::endl;
	std::cout<<"\tRefFrequency = "<<RefFrequency<<std::endl;
	std::cout<<"\tDutyCycle = "<<DutyCycle<<std::endl;
	std::cout<<"\tGain = "<<Gain<<std::endl;
	std::cout<<"\tBandwidth = "<<Bandwidth<<std::endl;
	std::cout<<"\tResolution = "<<Resolution<<std::endl;
	std::cout<<"\tCoupling = "<<Coupling<<std::endl;
	std::cout<<"\tSignalRef = "<<SignalRef<<std::endl;
	std::cout<<"\tGround = "<<Ground<<std::endl;

	// Run until the state machine stops us
	while(!m_bStop_DataGeneration){

		size_t scanVector(0);


		// Get signalType
		size_t signalType = m_DataGeneration.getSignalType();
		// Get amplitude
		double amplitude = m_DataGeneration.getAmplitude();
		// Get frequency
		double frequency = m_DataGeneration.getFrequency();
		// Get updateRate
		double updateRate = m_DataGeneration.getUpdateRate();
		// Get offset
		double offset = m_DataGeneration.getOffset();
		// Get phase
		double phase = m_DataGeneration.getPhase();
		// Get phase
		std::int32_t impedance = m_DataGeneration.getImpedance();

		switch(signalType){

			case 0:
				for(scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = amplitude;
				}
				last_sample+=scanVector;
				break;
			case 1:
				for(scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = amplitude;
				}
				break;
			case 2:
				for(scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = amplitude;
				}
				break;
			case 3:
				for(scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = (double)amplitude * sin((2*M_PI*(scanVector+last_sample)*frequency)/updateRate + phase) + offset;
				}
				break;
			case 4:
				for(scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = ((angle & 0xff) < 128) ? amplitude : - amplitude;
				}
				break;
			case 5:
				for(scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = amplitude;
				}
				break;
			case 6:
				for(scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = amplitude;
				}
				break;
			case 7:
				for(scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = amplitude;
				}
				break;
			default:
				for( scanVector=0; scanVector != outputData.size(); ++scanVector){
					outputData[scanVector] = amplitude;
				}
				break;
		}

		//Save last sample generated.
		last_sample+=scanVector;

	// Push the vector to the control system
	m_DataGeneration.push(m_DataGeneration.getTimestamp(), outputData);
	++NumberOfPushedDataBlocks;
	//TODO: Send values to data acquisition node.

	// Rest for a while
	::usleep(100000);
	}
	m_DataGeneration.setNumberOfPushedDataBlocks(m_DataGeneration.getTimestamp(),NumberOfPushedDataBlocks);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA PROCESSING
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control DataProcessing state machine
*/
void DeviceVectorDBL::switchOn_DataProcessing(){

}
void DeviceVectorDBL::switchOff_DataProcessing(){

}
void DeviceVectorDBL::start_DataProcessing(){

}
void DeviceVectorDBL::stop_DataProcessing(){

}
void DeviceVectorDBL::recover_DataProcessing(){

}

bool DeviceVectorDBL::allow_DataProcessing_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
* DataProcessing setters
*/
void DeviceVectorDBL::PV_DataProcessing_EnableFilter_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FilterType_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FilterParams_Writer(const timespec& /*timestamp*/, const std::vector<std::int32_t>& /*params*/){

}
void DeviceVectorDBL::PV_DataProcessing_EnableFFT_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_EnableSwFFT_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FFTwindowType_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FFTOverlap_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FFTFrameSize_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FFTSmooth_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_EnableDecimation_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_DecimationType_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_DecimationOffset_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_RAW2Eng_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}

/**
* DataProcessing getters
*/
void DeviceVectorDBL::PV_DataProcessing_EnableFilter_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FilterType_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FilterParams_Reader(timespec* /*timestamp*/, std::vector<std::int32_t>* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_EnableFFT_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_EnableSwFFT_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FFTwindowType_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FFTOverlap_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FFTFrameSize_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_FFTSmooth_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_EnableDecimation_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_DecimationType_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_DecimationOffset_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DataProcessing_RAW2Eng_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  DIGITAL I/O
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control DigitalIO state machine
*/
void DeviceVectorDBL::switchOn_DigitalIO(){

}
void DeviceVectorDBL::switchOff_DigitalIO(){

}
void DeviceVectorDBL::start_DigitalIO(){

}
void DeviceVectorDBL::stop_DigitalIO(){

}
void DeviceVectorDBL::recover_DigitalIO(){

}

bool DeviceVectorDBL::allow_DigitalIO_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
* DigitalIO setters
*/
void DeviceVectorDBL::PV_DigitalIO_voltLevelHigh_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DigitalIO_voltLevelLow_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_DigitalIO_ChannelDir_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  DMA support
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control DMASupport state machine
 */
void DeviceVectorDBL::switchOn_DMASupport(){

}
void DeviceVectorDBL::switchOff_DMASupport(){

}
void DeviceVectorDBL::start_DMASupport(){

}
void DeviceVectorDBL::stop_DMASupport(){

}
void DeviceVectorDBL::recover_DMASupport(){

}

bool DeviceVectorDBL::allow_DMASupport_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * DMASupport setters
 */
void DeviceVectorDBL::PV_DMASupport_EnableDMA_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}

/**
 * DMASupport getters
 */
void DeviceVectorDBL::PV_DMASupport_BufferSize_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_DMASupport_EnableDMA_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DMASupport_NumDMAChannels_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DMASupport_DMAFrameType_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DMASupport_DMASampleSize_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_DMASupport_DMASamplingRate_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  STREAMING CONFIGURATION
///////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* Methods to control StreamingConf state machine
*/
void DeviceVectorDBL::switchOn_StreamingConf(){

}
void DeviceVectorDBL::switchOff_StreamingConf(){

}
void DeviceVectorDBL::start_StreamingConf(){

}
void DeviceVectorDBL::stop_StreamingConf(){

}
void DeviceVectorDBL::recover_StreamingConf(){

}

bool DeviceVectorDBL::allow_StreamingConf_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * StreamingConf setters
 */
void DeviceVectorDBL::PV_StreamingConf_StreamingType_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}

/**
 * StreamingConf getters
 */
void DeviceVectorDBL::PV_StreamingConf_StreamingDataFormat_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_StreamingConf_StreamingType_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}


///////////////////////////////////////////////////////////////////////////////////////////////////////
//  IMAGE ACQUISITION
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control imageAcquisition state machine
 */
void DeviceVectorDBL::switchOn_imageAcquisition(){

}
void DeviceVectorDBL::switchOff_imageAcquisition(){

}
void DeviceVectorDBL::start_imageAcquisition(){

}
void DeviceVectorDBL::stop_imageAcquisition(){

}
void DeviceVectorDBL::recover_imageAcquisition(){

}

bool DeviceVectorDBL::allow_imageAcquisition_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * imageAcquisition setters
 */
void DeviceVectorDBL::PV_imageAcquisition_BinX_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_BinY_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_MinX_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_MinY_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_SizeX_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_SizeY_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ReverseX_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ReverseY_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Resolution_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_SamplesPerPixel_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_AcquireTime_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_AcquirePeriod_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Gain_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_FrameType_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_LostFrames_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ImageMode_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_TriggerMode_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_NumExposures_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Exposure_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_minExposure_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_maxExposure_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ExposureStep_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_BlackLevel_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_NumImages_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Acquire_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ReadStatus_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMode_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterControlMode_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_DelayStep_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterOpenDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMinOpenDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMaxOpenDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterCloseDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMinCloseDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMaxCloseDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_HotPixels_Writer(const timespec& /*timestamp*/, const std::vector<std::int32_t>& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_HotPixelsCorr_Writer(const timespec& /*timestamp*/, const std::vector<std::int32_t>& /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Temperature_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}

/**
 * imageAcquisition getters
 */
void DeviceVectorDBL::PV_imageAcquisition_MaxSizeX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_MaxSizeY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_BinX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_BinY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_MinX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_MinY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_SizeX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_SizeY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ReverseX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ReverseY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Resolution_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_SamplesPerPixel_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_AcquireTime_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_AcquirePeriod_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_TimeRemaining_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Gain_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_FrameType_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_LostFrames_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ImageMode_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_TriggerMode_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_NumExposures_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_NumExposuresCounter_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Exposure_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_minExposure_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_maxExposure_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ExposureStep_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_BlackLevel_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_NumImages_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_NumImagesCounter_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Acquire_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_DetectorState_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_StatusMessage_Reader(timespec* /*timestamp*/, std::string* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_StringToServer_Reader(timespec* /*timestamp*/, std::string* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_StringFromServer_Reader(timespec* /*timestamp*/, std::string* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMode_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterControlMode_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterStatus_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_DelayStep_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterOpenDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMinOpenDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMaxOpenDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterCloseDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMinCloseDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ShutterMaxCloseDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_HotPixels_Reader(timespec* /*timestamp*/, std::vector<std::int32_t>* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_HotPixelsCorr_Reader(timespec* /*timestamp*/, std::vector<std::int32_t>* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_Temperature_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_imageAcquisition_ActualTemperature_Reader(timespec* /*timestamp*/, double* /*value*/){

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  HEALTH MONITORING SUPPORT
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control HealthMonitSup state machine
 */
void DeviceVectorDBL::switchOn_HealthMonitSup(){

}
void DeviceVectorDBL::switchOff_HealthMonitSup(){

}
void DeviceVectorDBL::start_HealthMonitSup(){

}
void DeviceVectorDBL::stop_HealthMonitSup(){

}
void DeviceVectorDBL::recover_HealthMonitSup(){

}

bool DeviceVectorDBL::allow_HealthMonitSup_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * HealthMonitSup setters
 */
void DeviceVectorDBL::PV_HealthMonitSup_EnableSEU_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableMonitorDAQ_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTest_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_ShelfTestType_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_VerboseShelfTest_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTestId_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTestText_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}

/**
 * HealthMonitSup getters
 */
void DeviceVectorDBL::PV_HealthMonitSup_DevicePower_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_DeviceTemp_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_DeviceVoltage_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_DeviceCurrent_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableSEU_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableMonitorDAQ_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTest_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_ShelfTestType_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_VerboseShelfTest_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTestId_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_EnableShelfTestText_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_SignalQualityFlag_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void DeviceVectorDBL::PV_HealthMonitSup_SignalQualityFlagLevel_Reader(timespec* /*timestamp*/, double* /*value*/){

}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// EXTRA PVDELEGATE IN/OUT FOR TESTING PURPOSES
///////////////////////////////////////////////////////////////////////////////////////////////////

void DeviceVectorDBL::read_I32_DelegateIn(timespec* timestamp, std::int32_t* value){
	*value=PVDelegate_value_I32;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::read_DBL_DelegateIn(timespec* timestamp, double* value){
	*value=PVDelegate_value_DBL;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::read_vectorI8_DelegateIn(timespec* timestamp, std::vector<std::int8_t>* value){
	*value=PVDelegate_vector_I8;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::read_vectorUI8_DelegateIn(timespec* timestamp, std::vector<std::uint8_t>* value){
	*value=PVDelegate_vector_UI8;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::read_vectorI32_DelegateIn(timespec* timestamp, std::vector<std::int32_t>* value){
	*value=PVDelegate_vector_I32;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::read_vectorDBL_DelegateIn(timespec* timestamp, std::vector<double>* value){
	*value=PVDelegate_vector_DBL;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::read_string_DelegateIn(timespec* timestamp, std::string* value){
	*value=PVDelegate_value_string;
	*timestamp=timestamp_device;
}

void DeviceVectorDBL::write_I32_DelegateOut(const timespec& timestamp, const std::int32_t& value){
	PVDelegate_value_I32=value;
	timestamp_device=timestamp;
}
void DeviceVectorDBL::write_DBL_DelegateOut(const timespec& timestamp,const double& value){
	PVDelegate_value_DBL=value;
	timestamp_device=timestamp;
}
void DeviceVectorDBL::write_vectorI8_DelegateOut(const timespec& timestamp,const std::vector<std::int8_t>& value){
	PVDelegate_vector_I8=value;
	timestamp_device=timestamp;
}
void DeviceVectorDBL::write_vectorUI8_DelegateOut(const timespec& timestamp,const std::vector<std::uint8_t>& value){
	PVDelegate_vector_UI8=value;
	timestamp_device=timestamp;
}
void DeviceVectorDBL::write_vectorI32_DelegateOut(const timespec& timestamp,const std::vector<std::int32_t>& value){
	PVDelegate_vector_I32=value;
	timestamp_device=timestamp;
}
void DeviceVectorDBL::write_vectorDBL_DelegateOut(const timespec& timestamp,const std::vector<double>& value){
	PVDelegate_vector_DBL=value;
	timestamp_device=timestamp;
}
void DeviceVectorDBL::write_string_DelegateOut(const timespec& timestamp,const std::string& value){
	PVDelegate_value_string=value;
	timestamp_device=timestamp;
}

void DeviceVectorDBL::init_I32_DelegateOut(timespec* timestamp,  std::int32_t* value){
	*value=PVDelegate_value_I32;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::init_DBL_DelegateOut(timespec* timestamp, double* value){
	*value=PVDelegate_value_DBL;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::init_vectorI8_DelegateOut(timespec* timestamp, std::vector<std::int8_t>* value){
	*value=PVDelegate_vector_I8;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::init_vectorUI8_DelegateOut(timespec* timestamp, std::vector<std::uint8_t>* value){
	*value=PVDelegate_vector_UI8;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::init_vectorI32_DelegateOut(timespec* timestamp, std::vector<std::int32_t>* value){
	*value=PVDelegate_vector_I32;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::init_vectorDBL_DelegateOut(timespec* timestamp, std::vector<double>* value){
	*value=PVDelegate_vector_DBL;
	*timestamp=timestamp_device;
}
void DeviceVectorDBL::init_string_DelegateOut(timespec* timestamp, std::string* value){
	*value=PVDelegate_value_string;
	*timestamp=timestamp_device;
}


void DeviceVectorDBL::readDelegate(timespec* pTimestamp, std::string* pValue)
{
    *pTimestamp = timestamp_device;
    *pValue = m_writtenByDelegate;
}

void DeviceVectorDBL::writeDelegate(const timespec& timestamp, const std::string& value)
{
	timestamp_device = timestamp;
    m_writtenByDelegate = value;
}

void DeviceVectorDBL::writeTestVariableIn(const timespec& timestamp, const std::string& value)
{
    m_testVariableIn.setValue(timestamp, value);
}

void DeviceVectorDBL::pushTestVariableIn(const timespec& timestamp, const std::string& value)
{
    m_testVariableIn.push(timestamp, value);
}

void DeviceVectorDBL::readTestVariableOut(timespec* pTimestamp, std::string* pValue)
{
    m_testVariableOut.getValue(pTimestamp, pValue);
}

timespec DeviceVectorDBL::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}

