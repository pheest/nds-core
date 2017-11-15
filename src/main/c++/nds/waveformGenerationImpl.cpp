/*
 * Nominal Device Support v.3 (NDS3)
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/definitions.h"
#include "nds3/impl/waveformGenerationImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
WaveformGenerationImpl<T>::WaveformGenerationImpl(const std::string& name,
                                          size_t maxElements,
                                          stateChange_t switchOnFunction,
                                          stateChange_t switchOffFunction,
                                          stateChange_t startFunction,
                                          stateChange_t stopFunction,
                                          stateChange_t recoverFunction,
                                          allowChange_t allowStateChangeFunction,
		                                  writerDouble_t PV_Frequency_Writer,
		                                  writerDouble_t PV_RefFrequency_Writer,
		                                  writerDouble_t PV_Amp_Writer,
		                                  writerDouble_t PV_Phase_Writer,
		                                  writerDouble_t PV_UpdateRate_Writer,
		                                  writerDouble_t PV_DutyCycle_Writer,
		                                  writerDouble_t PV_Gain_Writer,
		                                  writerDouble_t PV_Offset_Writer,
		                                  writerDouble_t PV_Bandwidth_Writer,
		                                  writerDouble_t PV_Resolution_Writer,
										  writerInt32_t PV_Impedance_Writer,
		                                  writerInt32_t PV_Coupling_Writer,
		                                  writerInt32_t PV_SignalRef_Writer,
		                                  writerInt32_t PV_SignalType_Writer,
		                                  writerInt32_t PV_Ground_Writer):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{

	// Add the children PVs
    m_data_PV.reset(new PVVariableInImpl<T>("data"));
    m_data_PV->setDescription("Waveform Generated");
	m_data_PV->setMaxElements(maxElements);
    m_data_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_data_PV);

	m_data_AWG.reset(new PVVariableOutImpl<T>("dataAWG"));
	m_data_AWG->setMaxElements(maxElements);
	m_data_AWG->setDescription("AWG Samples for signal generation");
	m_data_AWG->setScanType(scanType_t::passive, 0);
    addChild(m_data_AWG);

    m_decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_decimation_PV->setDescription("Decimation");
    m_decimation_PV->setScanType(scanType_t::passive, 0);
    m_decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_decimation_PV);

    m_frequency_RBVPV.reset(new PVVariableInImpl<double>("Frequency_RBV"));
	m_frequency_RBVPV->setDescription("Generation frequency ReadBack");
	m_frequency_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_frequency_RBVPV->getDataDirection();
	addChild(m_frequency_RBVPV);

    m_frequency_PV.reset(new PVDelegateOutImpl<double>("Frequency", PV_Frequency_Writer));
    m_frequency_PV->setDescription("Generation frequency");
    m_frequency_PV->setUnits("Hz");
    m_frequency_PV->getDescription();
    m_frequency_PV->getUnits();
    m_frequency_PV->getScanType();
    m_frequency_PV->getScanPeriodSeconds();
    m_frequency_PV->getEnumerations();
    m_frequency_PV->getProcessAtInit();
    m_frequency_PV->getDataDirection();
    addChild(m_frequency_PV);

    m_RefFrequency_PV.reset(new PVDelegateOutImpl<double>("RefFrequency", PV_RefFrequency_Writer));
    m_RefFrequency_PV->setDescription("Reference frequency");
    addChild(m_RefFrequency_PV);

    m_RefFrequency_RBVPV.reset(new PVVariableInImpl<double>("RefFrequency_RBV"));
    m_RefFrequency_RBVPV->setDescription("Reference frequency");
    m_RefFrequency_RBVPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_RefFrequency_RBVPV);

    m_amplitude_PV.reset(new PVDelegateOutImpl<double>("Amplitude", PV_Amp_Writer));
    m_amplitude_PV->setDescription("Amplitude");
    addChild(m_amplitude_PV);

    //m_Aplitude_RBPV is PVVariable because every time that the CS updates m_Amplitude value, the m_Amplitude writer updates the AmplitudeRBPV value.
    m_amplitude_RBVPV.reset(new PVVariableInImpl<double>("Amplitude_RBV"/*, PV_Amp_Reader*/));
	m_amplitude_RBVPV->setDescription("Amplitude ReadBack");
	m_amplitude_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_amplitude_RBVPV);

    m_phase_PV.reset(new PVDelegateOutImpl<double>("Phase", PV_Phase_Writer));
    m_phase_PV->setDescription("Phase");
    addChild(m_phase_PV);

    m_phase_RBVPV.reset(new PVVariableInImpl<double>("Phase_RBV"));
	m_phase_RBVPV->setDescription("Phase ReadBack");
	m_phase_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_phase_RBVPV);

	m_updateRate_PV.reset(new PVDelegateOutImpl<double>("UpdateRate", PV_UpdateRate_Writer));
    m_updateRate_PV->setDescription("Update Rate");
    addChild(m_updateRate_PV);

    m_updateRate_RBVPV.reset(new PVVariableInImpl<double>("UpdateRate_RBV"));
	m_updateRate_RBVPV->setDescription("Update Rate ReadBack");
	m_updateRate_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_updateRate_RBVPV);

    m_DutyCycle_PV.reset(new PVDelegateOutImpl<double>("DutyCycle", PV_DutyCycle_Writer));
    m_DutyCycle_PV->setDescription("Signal Duty Cycle");
    addChild(m_DutyCycle_PV);

    m_DutyCycle_RBVPV.reset(new PVVariableInImpl<double>("DutyCycle_RBV"));
	m_DutyCycle_RBVPV->setDescription("Signal Duty Cycle ReadBack");
	m_DutyCycle_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DutyCycle_RBVPV);

	m_Gain_PV.reset(new PVDelegateOutImpl<double>("Gain",PV_Gain_Writer));
	m_Gain_PV->setDescription("Gain of the Channel");
	addChild(m_Gain_PV);

	m_Gain_RBVPV.reset(new PVVariableInImpl<double>("Gain_RBV"));
	m_Gain_RBVPV->setDescription("Gain of the Channel ReadBack");
	m_Gain_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_Gain_RBVPV);

	m_offset_PV.reset(new PVDelegateOutImpl<double>("Offset",PV_Offset_Writer));
	m_offset_PV->setDescription("Offset");
	addChild(m_offset_PV);

	m_offset_RBVPV.reset(new PVVariableInImpl<double>("Offset_RBV"));
	m_offset_RBVPV->setDescription("Offset ReadBack");
	m_offset_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_offset_RBVPV);

	m_Bandwidth_PV.reset(new PVDelegateOutImpl<double>("BandWidth",PV_Bandwidth_Writer));
	m_Bandwidth_PV->setDescription("BandWidth");
	addChild(m_Bandwidth_PV);

	m_Bandwidth_RBVPV.reset(new PVVariableInImpl<double>("BandWidth_RBV"));
	m_Bandwidth_RBVPV->setDescription("BandWidth ReadBack");
	m_Bandwidth_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Bandwidth_RBVPV);

	m_Resolution_PV.reset(new PVDelegateOutImpl<double>("Resolution",PV_Resolution_Writer));
	m_Resolution_PV->setDescription("Resolution: Number of Bits per Sample");
	addChild(m_Resolution_PV);

	m_Resolution_RBVPV.reset(new PVVariableInImpl<double>("Resolution_RBV"));
	m_Resolution_RBVPV->setDescription("Resolution: Number of Bits per Sample ReadBack");
	m_Resolution_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Resolution_RBVPV);

	//add enumeration for Signal Reference type
	enumerationStrings_t ImpedanceEnumeratorStrings;
	ImpedanceEnumeratorStrings.push_back("50ohm");
	ImpedanceEnumeratorStrings.push_back("Inf");

	m_Impedance_PV.reset(new PVDelegateOutImpl<std::int32_t>("Impedance",PV_Impedance_Writer));
	m_Impedance_PV->setDescription("Impedance: 50 ohm or Inf ");
	m_Impedance_PV->setEnumeration(ImpedanceEnumeratorStrings);
	addChild(m_Impedance_PV);

	m_Impedance_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Impedance_RBV"));
	m_Impedance_RBVPV->setDescription("Impedance ReadBack");
	m_Impedance_RBVPV->setEnumeration(ImpedanceEnumeratorStrings);
	m_Impedance_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Impedance_RBVPV);

	//add enumeration for Signal Reference type
	enumerationStrings_t CouplingEnumeratorStrings;
	CouplingEnumeratorStrings.push_back("AC");
	CouplingEnumeratorStrings.push_back("DC");

	m_Coupling_PV.reset(new PVDelegateOutImpl<std::int32_t>("Coupling",PV_Coupling_Writer));
	m_Coupling_PV->setDescription("Coupling: AC or DC");
	m_Coupling_PV->setEnumeration(CouplingEnumeratorStrings);
	addChild(m_Coupling_PV);


	m_Coupling_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Coupling_RBV"));
	m_Coupling_RBVPV->setDescription("Coupling: AC or DC ReadBack");
	m_Coupling_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_Coupling_RBVPV->setEnumeration(CouplingEnumeratorStrings);
	addChild(m_Coupling_RBVPV);

	//add enumeration for Signal Reference type
	enumerationStrings_t SignalRefTypeEnumeratorStrings;
	SignalRefTypeEnumeratorStrings.push_back("SingleEnded");
	SignalRefTypeEnumeratorStrings.push_back("Differential");

	m_SignalRefType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalRefType",PV_SignalRef_Writer));
	m_SignalRefType_PV->setDescription("Type of input: Differential or Single Ended");
	m_SignalRefType_PV->setEnumeration(SignalRefTypeEnumeratorStrings);
	addChild(m_SignalRefType_PV);

	m_SignalRefType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SignalRefType_RBV"));
	m_SignalRefType_RBVPV->setDescription("Type of input: Differential or Single Ended ReadBack");
	m_SignalRefType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_SignalRefType_RBVPV->setEnumeration(SignalRefTypeEnumeratorStrings);
	addChild(m_SignalRefType_RBVPV);

	//add enumeration for Signal Reference type
	enumerationStrings_t SignalTypeEnumeratorStrings;
	SignalTypeEnumeratorStrings.push_back("WaveForm");
	SignalTypeEnumeratorStrings.push_back("Spline");
	SignalTypeEnumeratorStrings.push_back("DC");
	SignalTypeEnumeratorStrings.push_back("Sin");
	SignalTypeEnumeratorStrings.push_back("Square");
	SignalTypeEnumeratorStrings.push_back("Triangle");
	SignalTypeEnumeratorStrings.push_back("Pulse");
	SignalTypeEnumeratorStrings.push_back("Sawtooth");

	m_signalType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalType",PV_SignalType_Writer));
	m_signalType_PV->setDescription("Type of signal: Waveform, Spline,DC, Sin, Square, Triangle, Pulse, Sawtooth");
	m_signalType_PV->setEnumeration(SignalTypeEnumeratorStrings);
	addChild(m_signalType_PV);

	m_signalType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SignalType_RBV"));
	m_signalType_RBVPV->setDescription("Type of signal: Waveform, Spline,DC, Sin,Square, Triangle, Pulse, Sawtooth ReadBack");
	m_signalType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_signalType_RBVPV->setEnumeration(SignalTypeEnumeratorStrings);
	addChild(m_signalType_RBVPV);

	//ad enumerator for ground state
	enumerationStrings_t groundEnumeratorStrings;
	groundEnumeratorStrings.push_back("On");
	groundEnumeratorStrings.push_back("Off");

	m_Ground_PV.reset(new PVDelegateOutImpl<std::int32_t>("Ground",PV_Ground_Writer));
	m_Ground_PV->setDescription("Ground State");
	m_Ground_PV->setEnumeration(groundEnumeratorStrings);
	addChild(m_Ground_PV);

	m_Ground_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Ground_RBV"));
	m_Ground_RBVPV->setDescription("Ground State ReadBack");
	m_Ground_RBVPV->setScanType(scanType_t::interrupt,0);
	m_Ground_RBVPV->setEnumeration(groundEnumeratorStrings);
	addChild(m_Ground_RBVPV);

	// Add the children PVs
	m_NumberOfPushedDataBlocks.reset(new PVVariableInImpl<std::int32_t>("NumberOfPushedDataBlocks"));
	m_NumberOfPushedDataBlocks->setDescription("Number Of Pushed Data Blocks");
	m_NumberOfPushedDataBlocks->setScanType(scanType_t::interrupt, 0);
	addChild(m_NumberOfPushedDataBlocks);


    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&WaveformGenerationImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);
}


