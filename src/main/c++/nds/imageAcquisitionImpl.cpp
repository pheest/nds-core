/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/definitions.h"
#include "nds3/impl/imageAcquisitionImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
imageAcquisitionImpl<T>::imageAcquisitionImpl(  const std::string& name,
												size_t maxElements,
												stateChange_t switchOnFunction,
												stateChange_t switchOffFunction,
												stateChange_t startFunction,
												stateChange_t stopFunction,
												stateChange_t recoverFunction,
												allowChange_t allowStateChangeFunction,
												readerInt32_t PV_MaxSizeX_Reader,
												readerInt32_t PV_MaxSizeY_Reader,
												writerInt32_t PV_BinX_Writer,
												readerInt32_t PV_BinX_Reader,
												writerInt32_t PV_BinY_Writer,
												readerInt32_t PV_BinY_Reader,
												writerInt32_t PV_MinX_Writer,
												readerInt32_t PV_MinX_Reader,
												writerInt32_t PV_MinY_Writer,
												readerInt32_t PV_MinY_Reader,
												writerInt32_t PV_SizeX_Writer,
												readerInt32_t PV_SizeX_Reader,
												writerInt32_t PV_SizeY_Writer,
												readerInt32_t PV_SizeY_Reader,
												writerInt32_t PV_ReverseX_Writer,
												readerInt32_t PV_ReverseX_Reader,
												writerInt32_t PV_ReverseY_Writer,
												readerInt32_t PV_ReverseY_Reader,
												writerInt32_t PV_Resolution_Writer,
												readerInt32_t PV_Resolution_Reader,
												writerInt32_t PV_SamplesPerPixel_Writer,
												readerInt32_t PV_SamplesPerPixel_Reader,
												writerDouble_t PV_AcquireTime_Writer,
												readerDouble_t PV_AcquireTime_Reader,
												writerDouble_t PV_AcquirePeriod_Writer,
												readerDouble_t PV_AcquirePeriod_Reader,
												readerDouble_t PV_TimeRemaining_Reader,
												writerDouble_t PV_Gain_Writer,
												readerDouble_t PV_Gain_Reader,
												writerInt32_t PV_FrameType_Writer,
												readerInt32_t PV_FrameType_Reader,
												writerInt32_t PV_LostFrames_Writer,
												readerInt32_t PV_LostFrames_Reader,
												writerInt32_t PV_ImageMode_Writer,
												readerInt32_t PV_ImageMode_Reader,
												writerInt32_t PV_TriggerMode_Writer,
												readerInt32_t PV_TriggerMode_Reader,
												writerInt32_t PV_NumExposures_Writer,
												readerInt32_t PV_NumExposures_Reader,
												readerInt32_t PV_NumExposuresCounter_Reader,
												writerInt32_t PV_Exposure_Writer,
												readerInt32_t PV_Exposure_Reader,
												writerInt32_t PV_minExposure_Writer,
												readerInt32_t PV_minExposure_Reader,
												writerInt32_t PV_maxExposure_Writer,
												readerInt32_t PV_maxExposure_Reader,
												writerInt32_t PV_ExposureStep_Writer,
												readerInt32_t PV_ExposureStep_Reader,
												writerInt32_t PV_BlackLevel_Writer,
												readerInt32_t PV_BlackLevel_Reader,
												writerInt32_t PV_NumImages_Writer,
												readerInt32_t PV_NumImages_Reader,
												readerInt32_t PV_NumImagesCounter_Reader,
												writerInt32_t PV_Acquire_Writer,
												readerInt32_t PV_Acquire_Reader,
												readerInt32_t PV_DetectorState_Reader,
												readerString_t PV_StatusMessage_Reader,
												readerString_t PV_StringToServer_Reader,
												readerString_t PV_StringFromServer_Reader,
												writerInt32_t PV_ReadStatus_Writer,
												writerInt32_t PV_ShutterMode_Writer,
												readerInt32_t PV_ShutterMode_Reader,
												writerInt32_t PV_ShutterControlMode_Writer,
												readerInt32_t PV_ShutterControlMode_Reader,
												readerInt32_t PV_ShutterStatus_Reader,
												writerInt32_t PV_DelayStep_Writer,
												readerInt32_t PV_DelayStep_Reader,
												writerDouble_t PV_ShutterOpenDelay_Writer,
												readerDouble_t PV_ShutterOpenDelay_Reader,
												writerDouble_t PV_ShutterMinOpenDelay_Writer,
												readerDouble_t PV_ShutterMinOpenDelay_Reader,
												writerDouble_t PV_ShutterMaxOpenDelay_Writer,
												readerDouble_t PV_ShutterMaxOpenDelay_Reader,
												writerDouble_t PV_ShutterCloseDelay_Writer,
												readerDouble_t PV_ShutterCloseDelay_Reader,
												writerDouble_t PV_ShutterMinCloseDelay_Writer,
												readerDouble_t PV_ShutterMinCloseDelay_Reader,
												writerDouble_t PV_ShutterMaxCloseDelay_Writer,
												readerDouble_t PV_ShutterMaxCloseDelay_Reader,
												writerVectorInt32_t PV_HotPixels_Writer,
												readerVectorInt32_t PV_HotPixels_Reader,
												writerVectorInt32_t PV_HotPixelsCorr_Writer,
												readerVectorInt32_t PV_HotPixelsCorr_Reader,
												writerDouble_t PV_Temperature_Writer,
												readerDouble_t PV_Temperature_Reader,
												readerDouble_t PV_ActualTemperature_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{

	// Add the children PVs
    m_image_PV.reset(new PVVariableInImpl<T>("image"));
    m_image_PV->setMaxElements(maxElements);
    m_image_PV->setDescription("Image");
    m_image_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_image_PV);

    m_imageSourceType_PV.reset(new PVVariableInImpl<std::string>("imageSourceType"));
    m_imageSourceType_PV->setDescription("Image Source Type");
    m_imageSourceType_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_imageSourceType_PV);


    m_imageSource_PV.reset(new PVVariableInImpl<std::string>("imageSource"));
    m_imageSource_PV->setDescription("Image Source");
    m_imageSource_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_imageSource_PV);


	// Add the children PVs
	m_MaxSizeX_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("MaxSizeX_RBV", PV_MaxSizeX_Reader));
	m_MaxSizeX_RBVPV->setDescription("Max size X ReadBack");
	m_MaxSizeX_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_MaxSizeX_RBVPV);

	m_MaxSizeY_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("MaxSizeY_RBV", PV_MaxSizeY_Reader));
	m_MaxSizeY_RBVPV->setDescription("Max size Y ReadBack");
	m_MaxSizeY_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_MaxSizeY_RBVPV);

