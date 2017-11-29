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
#include "nds3/impl/FFTImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
FFTImpl<T>::FFTImpl( const std::string& name,
										   size_t maxElements,
										   stateChange_t switchOnFunction,
										   stateChange_t switchOffFunction,
										   stateChange_t startFunction,
										   stateChange_t stopFunction,
										   stateChange_t recoverFunction,
										   allowChange_t allowStateChangeFunction,
										   writerInt32_t PV_FFTEnable_Writer,
										   writerInt32_t PV_FFTWindowType_Writer,
										   writerInt32_t PV_FFTFrameOverlap_Writer,
										   writerInt32_t PV_FFTFrameSize_Writer,
										   writerInt32_t PV_FFTSmoothFactor_Writer
										  ):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_OnStartDelegate(startFunction),
    m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_DataIn_PV.reset(new PVVariableInImpl<T>("DataIn"));
    m_DataIn_PV->setMaxElements(maxElements);
    m_DataIn_PV->setDescription("Data received in the FFT node");
    m_DataIn_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataIn_PV);

    m_DataOut_PV.reset(new PVVariableInImpl<T>("DataOut"));
    m_DataOut_PV->setMaxElements(maxElements);
    m_DataOut_PV->setDescription("Data produced by the FFT node");
    m_DataOut_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_DataOut_PV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_Decimation_PV);

	//add enumeration
	enumerationStrings_t FFTenableEnumeratorStrings;
	FFTenableEnumeratorStrings.push_back("On");
	FFTenableEnumeratorStrings.push_back("Off");

	m_FFTEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFTEnable",PV_FFTEnable_Writer));
	m_FFTEnable_PV->setDescription("Enable/Disable the FFT operation");
	m_FFTEnable_PV->setEnumeration(FFTenableEnumeratorStrings);
	m_FFTEnable_PV->setScanType(scanType_t::passive, 0);
	addChild(m_FFTEnable_PV);

	m_FFTEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("FFTEnable_RBV"));
	m_FFTEnable_RBVPV->setDescription("Enable/Disable FFT operation ReadBack");
	m_FFTEnable_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_FFTEnable_RBVPV->setEnumeration(FFTenableEnumeratorStrings);
	addChild(m_FFTEnable_RBVPV);

	enumerationStrings_t FFTWindowTypeEnumeratorStrings;
	FFTWindowTypeEnumeratorStrings.push_back("None");
	FFTWindowTypeEnumeratorStrings.push_back("Barlett");
	FFTWindowTypeEnumeratorStrings.push_back("Blackman");
	FFTWindowTypeEnumeratorStrings.push_back("Flattop");
	FFTWindowTypeEnumeratorStrings.push_back("Hann");
	FFTWindowTypeEnumeratorStrings.push_back("Hamm");
	FFTWindowTypeEnumeratorStrings.push_back("Tukey");
	FFTWindowTypeEnumeratorStrings.push_back("Welch");

	m_FFTWindowType_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFTWindowType",PV_FFTWindowType_Writer));
	m_FFTWindowType_PV->setDescription("FFT types");
	m_FFTWindowType_PV->setEnumeration(FFTWindowTypeEnumeratorStrings);
	m_FFTWindowType_PV->setScanType(scanType_t::passive, 0);
	addChild(m_FFTWindowType_PV);


	m_FFTWindowType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("FFTWindowType_RBV"));
	m_FFTWindowType_RBVPV->setDescription("FFT types");
	m_FFTWindowType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_FFTWindowType_RBVPV->setEnumeration(FFTWindowTypeEnumeratorStrings);
	addChild(m_FFTWindowType_RBVPV);

	m_FFTFrameOverlap_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFTFrameOverlap",PV_FFTFrameOverlap_Writer));
	m_FFTFrameOverlap_PV->setDescription("Overlap amount between consecutive frames");
	m_FFTFrameOverlap_PV->setScanType(scanType_t::passive, 0);
	addChild(m_FFTFrameOverlap_PV);

	m_FFTFrameOverlap_RBVPV.reset(new PVVariableInImpl<std::int32_t>("FFTFrameOverlap_RBV"));
	m_FFTFrameOverlap_RBVPV->setDescription("Overlap amount between consecutive frames ReadBack");
	m_FFTFrameOverlap_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_FFTFrameOverlap_RBVPV);

	m_FFTFrameSize_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFTFrameSize",PV_FFTFrameSize_Writer));
	m_FFTFrameSize_PV->setDescription("Frame size in samples for the FFT");
	m_FFTFrameSize_PV->setScanType(scanType_t::passive, 0);
	addChild(m_FFTFrameSize_PV);

	m_FFTFrameSize_RBVPV.reset(new PVVariableInImpl<std::int32_t>("FFTFrameSize_RBV"));
	m_FFTFrameSize_RBVPV->setDescription("Frame size in samples for the FFT ReadBack");
	m_FFTFrameSize_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_FFTFrameSize_RBVPV);

	m_FFTSmoothFactor_PV.reset(new PVDelegateOutImpl<std::int32_t>("FFTSmoothFactor",PV_FFTSmoothFactor_Writer));
	m_FFTSmoothFactor_PV->setDescription("Smoothing factor for averaging the FFT");
	m_FFTSmoothFactor_PV->setScanType(scanType_t::passive, 0);
	addChild(m_FFTSmoothFactor_PV);

	m_FFTSmoothFactor_RBVPV.reset(new PVVariableInImpl<std::int32_t>("FFTSmoothFactor_RBV"));
	m_FFTSmoothFactor_RBVPV->setDescription("Smoothing factor for averaging the FFT ReadBack");
	m_FFTSmoothFactor_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_FFTSmoothFactor_RBVPV);

    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&FFTImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);

}