template<typename T>
void WaveformGenerationImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
timespec WaveformGenerationImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void WaveformGenerationImpl<T>::push(const timespec& timestamp, const T& data)
{
    m_data_PV->push(timestamp,data);
}

template<typename T>
size_t WaveformGenerationImpl<T>::getMaxElements()
{
    return m_data_PV->getMaxElements();
}

template<typename T>
size_t WaveformGenerationImpl<T>::getSignalType()
{
	std::int32_t signalType;
	timespec timestamp;
	m_signalType_RBVPV->read(&timestamp, &signalType);
	return (size_t)signalType;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getAmplitude()
{
	double amplitude;
	timespec timestamp;
	m_amplitude_RBVPV->read(&timestamp, &amplitude);
	return (double)amplitude;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getFrequency()
{
	double frequency;
	timespec timestamp;
	m_frequency_RBVPV->read(&timestamp, &frequency);
	return (double)frequency;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getUpdateRate()
{
	double updateRate;
	timespec timestamp;
	m_updateRate_RBVPV->read(&timestamp, &updateRate);
	return (double)updateRate;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getOffset()
{
	double offset;
	timespec timestamp;
	m_offset_RBVPV->read(&timestamp, &offset);
	return (double)offset;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getPhase()
{
	double phase;
	timespec timestamp;
	m_phase_RBVPV->read(&timestamp, &phase);
	return (double)phase;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getImpedance()
{
	std::int32_t impedance;
	timespec timestamp;
	m_Impedance_RBVPV->read(&timestamp, &impedance);
	return (std::int32_t)impedance;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getRefFrequency()
{
	double RefFrequency;
	timespec timestamp;
	m_RefFrequency_RBVPV->read(&timestamp, &RefFrequency);
	return (double)RefFrequency;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getDutyCycle()
{
	double DutyCycle;
	timespec timestamp;
	m_DutyCycle_RBVPV->read(&timestamp, &DutyCycle);
	return (double)DutyCycle;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getGain()
{
	double Gain;
	timespec timestamp;
	m_Gain_RBVPV->read(&timestamp, &Gain);
	return (double)Gain;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getBandwidth()
{
	double Bandwidth;
	timespec timestamp;
	m_Bandwidth_RBVPV->read(&timestamp, &Bandwidth);
	return (double)Bandwidth;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getResolution()
{
	double Resolution;
	timespec timestamp;
	m_Resolution_RBVPV->read(&timestamp, &Resolution);
	return (double)Resolution;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getCoupling()
{
	std::int32_t Coupling;
	timespec timestamp;
	m_Coupling_RBVPV->read(&timestamp, &Coupling);
	return (std::int32_t)Coupling;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getSignalRef()
{
	std::int32_t SignalRef;
	timespec timestamp;
	m_SignalRefType_RBVPV->read(&timestamp, &SignalRef);
	return (std::int32_t)SignalRef;
}

template<typename T>
size_t WaveformGenerationImpl<T>::getGround()
{
	std::int32_t Ground;
	timespec timestamp;
	m_Ground_RBVPV->read(&timestamp, &Ground);
	return (std::int32_t)Ground;
}

template<typename T>
void WaveformGenerationImpl<T>::setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value)
{
	m_NumberOfPushedDataBlocks->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setAmplitude(const timespec& timestamp, const double& value)
{
	m_amplitude_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setSignalType(const timespec& timestamp, const std::int32_t& value)
{
	m_signalType_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setFrequency(const timespec& timestamp, const double& value)
{
	m_frequency_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setUpdateRate(const timespec& timestamp, const double& value)
{
	m_updateRate_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setOffset(const timespec& timestamp, const double& value)
{
	m_offset_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setPhase(const timespec& timestamp, const double& value)
{
	m_phase_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setImpedance(const timespec& timestamp, const std::int32_t& value)
{
	m_Impedance_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setRefFrequency(const timespec& timestamp, const double& value)
{
	m_RefFrequency_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setDutyCycle(const timespec& timestamp, const double& value)
{
	m_DutyCycle_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setGain(const timespec& timestamp, const double& value)
{
	m_Gain_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setBandwidth(const timespec& timestamp, const double& value)
{
	m_Bandwidth_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setResolution(const timespec& timestamp, const double& value)
{
	m_Resolution_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setCoupling(const timespec& timestamp, const std::int32_t& value)
{
	m_Coupling_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setSignalRef(const timespec& timestamp, const std::int32_t& value)
{
	m_SignalRefType_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::setGround(const timespec& timestamp, const std::int32_t& value)
{
	m_Ground_RBVPV->setValue(timestamp, value);
}

template<typename T>
void WaveformGenerationImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_data_PV->setDecimation((std::uint32_t)m_decimation_PV->getValue());
    m_onStartDelegate();
}


template class WaveformGenerationImpl<std::int32_t>;
template class WaveformGenerationImpl<double>;
template class WaveformGenerationImpl<std::vector<std::int8_t> >;
template class WaveformGenerationImpl<std::vector<std::uint8_t> >;
template class WaveformGenerationImpl<std::vector<std::int32_t> >;
template class WaveformGenerationImpl<std::vector<double> >;


}