//	###################################################################
//	# These records control the detector readout #
//	# including binning, region start and size #
//	###################################################################

	m_BinX_PV.reset(new PVDelegateOutImpl<std::int32_t>("BinX", PV_BinX_Writer));
	m_BinX_PV->setDescription("Size X");
	addChild(m_BinX_PV);

	m_BinX_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("BinX_RBV",PV_BinX_Reader));
	m_BinX_RBVPV->setDescription("Size X ReadBack");
	m_BinX_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_BinX_RBVPV);

	m_BinY_PV.reset(new PVDelegateOutImpl<std::int32_t>("Bin_Y",PV_BinY_Writer));
	m_BinY_PV->setDescription("Size Y");
	addChild(m_BinY_PV);

	m_BinY_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("BinY_RBV",PV_BinY_Reader));
	m_BinY_RBVPV->setDescription("Size Y ReadBack");
	m_BinY_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_BinY_RBVPV);

	m_MinX_PV.reset(new PVDelegateOutImpl<std::int32_t>("MinX",PV_MinX_Writer));
	m_MinX_PV->setDescription("Min Value of X");
	addChild(m_MinX_PV);

	m_MinX_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("MinX_RBV",PV_MinX_Reader));
	m_MinX_RBVPV->setDescription("Min Value of X ReadBack");
	m_MinX_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_MinX_RBVPV);

	m_MinY_PV.reset(new PVDelegateOutImpl<std::int32_t>("MinY",PV_MinY_Writer));
	m_MinY_PV->setDescription("Min Value of Y");
	addChild(m_MinY_PV);

	m_MinY_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("MinY_RBV",PV_MinY_Reader));
	m_MinY_RBVPV->setDescription("Min Value of Y ReadBack");
	m_MinY_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_MinY_RBVPV);

	m_SizeX_PV.reset(new PVDelegateOutImpl<std::int32_t>("SizeX",PV_SizeX_Writer));
	m_SizeX_PV->setDescription("Size of X");
	addChild(m_SizeX_PV);

	m_SizeX_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("SizeX_RBV",PV_SizeX_Reader));
	m_SizeX_RBVPV->setDescription("Size of X ReadBack");
	m_SizeX_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SizeX_RBVPV);

	m_SizeY_PV.reset(new PVDelegateOutImpl<std::int32_t>("SizeY",PV_SizeY_Writer));
	m_SizeY_PV->setDescription("Size of Y");
	addChild(m_SizeY_PV);

	m_SizeY_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("SizeY_RBV",PV_SizeY_Reader));
	m_SizeY_RBVPV->setDescription("Size of Y ReadBack");
	m_SizeY_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SizeY_RBVPV);

    //add enumeration for Signal Reference type
    enumerationStrings_t ReverseModeEnumeratorStrings;
    ReverseModeEnumeratorStrings.push_back("No");
    ReverseModeEnumeratorStrings.push_back("Yes");

	m_ReverseX_PV.reset(new PVDelegateOutImpl<std::int32_t>("ReverseX",PV_ReverseX_Writer));
	m_ReverseX_PV->setDescription("Reverse X");
	m_ReverseX_PV->setEnumeration(ReverseModeEnumeratorStrings);
	addChild(m_ReverseX_PV);

	m_ReverseX_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("ReverseX_RBV",PV_ReverseX_Reader));
	m_ReverseX_RBVPV->setDescription("Reverse X ReadBack");
	m_ReverseX_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_ReverseX_RBVPV->setEnumeration(ReverseModeEnumeratorStrings);
	addChild(m_ReverseX_RBVPV);

	m_ReverseY_PV.reset(new PVDelegateOutImpl<std::int32_t>("ReverseY",PV_ReverseY_Writer));
	m_ReverseY_PV->setDescription("Reverse Y");
	m_ReverseY_PV->setEnumeration(ReverseModeEnumeratorStrings);
	addChild(m_ReverseY_PV);

	m_ReverseY_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("ReverseY_RBV",PV_ReverseY_Reader));
	m_ReverseY_RBVPV->setDescription("Reverse Y ReadBack");
	m_ReverseY_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_ReverseY_RBVPV->setEnumeration(ReverseModeEnumeratorStrings);
	addChild(m_ReverseY_RBVPV);

	m_Resolution_PV.reset(new PVDelegateOutImpl<std::int32_t>("Resolution",PV_Resolution_Writer));
	m_Resolution_PV->setDescription("Image Resolution");
	addChild(m_Resolution_PV);

	m_Resolution_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("Resolution_RBV",PV_Resolution_Reader));
	m_Resolution_RBVPV->setDescription("Image Resolution ReadBack");
	m_Resolution_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Resolution_RBVPV);

	m_SamplesPerPixel_PV.reset(new PVDelegateOutImpl<std::int32_t>("SamplesPerPixel",PV_SamplesPerPixel_Writer));
	m_SamplesPerPixel_PV->setDescription("Samples per Pixel");
	addChild(m_SamplesPerPixel_PV);



	m_SamplesPerPixel_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("SamplesPerPixel_RBV",PV_SamplesPerPixel_Reader));
	m_SamplesPerPixel_RBVPV->setDescription("Samples per Pixel ReadBack");
	m_SamplesPerPixel_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SamplesPerPixel_RBVPV);



