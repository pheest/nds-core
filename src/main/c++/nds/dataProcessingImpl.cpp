/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 *
 */

#include "nds3/definitions.h"
#include "nds3/impl/dataProcessingImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
DataProcessingImpl<T>::DataProcessingImpl( const std::string& name,
										   size_t maxElements,
										   stateChange_t switchOnFunction,
										   stateChange_t switchOffFunction,
										   stateChange_t startFunction,
										   stateChange_t stopFunction,
										   stateChange_t recoverFunction,
										   allowChange_t allowStateChangeFunction,
										   writerInt32_t PV_EnableFilter_Writer,
										   readerInt32_t PV_EnableFilter_Reader,
										   writerInt32_t PV_FilterType_Writer,
										   readerInt32_t PV_FilterType_Reader,
										   writerVectorInt32_t PV_FilterParams_Writer,
										   readerVectorInt32_t PV_FilterParams_Reader,
										   size_t 		maxFFTElements,
										   writerInt32_t PV_EnableFFT_Writer,
										   readerInt32_t PV_EnableFFT_Reader,
										   writerInt32_t PV_EnableSwFFT_Writer,
										   readerInt32_t PV_EnableSwFFT_Reader,
										   writerInt32_t PV_FFTwindowType_Writer,
										   readerInt32_t PV_FFTwindowType_Reader,
										   writerInt32_t PV_FFTOverlap_Writer,
										   readerInt32_t PV_FFTOverlap_Reader,
										   writerInt32_t PV_FFTFrameSize_Writer,
										   readerInt32_t PV_FFTFrameSize_Reader,
										   writerInt32_t PV_FFTSmooth_Writer,
										   readerInt32_t PV_FFTSmooth_Reader,
										   writerInt32_t PV_EnableDecimation_Writer,
										   readerInt32_t PV_EnableDecimation_Reader,
										   writerInt32_t PV_DecimationType_Writer,
										   readerInt32_t PV_DecimationType_Reader,
										   writerInt32_t PV_DecimationOffset_Writer,
										   readerInt32_t PV_DecimationOffset_Reader,
										   writerInt32_t PV_RAW2Eng_Writer,
										   readerInt32_t PV_RAW2Eng_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_dataPV.reset(new PVVariableInImpl<T>("Data"));
    m_dataPV->setMaxElements(maxElements);
    m_dataPV->setDescription("Acquired data");
    m_dataPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_dataPV);

	//add enumeration
	enumerationStrings_t enableFiterEnumeratorStrings;
	enableFiterEnumeratorStrings.push_back("On");
	enableFiterEnumeratorStrings.push_back("Off");

	m_enableFilter_PV.reset(new PVDelegateOutImpl<std::int32_t>("enableFilter",PV_EnableFilter_Writer));
	m_enableFilter_PV->setDescription("Enable filter");
	m_enableFilter_PV->setEnumeration(enableFiterEnumeratorStrings);
	m_enableFilter_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_enableFilter_PV);

	m_enableFilter_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("enableFilter_RBV",PV_EnableFilter_Reader));
	m_enableFilter_RBVPV->setDescription("Enable filter ReadBack");
	m_enableFilter_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_enableFilter_RBVPV->setEnumeration(enableFiterEnumeratorStrings);
	addChild(m_enableFilter_RBVPV);

	enumerationStrings_t fiterTypeEnumeratorStrings;
	fiterTypeEnumeratorStrings.push_back("SW");
	fiterTypeEnumeratorStrings.push_back("Polynom");
	fiterTypeEnumeratorStrings.push_back("LPass");
	fiterTypeEnumeratorStrings.push_back("HPass");

	m_FilterType_PV.reset(new PVDelegateOutImpl<std::int32_t>("filterType",PV_FilterType_Writer));
	m_FilterType_PV->setDescription("Filter types: SW, Polynomial, LowPass, HighPass");
	m_FilterType_PV->setEnumeration(fiterTypeEnumeratorStrings);
	m_FilterType_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_FilterType_PV);


	m_FilterType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("filterType_RBV",PV_FilterType_Reader));
	m_FilterType_RBVPV->setDescription("Filter types: SW, Polynomial, LowPass, HighPass");
	m_FilterType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_FilterType_RBVPV->setEnumeration(fiterTypeEnumeratorStrings);
	addChild(m_FilterType_RBVPV);


	m_FilterParams_PV.reset(new PVDelegateOutImpl<std::vector<std::int32_t> >("filterParams",PV_FilterParams_Writer));
	m_FilterParams_PV->setDescription("Filter Parameters");
	m_FilterParams_PV->setEnumeration(fiterTypeEnumeratorStrings);
	m_FilterParams_PV->setMaxElements(100);//TODO TBD
	addChild(m_FilterParams_PV);

	m_FilterParams_RBVPV.reset(new PVDelegateInImpl<std::vector<std::int32_t> >("filterParams_RBV",PV_FilterParams_Reader));
	m_FilterParams_RBVPV->setDescription("Filter Parameters");
	m_FilterParams_RBVPV->setScanType(scanType_t::passive, 0);
	m_FilterParams_RBVPV->setEnumeration(fiterTypeEnumeratorStrings);
	addChild(m_FilterParams_RBVPV);

    m_FFTdataPV.reset(new PVVariableInImpl<T>("FFTData"));
    m_FFTdataPV->setMaxElements(maxFFTElements);
    m_FFTdataPV->setDescription("FFT data");
    m_FFTdataPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_FFTdataPV);

	enumerationStrings_t enableFFTEnumeratorStrings;
	enableFFTEnumeratorStrings.push_back("On");
	enableFFTEnumeratorStrings.push_back("Off");

	m_enableFFT_PV.reset(new PVDelegateOutImpl<std::int32_t>("enableFFT",PV_EnableFFT_Writer));
	m_enableFFT_PV->setDescription("Enable FFT");
	m_enableFFT_PV->setEnumeration(enableFFTEnumeratorStrings);
	m_enableFFT_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_enableFFT_PV);

	m_enableFFT_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("enableFFT_RBV",PV_EnableFFT_Reader));
	m_enableFFT_RBVPV->setDescription("Enable FFT ReadBack");
	m_enableFFT_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_enableFFT_RBVPV->setEnumeration(enableFFTEnumeratorStrings);
	addChild(m_enableFFT_RBVPV);

	enumerationStrings_t enableSwFFTEnumeratorStrings;
	enableSwFFTEnumeratorStrings.push_back("On");
	enableSwFFTEnumeratorStrings.push_back("Off");

	m_enableSwFFT_PV.reset(new PVDelegateOutImpl<std::int32_t>("enableSwFFT",PV_EnableSwFFT_Writer));
	m_enableSwFFT_PV->setDescription("Enable Software FFT");
	m_enableSwFFT_PV->setEnumeration(enableSwFFTEnumeratorStrings);
	m_enableSwFFT_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_enableSwFFT_PV);

	m_enableSwFFT_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("enableSwFFT_RBVPV",PV_EnableSwFFT_Reader));
	m_enableSwFFT_RBVPV->setDescription("Enable Software FFT ReadBack");
	m_enableSwFFT_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_enableSwFFT_RBVPV->setEnumeration(enableSwFFTEnumeratorStrings);
	addChild(m_enableSwFFT_RBVPV);

	enumerationStrings_t FFTwindowTypeEnumeratorStrings;
	FFTwindowTypeEnumeratorStrings.push_back("NONE");
	FFTwindowTypeEnumeratorStrings.push_back("Barlett");
	FFTwindowTypeEnumeratorStrings.push_back("Blackman");
	FFTwindowTypeEnumeratorStrings.push_back("FlatTop");
	FFTwindowTypeEnumeratorStrings.push_back("Hann");
	FFTwindowTypeEnumeratorStrings.push_back("Hamm");
	FFTwindowTypeEnumeratorStrings.push_back("Tukey");
	FFTwindowTypeEnumeratorStrings.push_back("Welch");

	m_FFTWindowType_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFTwindowType",PV_FFTwindowType_Writer));
	m_FFTWindowType_PV->setDescription("FFT Window Types: NONE, Barlett, Blackman, FlatTop, Hann, Hamm, Tukey, Welch");
	m_FFTWindowType_PV->setEnumeration(FFTwindowTypeEnumeratorStrings);
	m_FFTWindowType_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_FFTWindowType_PV);

	m_FFTWindowType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("FFTwindowType_RBV",PV_FFTwindowType_Reader));
	m_FFTWindowType_RBVPV->setDescription("FFT Window Types: NONE, Barlett, Blackman, FlatTop, Hann, Hamm, Tukey, Welch");
	m_FFTWindowType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_FFTWindowType_RBVPV->setEnumeration(FFTwindowTypeEnumeratorStrings);
	addChild(m_FFTWindowType_RBVPV);


	m_FFTFrameOverlap_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFToverlap",PV_FFTOverlap_Writer));
	m_FFTFrameOverlap_PV->setDescription("FFT Number of frames to overlap");
	m_FFTFrameOverlap_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_FFTFrameOverlap_PV);

	m_FFTFrameOverlap_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("FFToverlap_RBV",PV_FFTOverlap_Reader));
	m_FFTFrameOverlap_RBVPV->setDescription("FFT Number of frames to overlap ReadBack");
	m_FFTFrameOverlap_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_FFTFrameOverlap_RBVPV);

	m_FFTFrameSize_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFTframeSize",PV_FFTFrameSize_Writer));
	m_FFTFrameSize_PV->setDescription("FFT Size of the frame");
	m_FFTFrameSize_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_FFTFrameSize_PV);

	m_FFTFrameSize_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("FFTframeSize_RBV",PV_FFTFrameSize_Reader));
	m_FFTFrameSize_RBVPV->setDescription("FFT Size of the frame ReadBack");
	m_FFTFrameSize_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_FFTFrameSize_RBVPV);

	m_FFTSmoothFactor_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFTSmoothFactor",PV_FFTSmooth_Writer));
	m_FFTSmoothFactor_PV->setDescription("FFT Smooth Factor");
	m_FFTSmoothFactor_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_FFTSmoothFactor_PV);

	m_FFTSmoothFactor_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("FFTSmoothFactor_RBV",PV_FFTSmooth_Reader));
	m_FFTSmoothFactor_RBVPV->setDescription("FFT Smooth Factor ReadBack");
	m_FFTSmoothFactor_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_FFTSmoothFactor_RBVPV);

	enumerationStrings_t enableDecimationEnumeratorStrings;
	enableSwFFTEnumeratorStrings.push_back("On");
	enableSwFFTEnumeratorStrings.push_back("Off");

	m_enableDecimation_PV.reset(new PVDelegateOutImpl<std::int32_t>("enableDecimation",PV_EnableDecimation_Writer));
	m_enableDecimation_PV->setDescription("Enable Software FFT");
	m_enableDecimation_PV->setEnumeration(enableDecimationEnumeratorStrings);
	m_enableDecimation_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_enableDecimation_PV);

	m_enableDecimation_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("enableDecimation_RBV",PV_EnableDecimation_Reader));
	m_enableDecimation_RBVPV->setDescription("Enable Software FFT ReadBack");
	m_enableDecimation_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_enableDecimation_RBVPV->setEnumeration(enableDecimationEnumeratorStrings);
	addChild(m_enableDecimation_RBVPV);

	enumerationStrings_t decimationTypeEnumeratorStrings;
	decimationTypeEnumeratorStrings.push_back("Frames");
	decimationTypeEnumeratorStrings.push_back("Samples");

	m_enableDecimation_PV.reset(new PVDelegateOutImpl<std::int32_t>("DecimationType",PV_DecimationType_Writer));
	m_enableDecimation_PV->setDescription("Decimation type: Frames or Samples");
	m_enableDecimation_PV->setEnumeration(decimationTypeEnumeratorStrings);
	m_enableDecimation_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_enableDecimation_PV);

	m_enableDecimation_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("DecimationType_RBV",PV_DecimationType_Reader));
	m_enableDecimation_RBVPV->setDescription("Decimation type: Frames or Samples ReadBack");
	m_enableDecimation_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_enableDecimation_RBVPV->setEnumeration(decimationTypeEnumeratorStrings);
	addChild(m_enableDecimation_RBVPV);

	m_DecimationFactor_PV.reset(new PVVariableOutImpl<std::int32_t>("DecimationFactor"));
	m_DecimationFactor_PV->setDescription("Decimation");
	m_DecimationFactor_PV->setScanType(scanType_t::passive, 0);
	m_DecimationFactor_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_DecimationFactor_PV);

	m_DecimationOffset_PV.reset(new PVDelegateOutImpl<std::int32_t>("DecimationOffset",PV_DecimationOffset_Writer));
	m_DecimationOffset_PV->setDescription("Index of the first sample that is not decimated");
	m_DecimationOffset_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_DecimationOffset_PV);

	m_DecimationOffset_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("DecimationOffset_RBV",PV_DecimationOffset_Reader));
	m_DecimationOffset_RBVPV->setDescription("Index of the first sample that is not decimated ReadBack");
	m_DecimationOffset_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DecimationOffset_RBVPV);

	enumerationStrings_t enableRAW2ENGEnumeratorStrings;
	enableRAW2ENGEnumeratorStrings.push_back("On");
	enableRAW2ENGEnumeratorStrings.push_back("Off");

	m_enableRaw2EngConversion_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableRAW2Eng",PV_RAW2Eng_Writer));
	m_enableRaw2EngConversion_PV->setDescription("Enable RAW to ENG Conversion of data");
	m_enableRaw2EngConversion_PV->setEnumeration(enableRAW2ENGEnumeratorStrings);
	m_enableRaw2EngConversion_PV->write(getTimestamp(), (std::int32_t)1);
	addChild(m_enableRaw2EngConversion_PV);


	m_enableRaw2EngConversion_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableRAW2Eng_RBV",PV_RAW2Eng_Reader));
	m_enableRaw2EngConversion_RBVPV->setDescription("Enable RAW to ENG Conversion of data ReadBack");
	m_enableRaw2EngConversion_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_enableRaw2EngConversion_RBVPV->setEnumeration(enableRAW2ENGEnumeratorStrings);
	addChild(m_enableRaw2EngConversion_RBVPV);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DataProcessingImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);

}

template<typename T>
timespec DataProcessingImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void DataProcessingImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void DataProcessingImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_dataPV->push(timestamp, data);
}

template<typename T>
size_t DataProcessingImpl<T>::getMaxElements()
{
    return m_dataPV->getMaxElements();
}

template<typename T>
void DataProcessingImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}


template class DataProcessingImpl<std::int32_t>;
template class DataProcessingImpl<double>;
template class DataProcessingImpl<std::vector<std::int8_t> >;
template class DataProcessingImpl<std::vector<std::uint8_t> >;
template class DataProcessingImpl<std::vector<std::int32_t> >;
template class DataProcessingImpl<std::vector<double> >;
template class DataProcessingImpl<std::string >;


}
