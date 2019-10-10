
#include <nds3/nds.h>
#include <mutex>
#include <unistd.h>
#include <functional>

#include "Device.h"

static std::map<std::string, Device*> m_DevicesMap;
static std::mutex m_lockDevicesMap;

Device::Device(nds::Factory &factory, const std::string &DeviceName, const nds::namedParameters_t &parameters):
	m_name(DeviceName),

	PVVariable_value_I32(0),PVDelegate_value_I32(0),PVVariable_value_DBL(0),PVDelegate_value_DBL(0),PVVariable_vector_I8(2,0),PVDelegate_vector_I8(2,0),
	PVVariable_vector_UI8(2,0),	PVDelegate_vector_UI8(2,0),PVVariable_vector_I32(2,0),PVDelegate_vector_I32(2,0),PVVariable_vector_DBL(2,0),PVDelegate_vector_DBL(2,0),
	PVVariable_value_string{""},PVDelegate_value_string{""},

	timestamp_device{0,0},readtimeStamp{0,0},

	m_int32_DelegateIn("int32_DelegateIn",std::bind(&Device::read_I32_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_int32_DelegateOut("int32_DelegateOut",std::bind(&Device::write_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_int32_DelegateOut_init("int32_DelegateOut_init",std::bind(&Device::write_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&Device::init_I32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_double_DelegateIn("double_DelegateIn",std::bind(&Device::read_DBL_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_double_DelegateOut("double_DelegateOut",std::bind(&Device::write_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_double_DelegateOut_init("double_DelegateOut_init",std::bind(&Device::write_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&Device::init_DBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorI8_DelegateIn("vectorI8_DelegateIn",std::bind(&Device::read_vectorI8_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI8_DelegateOut("vectorI8_DelegateOut",std::bind(&Device::write_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI8_DelegateOut_init("vectorI8_DelegateOut_init",std::bind(&Device::write_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&Device::init_vectorI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorUI8_DelegateIn("vectorUI8_DelegateIn",std::bind(&Device::read_vectorUI8_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorUI8_DelegateOut("vectorUI8_DelegateOut",std::bind(&Device::write_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorUI8_DelegateOut_init("vectorUI8_DelegateOut_init",std::bind(&Device::write_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&Device::init_vectorUI8_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorI32_DelegateIn("vectorI32_DelegateIn",std::bind(&Device::read_vectorI32_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI32_DelegateOut("vectorI32_DelegateOut",std::bind(&Device::write_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorI32_DelegateOut_init("vectorI32_DelegateOut_init",std::bind(&Device::write_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&Device::init_vectorI32_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_vectorDBL_DelegateIn("vectorDBL_DelegateIn",std::bind(&Device::read_vectorDBL_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorDBL_DelegateOut("vectorDBL_DelegateOut",std::bind(&Device::write_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_vectorDBL_DelegateOut_init("vectorDBL_DelegateOut_init",std::bind(&Device::write_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&Device::init_vectorDBL_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_string_DelegateIn("string_DelegateIn",std::bind(&Device::read_string_DelegateIn,this, std::placeholders::_1, std::placeholders::_2)),
	m_string_DelegateOut("string_DelegateOut",std::bind(&Device::write_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),
	m_string_DelegateOut_init("string_DelegateOut_init",std::bind(&Device::write_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2),std::bind(&Device::init_string_DelegateOut,this, std::placeholders::_1, std::placeholders::_2)),

	m_delegateIn("delegateIn", std::bind(&Device::readDelegate, this, std::placeholders::_1, std::placeholders::_2)),
	m_delegateOut("delegateOut", std::bind(&Device::writeDelegate, this, std::placeholders::_1, std::placeholders::_2)),
	m_writeTestVariableIn("writeTestVariableIn", std::bind(&Device::writeTestVariableIn, this, std::placeholders::_1, std::placeholders::_2)),
	m_pushTestVariableIn("pushTestVariableIn", std::bind(&Device::pushTestVariableIn, this, std::placeholders::_1, std::placeholders::_2)),
	m_readTestVariableOut("readTestVariableOut", std::bind(&Device::readTestVariableOut, this, std::placeholders::_1, std::placeholders::_2))
	{
	//TODO:Study this.
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

	// Add state machine
	m_Device_stateMachine = rootNode.addChild(nds::StateMachine(true,
			std::bind(&Device::switchOn_Device, this),
			std::bind(&Device::switchOff_Device, this),
			std::bind(&Device::start_Device, this),
			std::bind(&Device::stop_Device, this),
			std::bind(&Device::recover_Device, this),
			std::bind(&Device::allow__Device_Change,this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));

	/**
	 * Add a DataProcessing node:
	 */
	m_DataProcessing = rootNode.addChild(nds::DataProcessing<std::vector<int32_t> >(
			"DataProcessingNode",
			128,
			std::bind(&Device::switchOn_DataProcessing, this),
			std::bind(&Device::switchOff_DataProcessing, this),
			std::bind(&Device::start_DataProcessing, this),
			std::bind(&Device::stop_DataProcessing, this),
			std::bind(&Device::recover_DataProcessing, this),
			std::bind(&Device::allow_DataProcessing_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
	));
	m_DataProcessing.setStartTimestampDelegate(std::bind(&Device::getCurrentTime,this));
	m_DataProcessing.getStartTimestamp();

	      /**
	      * Add a Streaming Config node:
	      */
	    m_Streaming = rootNode.addChild(nds::Streaming<std::vector<int32_t> >(
	     		"StreamingNode",
	 			128,
	 			std::bind(&Device::switchOn_Streaming, this),
	 			std::bind(&Device::switchOff_Streaming, this),
	 			std::bind(&Device::start_Streaming, this),
	 			std::bind(&Device::stop_Streaming, this),
	 			std::bind(&Device::recover_Streaming, this),
	 			std::bind(&Device::allow_Streaming_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
	 			std::bind(&Device::PV_Streaming_BufferSize_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&Device::PV_Streaming_Type_Writer,this, std::placeholders::_1, std::placeholders::_2),
	 			std::bind(&Device::PV_Streaming_DataFormat_Writer,this, std::placeholders::_1, std::placeholders::_2)
	     ));
	    m_Streaming.setStartTimestampDelegate(std::bind(&Device::getCurrentTime,this));
	    m_Streaming.getStartTimestamp();

	    /**
	     * Add a imageAcquisition node.
	     */
	    m_imageAcquisition = rootNode.addChild(nds::imageAcquisition<std::vector<double> >(
	    		"imageAcquisitionNode",
				128,
				std::bind(&Device::switchOn_imageAcquisition, this),
				std::bind(&Device::switchOff_imageAcquisition, this),
				std::bind(&Device::start_imageAcquisition, this),
				std::bind(&Device::stop_imageAcquisition, this),
				std::bind(&Device::recover_imageAcquisition, this),
				std::bind(&Device::allow_imageAcquisition_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&Device::PV_imageAcquisition_MaxSizeX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_MaxSizeY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_BinX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_BinX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_BinY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_BinY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_MinX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_MinX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_MinY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_MinY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_SizeX_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_SizeX_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_SizeY_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_SizeY_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ReverseX_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ReverseX_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ReverseY_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ReverseY_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Resolution_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Resolution_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_SamplesPerPixel_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_SamplesPerPixel_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_AcquireTime_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_AcquireTime_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_AcquirePeriod_Writer,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_AcquirePeriod_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_TimeRemaining_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Gain_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Gain_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_FrameType_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_FrameType_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_LostFrames_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_LostFrames_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ImageMode_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ImageMode_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_TriggerMode_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_TriggerMode_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_NumExposures_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_NumExposures_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_NumExposuresCounter_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Exposure_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Exposure_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_minExposure_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_minExposure_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_maxExposure_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_maxExposure_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ExposureStep_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ExposureStep_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_BlackLevel_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_BlackLevel_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_NumImages_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_NumImages_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_NumImagesCounter_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Acquire_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Acquire_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_DetectorState_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_StatusMessage_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_StringToServer_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_StringFromServer_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ReadStatus_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMode_Writer,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMode_Reader,this,   std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterControlMode_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterControlMode_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterStatus_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_DelayStep_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_DelayStep_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterOpenDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterOpenDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMinOpenDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMinOpenDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMaxOpenDelay_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMaxOpenDelay_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterCloseDelay_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterCloseDelay_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMinCloseDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMinCloseDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMaxCloseDelay_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ShutterMaxCloseDelay_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_HotPixels_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_HotPixels_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_HotPixelsCorr_Writer,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_HotPixelsCorr_Reader,this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Temperature_Writer,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_Temperature_Reader,this,  std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_imageAcquisition_ActualTemperature_Reader,this,  std::placeholders::_1, std::placeholders::_2)
	    ));
	    m_imageAcquisition.setStartTimestampDelegate(std::bind(&Device::getCurrentTime,this));
	    m_imageAcquisition.getStartTimestamp();

	    /**
	     * Add Firmware node
	     */
	    m_Firmware = rootNode.addChild(nds::Firmware("Firm",
	    		256, // Maximum string length.
				std::bind(&Device::switchOn_Firmware, this),
				std::bind(&Device::switchOff_Firmware, this),
				std::bind(&Device::start_Firmware, this),
				std::bind(&Device::stop_Firmware, this),
				std::bind(&Device::recover_Firmware, this),
				std::bind(&Device::allow_Firmware_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&Device::PV_Firmware_Path_Writer, this, std::placeholders::_1, std::placeholders::_2)));

	    m_Firmware.setTimestampDelegate(std::bind(&Device::getCurrentTime,this));
	    m_Firmware.setLogLevel(nds::logLevel_t::debug);

	    /**
	     * Add Decimation node
	     */
	    m_Decimation = rootNode.addChild(nds::Decimation<std::vector<double>>("DecimationNode",
	    		128,
				std::bind(&Device::switchOn_Decimation, this),
				std::bind(&Device::switchOff_Decimation, this),
				std::bind(&Device::start_Decimation, this),
				std::bind(&Device::stop_Decimation, this),
				std::bind(&Device::recover_Decimation, this),
				std::bind(&Device::allow_Decimation_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&Device::PV_Decimation_Enable_Writer, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_Decimation_Type_Writer, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_Decimation_Factor_Writer, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_Decimation_Offset_Writer, this, std::placeholders::_1, std::placeholders::_2)));

	    m_Decimation.setTimestampDelegate(std::bind(&Device::getCurrentTime,this));
	    m_Decimation.setLogLevel(nds::logLevel_t::debug);

	    /**
	     * Add FFT node
	     */
	    m_FFT = rootNode.addChild(nds::FFT<std::vector<double>>("FFTNode",
	    		128,
				std::bind(&Device::switchOn_FFT, this),
				std::bind(&Device::switchOff_FFT, this),
				std::bind(&Device::start_FFT, this),
				std::bind(&Device::stop_FFT, this),
				std::bind(&Device::recover_FFT, this),
				std::bind(&Device::allow_FFT_Change, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
				std::bind(&Device::PV_FFT_Enable_Writer, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_FFT_WindowType_Writer, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_FFT_FrameOverlap_Writer, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_FFT_FrameSize_Writer, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&Device::PV_FFT_SmoothFactor_Writer, this, std::placeholders::_1, std::placeholders::_2)));

	    m_FFT.setTimestampDelegate(std::bind(&Device::getCurrentTime,this));
	    m_FFT.setLogLevel(nds::logLevel_t::debug);


	    /**
	     * Test PV variables:
	     * 		- PVVariableIn: input variable to CS. The Device support sets its value using setValue, and the CS reads this value.
	     * 		- PVVariableOut: output variable from CS. The Device support uses getValue()to retrieve the PV's value,
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
	     * Test PV Delegates: input Delegate to CS. The Device support sets its value, and the CS reads this value
	     * The Device support uses getValue()to retrieve the PV's value and the control system use read() and write() to read and set the value.
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

	    // We have declared all the nodes and PVs in our Device: now we register them
	    //  with the control system that called this constructor.
	    ////////////////////////////////////////////////////////////////////////////////
	    rootNode.initialize(this, factory);

	    std::string rootNodeComponentName = rootNode.getComponentName();
	    std::string rootNodeFullExternalName = rootNode.getFullExternalName();
	    std::string rootNodeFullName = rootNode.getFullName();
	    std::string rootNodeFullNameFromPort = rootNode.getFullNameFromPort();
	    //timespec rootNodetime = rootNode.getTimestamp();
	    //    bool isLogLevelEnabled = rootNode.isLogLevelEnabled(nds::logLevel_t::debug);
	    //
	    //    std::cout<<"\trootNodeComponentName = " <<rootNodeComponentName<<std::endl;
	    //    std::cout<<"\trootNodeFullExternalName = " <<rootNodeFullExternalName<<std::endl;
	    //    std::cout<<"\trootNodeFullName = " <<rootNodeFullName<<std::endl;
	    //    std::cout<<"\trootNodeFullNameFromPort = " <<rootNodeFullNameFromPort<<std::endl;
	    //    std::cout<<"\tisLogLevelEnabled = " <<isLogLevelEnabled<<std::endl;
	    //    std::cout<<"\trootNodetime.tv_sec = " <<rootNodetime.tv_sec<<"\t;\trootNodetime.tv_nsec = " <<rootNodetime.tv_nsec<<std::endl;

	    rootNode.setTimestampDelegate(std::bind(&Device::getCurrentTime,this));
	    rootNode.setLogLevel(nds::logLevel_t::debug);

	    rootNode.getLogger(nds::logLevel_t::debug) << "This is the debugging logger:The device is created" << std::endl;
	    ndsDebugStream(rootNode) << "This is the ndsDebugStream: The device is created" << std::endl;

}


Device::~Device()
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);
    m_DevicesMap.erase(m_name);

}

Device* Device::getInstance(const std::string& DeviceName)
{
    std::lock_guard<std::mutex> lock(m_lockDevicesMap);

    std::map<std::string, Device*>::const_iterator findDevice = m_DevicesMap.find(DeviceName);
    if(findDevice == m_DevicesMap.end())
    {
        return 0;
    }
    return findDevice->second;
}

/*
 * Allocation function
 *********************/
void* Device::allocateDevice(nds::Factory& factory, const std::string& DeviceName, const nds::namedParameters_t& parameters)
{
    return new Device(factory, DeviceName, parameters);
}

/*
 * Deallocation function
 ***********************/
void Device::deallocateDevice(void* DeviceName)
{
    delete (Device*)DeviceName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  TEST Device STATE MACHINE
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control Device state machine
 */
void Device::switchOn_Device(){
	// Call HW initialization function here,
		//HW_CALL_INIT_FUNCTION

		// Call API HW to retrieve FirmwareVersion
		m_Firmware.setFirmwareVersion(getCurrentTime(),"Firmware test version");
		// Call API HW to retrieve FirmwareStatus
		m_Firmware.setFirmwareStatus(getCurrentTime(),0);
		// Call API HW to retrieve HardwareRevision
		m_Firmware.setHardwareRevision(getCurrentTime(),"Firmware test hardware revision");
		// Call API HW to retrieve SerialNumber
		m_Firmware.setSerialNumber(getCurrentTime(),"Firmware test serial number");
		// Call API HW to retrieve DeviceModel
		m_Firmware.setDeviceModel(getCurrentTime(),"Firmware test device model");
		// Call API HW to retrieve DeviceType
		m_Firmware.setDeviceType(getCurrentTime(),"Firmware test device type");
		// Call API HW to retrieve FirmwarePath
		m_Firmware.setFirmwarePath(getCurrentTime(),"Firmware path to be uploaded");

}
void Device::switchOff_Device(){

}
void Device::start_Device(){

}
void Device::stop_Device(){

}
void Device::recover_Device(){

}

bool Device::allow__Device_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  DATA PROCESSING
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* Methods to control DataProcessing state machine
*/
void Device::switchOn_DataProcessing(){

}
void Device::switchOff_DataProcessing(){

}
void Device::start_DataProcessing(){

}
void Device::stop_DataProcessing(){

}
void Device::recover_DataProcessing(){

}

bool Device::allow_DataProcessing_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//  STREAMING CONFIGURATION
///////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* Methods to control Streaming state machine
*/
void Device::switchOn_Streaming(){

}
void Device::switchOff_Streaming(){

}
void Device::start_Streaming(){

}
void Device::stop_Streaming(){

}
void Device::recover_Streaming(){

}

bool Device::allow_Streaming_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * Streaming setters
 */
void Device::PV_Streaming_BufferSize_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_Streaming_Type_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_Streaming_DataFormat_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//  IMAGE ACQUISITION
///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Methods to control imageAcquisition state machine
 */
void Device::switchOn_imageAcquisition(){

}
void Device::switchOff_imageAcquisition(){

}
void Device::start_imageAcquisition(){

}
void Device::stop_imageAcquisition(){

}
void Device::recover_imageAcquisition(){

}

bool Device::allow_imageAcquisition_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/**
 * imageAcquisition setters
 */
void Device::PV_imageAcquisition_BinX_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_BinY_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_MinX_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_MinY_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_SizeX_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_SizeY_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_ReverseX_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_ReverseY_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_Resolution_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_SamplesPerPixel_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_AcquireTime_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_AcquirePeriod_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_Gain_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_FrameType_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_LostFrames_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_ImageMode_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_TriggerMode_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_NumExposures_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_Exposure_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_minExposure_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_maxExposure_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_ExposureStep_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_BlackLevel_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_NumImages_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_Acquire_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_ReadStatus_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_ShutterMode_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_ShutterControlMode_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_DelayStep_Writer(const timespec& /*timestamp*/, const int32_t& /*value*/){

}
void Device::PV_imageAcquisition_ShutterOpenDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_ShutterMinOpenDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_ShutterMaxOpenDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_ShutterCloseDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_ShutterMinCloseDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_ShutterMaxCloseDelay_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}
void Device::PV_imageAcquisition_HotPixels_Writer(const timespec& /*timestamp*/, const std::vector<std::int32_t>& /*value*/){

}
void Device::PV_imageAcquisition_HotPixelsCorr_Writer(const timespec& /*timestamp*/, const std::vector<std::int32_t>& /*value*/){

}
void Device::PV_imageAcquisition_Temperature_Writer(const timespec& /*timestamp*/, const double& /*value*/){

}

/**
 * imageAcquisition getters
 */
void Device::PV_imageAcquisition_MaxSizeX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_MaxSizeY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_BinX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_BinY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_MinX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_MinY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_SizeX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_SizeY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_ReverseX_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_ReverseY_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_Resolution_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_SamplesPerPixel_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_AcquireTime_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_AcquirePeriod_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_TimeRemaining_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_Gain_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_FrameType_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_LostFrames_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_ImageMode_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_TriggerMode_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_NumExposures_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_NumExposuresCounter_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_Exposure_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_minExposure_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_maxExposure_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_ExposureStep_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_BlackLevel_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_NumImages_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_NumImagesCounter_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_Acquire_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_DetectorState_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_StatusMessage_Reader(timespec* /*timestamp*/, std::string* /*value*/){

}
void Device::PV_imageAcquisition_StringToServer_Reader(timespec* /*timestamp*/, std::string* /*value*/){

}
void Device::PV_imageAcquisition_StringFromServer_Reader(timespec* /*timestamp*/, std::string* /*value*/){

}
void Device::PV_imageAcquisition_ShutterMode_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_ShutterControlMode_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_ShutterStatus_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_DelayStep_Reader(timespec* /*timestamp*/, int32_t* /*value*/){

}
void Device::PV_imageAcquisition_ShutterOpenDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_ShutterMinOpenDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_ShutterMaxOpenDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_ShutterCloseDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_ShutterMinCloseDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_ShutterMaxCloseDelay_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_HotPixels_Reader(timespec* /*timestamp*/, std::vector<std::int32_t>* /*value*/){

}
void Device::PV_imageAcquisition_HotPixelsCorr_Reader(timespec* /*timestamp*/, std::vector<std::int32_t>* /*value*/){

}
void Device::PV_imageAcquisition_Temperature_Reader(timespec* /*timestamp*/, double* /*value*/){

}
void Device::PV_imageAcquisition_ActualTemperature_Reader(timespec* /*timestamp*/, double* /*value*/){

}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// EXTRA PVDELEGATE IN/OUT FOR TESTING PURPOSES
///////////////////////////////////////////////////////////////////////////////////////////////////

void Device::read_I32_DelegateIn(timespec* timestamp, std::int32_t* value){
	*value=PVDelegate_value_I32;
	*timestamp=timestamp_device;
}
void Device::read_DBL_DelegateIn(timespec* timestamp, double* value){
	*value=PVDelegate_value_DBL;
	*timestamp=timestamp_device;
}
void Device::read_vectorI8_DelegateIn(timespec* timestamp, std::vector<std::int8_t>* value){
	*value=PVDelegate_vector_I8;
	*timestamp=timestamp_device;
}
void Device::read_vectorUI8_DelegateIn(timespec* timestamp, std::vector<std::uint8_t>* value){
	*value=PVDelegate_vector_UI8;
	*timestamp=timestamp_device;
}
void Device::read_vectorI32_DelegateIn(timespec* timestamp, std::vector<std::int32_t>* value){
	*value=PVDelegate_vector_I32;
	*timestamp=timestamp_device;
}
void Device::read_vectorDBL_DelegateIn(timespec* timestamp, std::vector<double>* value){
	*value=PVDelegate_vector_DBL;
	*timestamp=timestamp_device;
}
void Device::read_string_DelegateIn(timespec* timestamp, std::string* value){
	*value=PVDelegate_value_string;
	*timestamp=timestamp_device;
}

void Device::write_I32_DelegateOut(const timespec& timestamp, const std::int32_t& value){
	PVDelegate_value_I32=value;
	timestamp_device=timestamp;
}
void Device::write_DBL_DelegateOut(const timespec& timestamp,const double& value){
	PVDelegate_value_DBL=value;
	timestamp_device=timestamp;
}
void Device::write_vectorI8_DelegateOut(const timespec& timestamp,const std::vector<std::int8_t>& value){
	PVDelegate_vector_I8=value;
	timestamp_device=timestamp;
}
void Device::write_vectorUI8_DelegateOut(const timespec& timestamp,const std::vector<std::uint8_t>& value){
	PVDelegate_vector_UI8=value;
	timestamp_device=timestamp;
}
void Device::write_vectorI32_DelegateOut(const timespec& timestamp,const std::vector<std::int32_t>& value){
	PVDelegate_vector_I32=value;
	timestamp_device=timestamp;
}
void Device::write_vectorDBL_DelegateOut(const timespec& timestamp,const std::vector<double>& value){
	PVDelegate_vector_DBL=value;
	timestamp_device=timestamp;
}
void Device::write_string_DelegateOut(const timespec& timestamp,const std::string& value){
	PVDelegate_value_string=value;
	timestamp_device=timestamp;
}

void Device::init_I32_DelegateOut(timespec* timestamp,  std::int32_t* value){
	*value=PVDelegate_value_I32;
	*timestamp=timestamp_device;
}
void Device::init_DBL_DelegateOut(timespec* timestamp, double* value){
	*value=PVDelegate_value_DBL;
	*timestamp=timestamp_device;
}
void Device::init_vectorI8_DelegateOut(timespec* timestamp, std::vector<std::int8_t>* value){
	*value=PVDelegate_vector_I8;
	*timestamp=timestamp_device;
}
void Device::init_vectorUI8_DelegateOut(timespec* timestamp, std::vector<std::uint8_t>* value){
	*value=PVDelegate_vector_UI8;
	*timestamp=timestamp_device;
}
void Device::init_vectorI32_DelegateOut(timespec* timestamp, std::vector<std::int32_t>* value){
	*value=PVDelegate_vector_I32;
	*timestamp=timestamp_device;
}
void Device::init_vectorDBL_DelegateOut(timespec* timestamp, std::vector<double>* value){
	*value=PVDelegate_vector_DBL;
	*timestamp=timestamp_device;
}
void Device::init_string_DelegateOut(timespec* timestamp, std::string* value){
	*value=PVDelegate_value_string;
	*timestamp=timestamp_device;
}


void Device::readDelegate(timespec* pTimestamp, std::string* pValue)
{
    *pTimestamp = timestamp_device;
    *pValue = m_writtenByDelegate;
}

void Device::writeDelegate(const timespec& timestamp, const std::string& value)
{
	timestamp_device = timestamp;
    m_writtenByDelegate = value;
}

void Device::writeTestVariableIn(const timespec& timestamp, const std::string& value)
{
    m_testVariableIn.setValue(timestamp, value);
}

void Device::pushTestVariableIn(const timespec& timestamp, const std::string& value)
{
    m_testVariableIn.push(timestamp, value);
}

void Device::readTestVariableOut(timespec* pTimestamp, std::string* pValue)
{
    m_testVariableOut.getValue(pTimestamp, pValue);
}

timespec Device::getCurrentTime()
{
    timespec time;
    time.tv_sec = m_setCurrentTime.getValue();
    time.tv_nsec = time.tv_sec + 10;
    return time;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// FIRMWARE SUPPORT NODE*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* Firmware State Machine
*/

// Called when the Firmware node has to be switched on.
void Device::switchOn_Firmware(){

}

// Called when the Firmware node has to be switched off.
void Device::switchOff_Firmware(){

}

// Called when the Firmware node has to start acquiring. We start the Firmware thread.
void Device::start_Firmware(){

	m_bStop_Firmware = false; //< We will set to true to stop the Firmware thread
	/**
	 *   Start the Firmware thread.
	 *   We don't need to check if the thread was already started because the state
	 *   machine guarantees that the start handler is called only while the state
	 *   is ON.
	 */
	m_Firmware_Thread = std::thread(std::bind(&Device::Firmware_thread_body, this));
}

// Stop the DataAcquisition node thread
void Device::stop_Firmware(){
	m_bStop_Firmware = true;
	m_Firmware_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.
void Device::recover_Firmware(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.
bool Device::allow_Firmware_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/*
* Firmware support setters
*/
void Device::PV_Firmware_Path_Writer(const timespec& timestamp, const std::string& value){
	std::string firmwarePath;
	//firmwarePath has the firmware path to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real firmware path programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  firmwarePath are equal.
	firmwarePath=value;
	m_Firmware.setFirmwarePath(timestamp,firmwarePath);
}

void Device::Firmware_thread_body(){

	// Get FirmwareVersion
	std::string FirmwareVersion = m_Firmware.getFirmwareVersion();
	// Get FirmwareStatus
	std::int32_t FirmwareStatus = m_Firmware.getFirmwareStatus();
	// Get HardwareRevision
	std::string HardwareRevision = m_Firmware.getHardwareRevision();
	// Get SerialNumber
	std::string SerialNumber = m_Firmware.getSerialNumber();
	// Get DeviceModel
	std::string DeviceModel = m_Firmware.getDeviceModel();
	// Get DeviceType
	std::string DeviceType = m_Firmware.getDeviceType();
	// Get FirmwarePath
	std::string FirmwarePath = m_Firmware.getFirmwarePath();
	std::string FirmwarePathOld=m_Firmware.getFirmwarePath();


	std::cout<<"Firmware support information:"<<std::endl;
	std::cout<<"\tFirmwareVersion = "<<FirmwareVersion<<std::endl;
	std::cout<<"\tFirmwareStatus = "<<FirmwareStatus<<std::endl;
	std::cout<<"\tHardwareRevision = "<<HardwareRevision<<std::endl;
	std::cout<<"\tSerialNumber = "<<SerialNumber<<std::endl;
	std::cout<<"\tDeviceModel = "<<DeviceModel<<std::endl;
	std::cout<<"\tDeviceType = "<<DeviceType<<std::endl;
	std::cout<<"\tFirmwarePath = "<<FirmwarePath<<std::endl;

	// Run until the state machine stops us
	while(!m_bStop_Firmware){


		// Get FirmwarePath
		std::string FirmwarePath = m_Firmware.getFirmwarePath();
		if(FirmwarePath.compare(FirmwarePathOld)!=0){
			// Push the FirmwarePath to the control system
			m_Firmware.push(m_Firmware.getTimestamp(), FirmwarePath);
			FirmwarePathOld=FirmwarePath;
		}
		// Rest for a while
		::usleep(1000000);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// DECIMATION  NODE*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* Decimation State Machine
*/

// Called when the Decimation node has to be switched on.
void Device::switchOn_Decimation(){

}

// Called when the Decimation node has to be switched off.
void Device::switchOff_Decimation(){

}

// Called when the Decimation node has to start acquiring. We start the Decimation thread.
void Device::start_Decimation(){

	m_bStop_Decimation = false; //< We will set to true to stop the Decimation thread
	/**
	 *   Start the Decimation thread.
	 *   We don't need to check if the thread was already started because the state
	 *   machine guarantees that the start handler is called only while the state
	 *   is ON.
	 */
	m_Decimation_Thread = std::thread(std::bind(&Device::Decimation_thread_body, this));
}

// Stop the DataAcquisition node thread
void Device::stop_Decimation(){
	m_bStop_Decimation = true;
	m_Decimation_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.
void Device::recover_Decimation(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.
bool Device::allow_Decimation_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/*
* Decimation setters
*/
void Device::PV_Decimation_Enable_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t DecimationEnable;
	//DecimationEnable has the value to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real DecimationEnable programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  DecimationEnable are equal.
	DecimationEnable=value;
	m_Decimation.setDecimationEnable(timestamp,DecimationEnable);
}
void Device::PV_Decimation_Type_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t DecimationType;
	//DecimationType has the DecimationType to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real DecimationType programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  DecimationType are equal.
	DecimationType=value;
	m_Decimation.setDecimationType(timestamp,DecimationType);
}
void Device::PV_Decimation_Factor_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t DecimationFactor;
	//DecimationFactor has the DecimationFactor to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real DecimationFactor programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  DecimationFactor are equal.
	DecimationFactor=value;
	m_Decimation.setDecimationFactor(timestamp,DecimationFactor);
}
void Device::PV_Decimation_Offset_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t DecimationOffset;
	//DecimationOffset has the DecimationOffset to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real DecimationOffset programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  DecimationOffset are equal.
	DecimationOffset=value;
	m_Decimation.setDecimationOffset(timestamp,DecimationOffset);
}


void Device::Decimation_thread_body(){
	//TODO
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// FFT  NODE*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* FFT State Machine
*/

// Called when the FFT node has to be switched on.
void Device::switchOn_FFT(){

}

// Called when the FFT node has to be switched off.
void Device::switchOff_FFT(){

}

// Called when the FFT node has to start acquiring. We start the FFT thread.
void Device::start_FFT(){

	m_bStop_FFT = false; //< We will set to true to stop the FFT thread
	/**
	 *   Start the FFT thread.
	 *   We don't need to check if the thread was already started because the state
	 *   machine guarantees that the start handler is called only while the state
	 *   is ON.
	 */
	m_FFT_Thread = std::thread(std::bind(&Device::FFT_thread_body, this));
}

// Stop the DataAcquisition node thread
void Device::stop_FFT(){
	m_bStop_FFT = true;
	m_FFT_Thread.join();
}

// A failure during a state transition will cause the state machine to switch to the failure state. For now we don't plan for this and every time the
//  state machine wants to recover we throw StateMachineRollBack to force the state machine to stay on the failure state.
void Device::recover_FFT(){
    throw nds::StateMachineRollBack("Cannot recover"); //TODO: Study this
}

// We always allow the state machine to switch state. Before calling this function the state machine has already verified that the requested state transition is legal.
bool Device::allow_FFT_Change(const nds::state_t, const nds::state_t, const nds::state_t){
	return true;
}

/*
* FFT setters
*/
void Device::PV_FFT_Enable_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t FFTEnable;
	//FFTEnable has the value to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real FFTEnable programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  FFTEnable are equal.
	FFTEnable=value;
	m_FFT.setFFTEnable(timestamp,FFTEnable);
}
void Device::PV_FFT_WindowType_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t FFTWindowType;
	//FFTWindowType has the FFTWindowType to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real FFTWindowType programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  FFTWindowType are equal.
	FFTWindowType=value;
	m_FFT.setFFTWindowType(timestamp,FFTWindowType);
}
void Device::PV_FFT_FrameOverlap_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t FFTFrameOverlap;
	//FFTFrameOverlap has the FFTFrameOverlap to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real FFTFrameOverlap programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  FFTFrameOverlap_ are equal.
	FFTFrameOverlap=value;
	m_FFT.setFFTFrameOverlap(timestamp,FFTFrameOverlap);
}
void Device::PV_FFT_FrameSize_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t FFTFrameSize;
	//FFTFrameSize has the FFTFrameSize to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real FFTFrameSize programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  FFTFrameSize are equal.
	FFTFrameSize=value;
	m_FFT.setFFTFrameSize(timestamp,FFTFrameSize);
}
void Device::PV_FFT_SmoothFactor_Writer(const timespec& timestamp, const std::int32_t& value){
	std::int32_t FFTSmoothFactor;
	//FFTSmoothFactor has the FFTSmoothFactor to be programmed on the hardware.
	//Call to function programming the hardware. This function should return the real FFTSmoothFactor programmed. This value has to be set to the readback attribute.
	//In the meantime, without real hardware value and  FFTSmoothFactor are equal.
	FFTSmoothFactor=value;
	m_FFT.setFFTSmoothFactor(timestamp,FFTSmoothFactor);
}

void Device::FFT_thread_body(){
	//TODO
}