//	###################################################################
//	# These records control the acquisition time and #
//	# period #
//	###################################################################

	m_AcquireTime_PV.reset(new PVDelegateOutImpl<double>("AcquireTime",PV_AcquireTime_Writer));
	m_AcquireTime_PV->setDescription("Acquire time");
	addChild(m_AcquireTime_PV);

	m_AcquireTime_RBVPV.reset(new PVDelegateInImpl<double>("AcquireTime_RBV",PV_AcquireTime_Reader));
	m_AcquireTime_RBVPV->setDescription("Acquire time ReadBack");
	m_AcquireTime_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_AcquireTime_RBVPV);

	m_AcquirePeriod_PV.reset(new PVDelegateOutImpl<double>("AcquirePeriod",PV_AcquirePeriod_Writer));
	m_AcquirePeriod_PV->setDescription("Acquire Period");
	addChild(m_AcquirePeriod_PV);

	m_AcquirePeriod_RBVPV.reset(new PVDelegateInImpl<double>("AcquirePeriod_RBV",PV_AcquirePeriod_Reader));
	m_AcquirePeriod_RBVPV->setDescription("Acquire Period ReadBack");
	m_AcquirePeriod_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_AcquirePeriod_RBVPV);

	m_Time_Remaining_RBVPV.reset(new PVDelegateInImpl<double>("Time_Remaining_RBV",PV_TimeRemaining_Reader));
	m_Time_Remaining_RBVPV->setDescription("Time Remaining ReadBack");
	m_Time_Remaining_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Time_Remaining_RBVPV);