template<typename T>
timespec FFTImpl<T>::getStartTimestamp() const
{
    return m_StartTime;
}

template<typename T>
void FFTImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void FFTImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_DataOut_PV->push(timestamp, data);
}

template<typename T>
size_t FFTImpl<T>::getMaxElements()
{
    return m_DataOut_PV->getMaxElements();
}

template<typename T>
void FFTImpl<T>::onStart()
{
    m_StartTime = m_StartTimestampFunction();
    m_DataOut_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_DataIn_PV->setDecimation((std::uint32_t)m_Decimation_PV->getValue());
    m_OnStartDelegate();
}

template<typename T>
size_t FFTImpl<T>::getFFTEnable()
{
	std::int32_t FFTEnable;
	timespec timestamp;
	m_FFTEnable_RBVPV->read(&timestamp, &FFTEnable);
	return (std::int32_t)FFTEnable;
}

template<typename T>
size_t FFTImpl<T>::getFFTWindowType()
{
	std::int32_t FFTWindowType;
	timespec timestamp;
	m_FFTWindowType_RBVPV->read(&timestamp, &FFTWindowType);
	return (std::int32_t)FFTWindowType;
}

template<typename T>
size_t FFTImpl<T>::getFFTFrameOverlap()
{
	std::int32_t FFTFrameOverlap;
	timespec timestamp;
	m_FFTFrameOverlap_RBVPV->read(&timestamp, &FFTFrameOverlap);
	return (std::int32_t)FFTFrameOverlap;
}

template<typename T>
size_t FFTImpl<T>::getFFTFrameSize()
{
	std::int32_t FFTFrameSize;
	timespec timestamp;
	m_FFTFrameSize_RBVPV->read(&timestamp, &FFTFrameSize);
	return (std::int32_t)FFTFrameSize;
}

template<typename T>
size_t FFTImpl<T>::getFFTSmoothFactor()
{
	std::int32_t FFTSmoothFactor;
	timespec timestamp;
	m_FFTSmoothFactor_RBVPV->read(&timestamp, &FFTSmoothFactor);
	return (std::int32_t)FFTSmoothFactor;
}

template<typename T>
void FFTImpl<T>::setFFTEnable(const timespec& timestamp, const std::int32_t& value)
{
	m_FFTEnable_RBVPV->setValue(timestamp, value);
	m_FFTEnable_RBVPV->push(timestamp, value);
}

template<typename T>
void FFTImpl<T>::setFFTWindowType(const timespec& timestamp, const std::int32_t& value)
{
	m_FFTWindowType_RBVPV->setValue(timestamp, value);
	m_FFTWindowType_RBVPV->push(timestamp, value);
}
template<typename T>
void FFTImpl<T>::setFFTFrameOverlap(const timespec& timestamp, const std::int32_t& value)
{
	m_FFTFrameOverlap_RBVPV->setValue(timestamp, value);
	m_FFTFrameOverlap_RBVPV->push(timestamp, value);
}
template<typename T>
void FFTImpl<T>::setFFTFrameSize(const timespec& timestamp, const std::int32_t& value)
{
	m_FFTFrameSize_RBVPV->setValue(timestamp, value);
	m_FFTFrameSize_RBVPV->push(timestamp, value);
}
template<typename T>
void FFTImpl<T>::setFFTSmoothFactor(const timespec& timestamp, const std::int32_t& value)
{
	m_FFTSmoothFactor_RBVPV->setValue(timestamp, value);
	m_FFTSmoothFactor_RBVPV->push(timestamp, value);
}

template class FFTImpl<std::int32_t>;
template class FFTImpl<double>;
template class FFTImpl<std::vector<std::int8_t> >;
template class FFTImpl<std::vector<std::uint8_t> >;
template class FFTImpl<std::vector<std::int32_t> >;
template class FFTImpl<std::vector<double> >;

}