//	###################################################################
//	# These records control the gain #
//	###################################################################

	m_Gain_PV.reset(new PVDelegateOutImpl<double>("Gain",PV_Gain_Writer));
	m_Gain_PV->setDescription("Gain");
	addChild(m_Gain_PV);

	m_Gain_RBVPV.reset(new PVDelegateInImpl<double>("Gain_RBV",PV_Gain_Reader));
	m_Gain_RBVPV->setDescription("Gain ReadBack");
	m_Gain_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Gain_RBVPV);

//	###################################################################
//	# These records control the frame type #
//	###################################################################

    enumerationStrings_t FrameTypeEnumeratorStrings;
    FrameTypeEnumeratorStrings.push_back("Normal");
    FrameTypeEnumeratorStrings.push_back("Background");
    FrameTypeEnumeratorStrings.push_back("Flatfield");
    FrameTypeEnumeratorStrings.push_back("DblCorrelation");

	m_FrameType_PV.reset(new PVDelegateOutImpl<std::int32_t>("FrameType",PV_FrameType_Writer));
	m_FrameType_PV->setDescription("Frame Type: Normal, Background, Flatfield, BdlCorrelation");
	m_FrameType_PV->setEnumeration(FrameTypeEnumeratorStrings);
	addChild(m_FrameType_PV);

	m_FrameType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("FrameType_RBV",PV_FrameType_Reader));
	m_FrameType_RBVPV->setDescription("FrameType ReadBack:Normal, Background, Flatfield, BdlCorrelation");
	m_FrameType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_FrameType_RBVPV->setEnumeration(FrameTypeEnumeratorStrings);
	addChild(m_FrameType_RBVPV);

	m_LostFrames_PV.reset(new PVDelegateOutImpl<std::int32_t>("LostFrames",PV_LostFrames_Writer));
	m_LostFrames_PV->setDescription("Number of Lost Frames");
	addChild(m_LostFrames_PV);

	m_LostFrames_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("LostFrames_RBV",PV_LostFrames_Reader));
	m_LostFrames_RBVPV->setDescription("Number of Lost Frames ReadBack");
	m_LostFrames_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_LostFrames_RBVPV);

//	###################################################################
//	# These records control the acquisition mode #
//	###################################################################

    enumerationStrings_t ImageModeEnumeratorStrings;
    ImageModeEnumeratorStrings.push_back("Single");
    ImageModeEnumeratorStrings.push_back("Multiple");
    ImageModeEnumeratorStrings.push_back("Continuous");

	m_ImageMode_PV.reset(new PVDelegateOutImpl<std::int32_t>("ImageMode",PV_ImageMode_Writer));
	m_ImageMode_PV->setDescription("Image Mode Type: Single, Multiple, Continuous");
	m_ImageMode_PV->setEnumeration(ImageModeEnumeratorStrings);
	addChild(m_ImageMode_PV);

	m_ImageMode_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("ImageMode_RBV",PV_ImageMode_Reader));
	m_ImageMode_RBVPV->setDescription("ImageMode ReadBack:Single, Multiple, Continuous");
	m_ImageMode_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_ImageMode_RBVPV->setEnumeration(ImageModeEnumeratorStrings);
	addChild(m_ImageMode_RBVPV);

//	###################################################################
//	# These records control the trigger mode #
//	###################################################################

    enumerationStrings_t TriggerModeEnumeratorStrings;
    TriggerModeEnumeratorStrings.push_back("Internal");
    TriggerModeEnumeratorStrings.push_back("External");

	m_TriggerMode_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggerMode",PV_TriggerMode_Writer));
	m_TriggerMode_PV->setDescription("TriggerMode Mode Type: Internal, External");
	m_TriggerMode_PV->setEnumeration(TriggerModeEnumeratorStrings);
	addChild(m_TriggerMode_PV);

	m_TriggerMode_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggerMode_RBV",PV_TriggerMode_Reader));
	m_TriggerMode_RBVPV->setDescription("TriggerMode ReadBack: Internal, External");
	m_TriggerMode_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_TriggerMode_RBVPV->setEnumeration(TriggerModeEnumeratorStrings);
	addChild(m_TriggerMode_RBVPV);

//	###################################################################
//	# These records control the number of exposures and #
//	# number of images #
//	###################################################################

	m_NumExposures_PV.reset(new PVDelegateOutImpl<std::int32_t>("NumExposures",PV_NumExposures_Writer));
	m_NumExposures_PV->setDescription("Number of Exposures");
	addChild(m_NumExposures_PV);

	m_NumExposures_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("NumExposures_RBV",PV_NumExposures_Reader));
	m_NumExposures_RBVPV->setDescription("Number of Exposures ReadBack");
	m_NumExposures_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumExposures_RBVPV);

	m_NumExposuresCounter_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("NumExposuresCounter_RBV",PV_NumExposuresCounter_Reader));
	m_NumExposuresCounter_RBVPV->setDescription("Number of Exposures Counter");
	m_NumExposuresCounter_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumExposuresCounter_RBVPV);

	m_Exposure_PV.reset(new PVDelegateOutImpl<std::int32_t>("Exposure",PV_Exposure_Writer));
	m_Exposure_PV->setDescription("Exposure Value");
	addChild(m_Exposure_PV);

	m_Exposure_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("Exposure_RBV",PV_Exposure_Reader));
	m_Exposure_RBVPV->setDescription("Exposure Value ReadBack");
	m_Exposure_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Exposure_RBVPV);

	m_minExposure_PV.reset(new PVDelegateOutImpl<std::int32_t>("minExposure",PV_minExposure_Writer));
	m_minExposure_PV->setDescription("Minimum Exposure Value");
	addChild(m_minExposure_PV);

	m_minExposure_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("minExposures_RBV",PV_minExposure_Reader));
	m_minExposure_RBVPV->setDescription("Minimum Exposure Value ReadBack");
	m_minExposure_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_minExposure_RBVPV);

	m_maxExposure_PV.reset(new PVDelegateOutImpl<std::int32_t>("maxExposure",PV_maxExposure_Writer));
	m_maxExposure_PV->setDescription("Maximum Exposure Value");
	addChild(m_maxExposure_PV);

	m_maxExposure_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("maxExposure_RBV",PV_maxExposure_Reader));
	m_maxExposure_RBVPV->setDescription("Maximum Exposure Value ReadBack");
	m_maxExposure_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_maxExposure_RBVPV);

	m_ExposureStep_PV.reset(new PVDelegateOutImpl<std::int32_t>("ExposureStep",PV_ExposureStep_Writer));
	m_ExposureStep_PV->setDescription("Exposure Step");
	addChild(m_ExposureStep_PV);

	m_ExposureStep_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("ExposureStep_RBV",PV_ExposureStep_Reader));
	m_ExposureStep_RBVPV->setDescription("Exposure Step Value ReadBack");
	m_ExposureStep_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ExposureStep_RBVPV);

	m_BlackLevel_PV.reset(new PVDelegateOutImpl<std::int32_t>("BlackLevel",PV_BlackLevel_Writer));
	m_BlackLevel_PV->setDescription("Black Level");
	addChild(m_BlackLevel_PV);

	m_BlackLevel_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("BlackLevel_RBV",PV_BlackLevel_Reader));
	m_BlackLevel_RBVPV->setDescription("Black Level ReadBack");
	m_BlackLevel_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_BlackLevel_RBVPV);

	m_NumImages_PV.reset(new PVDelegateOutImpl<std::int32_t>("NumImages",PV_NumImages_Writer));
	m_NumImages_PV->setDescription("Number of Images");
	addChild(m_NumImages_PV);

	m_NumImages_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("NumImages_RBV",PV_NumImages_Reader));
	m_NumImages_RBVPV->setDescription("Number of Images ReadBack");
	m_NumImages_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumImages_RBVPV);

	m_NumImagesCounter_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("NumImagesCounter_RBV",PV_NumImagesCounter_Reader));
	m_NumImagesCounter_RBVPV->setDescription("Number of Images Counter ReadBack");
	m_NumImagesCounter_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumImagesCounter_RBVPV);

//	###################################################################
//	# These records control acquisition start and #
//	# and stop #
//	###################################################################

    enumerationStrings_t AcquireModeEnumeratorStrings;
    AcquireModeEnumeratorStrings.push_back("Done");
    AcquireModeEnumeratorStrings.push_back("Acquire");

    enumerationStrings_t AcquiringModeEnumeratorStrings;
    AcquiringModeEnumeratorStrings.push_back("Done");
    AcquiringModeEnumeratorStrings.push_back("Acquiring");

	m_Acquire_PV.reset(new PVDelegateOutImpl<std::int32_t>("Acquire",PV_Acquire_Writer));
	m_Acquire_PV->setDescription("Acquire PV");
	m_Acquire_PV->setEnumeration(AcquireModeEnumeratorStrings);
	addChild(m_Acquire_PV);

	m_Acquire_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("Acquire_RBV",PV_Acquire_Reader));
	m_Acquire_RBVPV->setDescription("Acquire ReadBack PV");
	m_Acquire_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_Acquire_RBVPV->setEnumeration(AcquiringModeEnumeratorStrings);
	addChild(m_Acquire_RBVPV);

    enumerationStrings_t DetectorStateEnumeratorStrings;
    DetectorStateEnumeratorStrings.push_back("Idle");
    DetectorStateEnumeratorStrings.push_back("Acquire");
    DetectorStateEnumeratorStrings.push_back("Readout");
    DetectorStateEnumeratorStrings.push_back("Correct");
    DetectorStateEnumeratorStrings.push_back("Saving");
    DetectorStateEnumeratorStrings.push_back("Aborting");
    DetectorStateEnumeratorStrings.push_back("Error");
    DetectorStateEnumeratorStrings.push_back("Waiting");
    DetectorStateEnumeratorStrings.push_back("Initializing");
    DetectorStateEnumeratorStrings.push_back("Disconnected");
    DetectorStateEnumeratorStrings.push_back("Aborted");

	m_DetectorState_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("DetectorState",PV_DetectorState_Reader));
	m_DetectorState_RBVPV->setDescription("Detector State ReadBack");
	m_DetectorState_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_DetectorState_RBVPV->setEnumeration(DetectorStateEnumeratorStrings);
	addChild(m_DetectorState_RBVPV);


//	###################################################################
//	# These records provide status information #
//	###################################################################
//	# Status message.

	m_StatusMessage_RBVPV.reset(new PVDelegateInImpl<std::string>("StatusMessage",PV_StatusMessage_Reader));
	m_StatusMessage_RBVPV->setDescription("Status Message ReadBack");
	m_StatusMessage_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_StatusMessage_RBVPV);

	m_StringToServer_RBVPV.reset(new PVDelegateInImpl<std::string>("StringToServer",PV_StringToServer_Reader));
	m_StringToServer_RBVPV->setDescription("String To Server ReadBack");
	m_StringToServer_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_StringToServer_RBVPV);

	m_StringFromServer_RBVPV.reset(new PVDelegateInImpl<std::string>("StringFromServer",PV_StringFromServer_Reader));
	m_StringFromServer_RBVPV->setDescription("String From Server ReadBack");
	m_StringFromServer_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_StringFromServer_RBVPV);

//	################################################################@###
//	# This record can be used to force a read of the detector status #
//	####################################################################

	m_ReadStatus_PV.reset(new PVDelegateOutImpl<std::int32_t>("ReadStatus",PV_ReadStatus_Writer));
	m_ReadStatus_PV->setDescription("Force the Read of the detector status PV");
	addChild(m_ReadStatus_PV);

//	###################################################################
//	# These records control the shutter #
//	###################################################################

    enumerationStrings_t ShutterEnumeratorStrings;
    ShutterEnumeratorStrings.push_back("None");
    ShutterEnumeratorStrings.push_back("EPICS_PV");
    ShutterEnumeratorStrings.push_back("Detector_Output");

  	m_ShutterMode_PV.reset(new PVDelegateOutImpl<std::int32_t>("ShutterMode",PV_ShutterMode_Writer));
	m_ShutterMode_PV->setDescription("Shutter Mode PV");
	m_ShutterMode_PV->setEnumeration(ShutterEnumeratorStrings);
	addChild(m_ShutterMode_PV);

	m_ShutterMode_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("ShutterMode_RBV",PV_ShutterMode_Reader));
	m_ShutterMode_RBVPV->setDescription("Shutter Mode PV ReadBack");
	m_ShutterMode_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_ShutterMode_RBVPV->setEnumeration(ShutterEnumeratorStrings);
	addChild(m_ShutterMode_RBVPV);

    enumerationStrings_t ShutterControlEnumeratorStrings;
    ShutterControlEnumeratorStrings.push_back("Close");
    ShutterControlEnumeratorStrings.push_back("Open");

  	m_ShutterControlMode_PV.reset(new PVDelegateOutImpl<std::int32_t>("ShutterControlMode",PV_ShutterControlMode_Writer));
	m_ShutterControlMode_PV->setDescription("Shutter Control Mode PV");
	m_ShutterControlMode_PV->setEnumeration(ShutterControlEnumeratorStrings);
	addChild(m_ShutterControlMode_PV);

	m_ShutterControlMode_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("ShutterControlMode_RBV",PV_ShutterControlMode_Reader));
	m_ShutterControlMode_RBVPV->setDescription("DShutter Mode PV ReadBack");
	m_ShutterControlMode_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_ShutterControlMode_RBVPV->setEnumeration(ShutterControlEnumeratorStrings);
	addChild(m_ShutterControlMode_RBVPV);

    enumerationStrings_t ShutterStatusEnumeratorStrings;
    ShutterStatusEnumeratorStrings.push_back("Closed");
    ShutterStatusEnumeratorStrings.push_back("Open");

	m_ShutterStatus_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("ShutterStatus_RBV",PV_ShutterStatus_Reader));
	m_ShutterStatus_RBVPV->setDescription("Shutter Mode PV ReadBack");
	m_ShutterStatus_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_ShutterStatus_RBVPV->setEnumeration(ShutterStatusEnumeratorStrings);
	addChild(m_ShutterStatus_RBVPV);

	m_DelayStep_PV.reset(new PVDelegateOutImpl<std::int32_t>("DelayStep",PV_DelayStep_Writer));
	m_DelayStep_PV->setDescription("Delay Step");
	addChild(m_DelayStep_PV);

	m_DelayStep_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("DelayStep_RBV",PV_DelayStep_Reader));
	m_DelayStep_RBVPV->setDescription("Delay Step Value ReadBack");
	m_DelayStep_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DelayStep_RBVPV);

  	m_ShutterOpenDelay_PV.reset(new PVDelegateOutImpl<double>("ShutterOpenDelay",PV_ShutterOpenDelay_Writer));
	m_ShutterOpenDelay_PV->setDescription("Shutter Open Delay PV");
	addChild(m_ShutterOpenDelay_PV);

	m_ShutterOpenDelay_RBVPV.reset(new PVDelegateInImpl<double>("ShutterOpenDelay_RBV",PV_ShutterOpenDelay_Reader));
	m_ShutterOpenDelay_RBVPV->setDescription("Shutter Open Delay PV ReadBack");
	m_ShutterOpenDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ShutterOpenDelay_RBVPV);

  	m_ShutterMinOpenDelay_PV.reset(new PVDelegateOutImpl<double>("ShutterMinOpenDelay",PV_ShutterMinOpenDelay_Writer));
	m_ShutterMinOpenDelay_PV->setDescription("Shutter Min Open Delay PV");
	addChild(m_ShutterMinOpenDelay_PV);

	m_ShutterMinOpenDelay_RBVPV.reset(new PVDelegateInImpl<double>("ShutterMinOpenDelay_RBV",PV_ShutterMinOpenDelay_Reader));
	m_ShutterMinOpenDelay_RBVPV->setDescription("Shutter Min Open Delay PV ReadBack");
	m_ShutterMinOpenDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ShutterMinOpenDelay_RBVPV);

  	m_ShutterMaxOpenDelay_PV.reset(new PVDelegateOutImpl<double>("ShutterMaxOpenDelay",PV_ShutterMaxOpenDelay_Writer));
	m_ShutterMaxOpenDelay_PV->setDescription("Shutter Max Open Delay PV");
	addChild(m_ShutterMaxOpenDelay_PV);

	m_ShutterMaxOpenDelay_RBVPV.reset(new PVDelegateInImpl<double>("ShutterMaxOpenDelay_RBV",PV_ShutterMaxOpenDelay_Reader));
	m_ShutterMaxOpenDelay_RBVPV->setDescription("Shutter Max Open Delay PV ReadBack");
	m_ShutterMaxOpenDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ShutterMaxOpenDelay_RBVPV);


	m_ShutterCloseDelay_PV.reset(new PVDelegateOutImpl<double>("ShutterCloseDelay",PV_ShutterCloseDelay_Writer));
	m_ShutterCloseDelay_PV->setDescription("Shutter Close Delay PV");
	addChild(m_ShutterCloseDelay_PV);

	m_ShutterCloseDelay_RBVPV.reset(new PVDelegateInImpl<double>("ShutterCloseDelay_RBV",PV_ShutterCloseDelay_Reader));
	m_ShutterCloseDelay_RBVPV->setDescription("Shutter Open Delay PV ReadBack");
	m_ShutterCloseDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ShutterCloseDelay_RBVPV);

  	m_ShutterMinCloseDelay_PV.reset(new PVDelegateOutImpl<double>("ShutterMinCloseDelay",PV_ShutterMinCloseDelay_Writer));
	m_ShutterMinCloseDelay_PV->setDescription("Shutter Min Open Delay PV");
	addChild(m_ShutterMinCloseDelay_PV);

	m_ShutterMinCloseDelay_RBVPV.reset(new PVDelegateInImpl<double>("ShutterMinCloseDelay_RBV",PV_ShutterMinCloseDelay_Reader));
	m_ShutterMinCloseDelay_RBVPV->setDescription("Shutter Min Close Delay PV ReadBack");
	m_ShutterMinCloseDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ShutterMinCloseDelay_RBVPV);

  	m_ShutterMaxCloseDelay_PV.reset(new PVDelegateOutImpl<double>("ShutterMaxCloseDelay",PV_ShutterMaxCloseDelay_Writer));
	m_ShutterMaxCloseDelay_PV->setDescription("Shutter Max Close Delay PV");
	addChild(m_ShutterMaxCloseDelay_PV);

	m_ShutterMaxCloseDelay_RBVPV.reset(new PVDelegateInImpl<double>("ShutterMaxCloseDelay_RBV",PV_ShutterMaxCloseDelay_Reader));
	m_ShutterMaxCloseDelay_RBVPV->setDescription("Shutter Max Close Delay PV ReadBack");
	m_ShutterMaxCloseDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ShutterMaxCloseDelay_RBVPV);

    m_HotPixels_PV.reset(new PVDelegateOutImpl<std::vector<std::int32_t>>("HotPixels",PV_HotPixels_Writer));
    m_HotPixels_PV->setDescription("HotPixels");
    addChild(m_HotPixels_PV);

    m_HotPixels_RBVPV.reset(new PVDelegateInImpl<std::vector<std::int32_t>>("HotPixels_RBV",PV_HotPixels_Reader));
	m_HotPixels_RBVPV->setDescription("HotPixels ReadBack");
	m_HotPixels_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_HotPixels_RBVPV);

    m_HotPixelsCorr_PV.reset(new PVDelegateOutImpl<std::vector<std::int32_t>>("HotPixelsCorr",PV_HotPixelsCorr_Writer));
    m_HotPixelsCorr_PV->setDescription("HotPixels Correction");
    addChild(m_HotPixelsCorr_PV);

    m_HotPixelsCorr_RBVPV.reset(new PVDelegateInImpl<std::vector<std::int32_t>>("HotPixelsCorr_RBV",PV_HotPixelsCorr_Reader));
	m_HotPixelsCorr_RBVPV->setDescription("HotPixels Correction ReadBack");
	m_HotPixelsCorr_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_HotPixelsCorr_RBVPV);

//	###################################################################
//	# These records control the detector temperature #
//	###################################################################

  	m_Temperature_PV.reset(new PVDelegateOutImpl<double>("Temperature",PV_Temperature_Writer));
	m_Temperature_PV->setDescription("Temperature");
	addChild(m_Temperature_PV);

  	m_Temperature_RBVPV.reset(new PVDelegateInImpl<double>("Temperature_RBV",PV_Temperature_Reader));
	m_Temperature_RBVPV->setDescription("Temperature Readback");
	m_Temperature_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Temperature_RBVPV);

  	m_ActualTemperature_RBVPV.reset(new PVDelegateInImpl<double>("ActualTemperature_RBV",PV_ActualTemperature_Reader));
	m_ActualTemperature_RBVPV->setDescription("Actual Temperature Readback");
	m_ActualTemperature_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ActualTemperature_RBVPV);


    m_decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_decimation_PV->setDescription("Decimation");
    m_decimation_PV->setScanType(scanType_t::passive, 0);
    m_decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_decimation_PV);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&imageAcquisitionImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);
}

template<typename T>
timespec imageAcquisitionImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void imageAcquisitionImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void imageAcquisitionImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_image_PV->push(timestamp, data);
}

template<typename T>
void imageAcquisitionImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_image_PV->setDecimation((std::uint32_t)m_decimation_PV->getValue());
    m_onStartDelegate();
}

template class imageAcquisitionImpl<std::int32_t>;
template class imageAcquisitionImpl<double>;
template class imageAcquisitionImpl<std::vector<std::int8_t> >;
template class imageAcquisitionImpl<std::vector<std::uint8_t> >;
template class imageAcquisitionImpl<std::vector<std::int32_t> >;
template class imageAcquisitionImpl<std::vector<double> >;
template class imageAcquisitionImpl<std::string >;


}
