/*
 * Nominal Device Support v.3 (NDS3)
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#include "nds3/definitions.h"
#include "nds3/impl/dataGenerationImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
DataGenerationImpl<T>::DataGenerationImpl(
        const std::string& name,
        size_t maxElements,
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
		writerDouble_t PV_Frequency_Writer,
		readerDouble_t PV_Frequency_Reader,
		writerDouble_t PV_RefFrequency_Writer,
		readerDouble_t PV_RefFrequency_Reader,
		writerDouble_t PV_Amp_Writer,
		readerDouble_t PV_Amp_Reader,
		writerDouble_t PV_Phase_Writer,
		readerDouble_t PV_Phase_Reader,
		writerDouble_t PV_UpdateRate_Writer,
		readerDouble_t PV_UpdateRate_Reader,
		writerDouble_t PV_DutyCycle_Writer,
		readerDouble_t PV_DutyCycle_Reader,
		writerDouble_t PV_Gain_Writer,
		readerDouble_t PV_Gain_Reader,
		writerDouble_t PV_Offset_Writer,
		readerDouble_t PV_Offset_Reader,
		writerDouble_t PV_Bw_Writer,
		readerDouble_t PV_Bw_Reader,
		writerDouble_t PV_Resolution_Writer,
		readerDouble_t PV_Resolution_Reader,
		writerDouble_t PV_Impedance_Writer,
		readerDouble_t PV_Impedance_Reader,
		writerInt32_t PV_Coupling_Writer,
		readerInt32_t PV_Coupling_Reader,
		writerInt32_t PV_SignalRef_Writer,
		readerInt32_t PV_SignalRef_Reader,
		writerInt32_t PV_SignalType_Writer,
		readerInt32_t PV_SignalType_Reader,
		writerInt32_t PV_Ground_Writer,
		readerInt32_t PV_Ground_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
    m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{

	// Add the children PVs
    m_dataPV.reset(new PVVariableOutImpl<T>("Data"));
    m_dataPV->setMaxElements(maxElements);
    m_dataPV->setDescription("AWG Samples to send");
    m_dataPV->setScanType(scanType_t::interrupt, 0);
    addChild(m_dataPV);

    m_frequency_PV.reset(new PVDelegateOutImpl<double>("Frequency", PV_Frequency_Writer));
    m_frequency_PV->setDescription("Generation frequency");
    m_frequency_PV->write(getTimestamp(), (double)1);
    addChild(m_frequency_PV);

    m_frequency_RBVPV.reset(new PVDelegateInImpl<double>("Frequency_RBV", PV_Frequency_Reader));
	m_frequency_RBVPV->setDescription("Generation frequency ReadBack");
	m_frequency_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_frequency_RBVPV->write(getTimestamp(), (double)1);
	addChild(m_frequency_RBVPV);

    m_RefFrequency_PV.reset(new PVDelegateOutImpl<double>("RefFrequency", PV_RefFrequency_Writer));
    m_RefFrequency_PV->setDescription("Reference frequency");
    m_RefFrequency_PV->write(getTimestamp(), (double)1);
    addChild(m_RefFrequency_PV);

    m_RefFrequency_RBVPV.reset(new PVDelegateInImpl<double>("RefFrequency_RBV", PV_RefFrequency_Reader));
    m_RefFrequency_RBVPV->setDescription("Reference frequency");
    m_RefFrequency_RBVPV->setScanType(scanType_t::passive, 0);
    m_RefFrequency_RBVPV->write(getTimestamp(), (double)1);
    addChild(m_RefFrequency_RBVPV);


    m_amplitude_PV.reset(new PVDelegateOutImpl<double>("Amplitude", PV_Amp_Writer));
    m_amplitude_PV->setDescription("Amplitude");
    m_amplitude_PV->write(getTimestamp(), (double)1);
    addChild(m_amplitude_PV);

    m_amplitude_RBVPV.reset(new PVDelegateInImpl<double>("Amplitude_RBV", PV_Amp_Reader));
	m_amplitude_RBVPV->setDescription("Amplitude ReadBack");
	m_amplitude_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_amplitude_RBVPV->write(getTimestamp(), (double)1);
	addChild(m_amplitude_RBVPV);

    m_phase_PV.reset(new PVDelegateOutImpl<double>("Phase", PV_Phase_Writer));
    m_phase_PV->setDescription("Phase");
    m_phase_PV->write(getTimestamp(), (double)1);
    addChild(m_phase_PV);

    m_phase_RBVPV.reset(new PVDelegateInImpl<double>("Phase_RBV", PV_Phase_Reader));
	m_phase_RBVPV->setDescription("Phase ReadBack");
	m_phase_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_phase_RBVPV->write(getTimestamp(), (double)1);
	addChild(m_phase_RBVPV);

	m_updateRate_PV.reset(new PVDelegateOutImpl<double>("UpdateRate", PV_UpdateRate_Writer));
    m_updateRate_PV->setDescription("Update Rate");
    m_updateRate_PV->write(getTimestamp(), (double)1);
    addChild(m_updateRate_PV);

    m_updateRate_RBVPV.reset(new PVDelegateInImpl<double>("UpdateRate_RBV", PV_UpdateRate_Reader));
	m_updateRate_RBVPV->setDescription("Update Rate ReadBack");
	m_updateRate_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_updateRate_RBVPV->write(getTimestamp(), (double)1);
	addChild(m_updateRate_RBVPV);

    m_DutyCycle_PV.reset(new PVDelegateOutImpl<double>("DutyCycle", PV_DutyCycle_Writer));
    m_DutyCycle_PV->setDescription("Signal Duty Cycle");
    m_DutyCycle_PV->write(getTimestamp(), (double)1);
    addChild(m_DutyCycle_PV);

    m_DutyCycle_RBVPV.reset(new PVDelegateInImpl<double>("DutyCycle_RBV", PV_DutyCycle_Reader));
	m_DutyCycle_RBVPV->setDescription("Signal Duty Cycle ReadBack");
	m_DutyCycle_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_DutyCycle_RBVPV->write(getTimestamp(), (double)1);
	addChild(m_DutyCycle_RBVPV);

	m_Gain_PV.reset(new PVDelegateOutImpl<double>("Gain",PV_Gain_Writer));
	m_Gain_PV->setDescription("Gain of the Channel");
	addChild(m_Gain_PV);

	m_Gain_RBVPV.reset(new PVDelegateInImpl<double>("Gain_RBV",PV_Gain_Reader));
	m_Gain_RBVPV->setDescription("Gain of the Channel ReadBack");
	m_Gain_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_Gain_RBVPV);

	m_offset_PV.reset(new PVDelegateOutImpl<double>("Offset",PV_Offset_Writer));
	m_offset_PV->setDescription("Offset");
	addChild(m_offset_PV);

	m_offset_RBVPV.reset(new PVDelegateInImpl<double>("Offset_RBV",PV_Offset_Reader));
	m_offset_RBVPV->setDescription("Offset ReadBack");
	m_offset_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_offset_RBVPV);

	m_BW_PV.reset(new PVDelegateOutImpl<double>("BandWidth",PV_Bw_Writer));
	m_BW_PV->setDescription("BandWidth");
	addChild(m_BW_PV);

	m_BW_RBVPV.reset(new PVDelegateInImpl<double>("BandWidth_RBV",PV_Bw_Reader));
	m_BW_RBVPV->setDescription("BandWidth ReadBack");
	m_BW_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_BW_RBVPV);

	m_Resolution_PV.reset(new PVDelegateOutImpl<double>("Resolution",PV_Resolution_Writer));
	m_Resolution_PV->setDescription("Resolution: Number of Bits per Sample");
	addChild(m_Resolution_PV);

	m_Resolution_RBVPV.reset(new PVDelegateInImpl<double>("Resolution_RBV",PV_Resolution_Reader));
	m_Resolution_RBVPV->setDescription("Resolution: Number of Bits per Sample ReadBack");
	m_Resolution_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Resolution_RBVPV);

	m_Impedance_PV.reset(new PVDelegateOutImpl<double>("Impedance",PV_Impedance_Writer));
	m_Impedance_PV->setDescription("Impedance");
	addChild(m_Impedance_PV);

	m_Impedance_RBVPV.reset(new PVDelegateInImpl<double>("Impedance_RBV",PV_Impedance_Reader));
	m_Impedance_RBVPV->setDescription("Impedance ReadBack");
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


	m_Coupling_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("Coupling_RBV",PV_Coupling_Reader));
	m_Coupling_RBVPV->setDescription("Coupling: AC or DC ReadBack");
	m_Coupling_RBVPV->setScanType(scanType_t::passive, 0);
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

	m_SignalRefType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("SignalRefType_RBV",PV_SignalRef_Reader));
	m_SignalRefType_RBVPV->setDescription("Type of input: Differential or Single Ended ReadBack");
	m_SignalRefType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_SignalRefType_RBVPV->setEnumeration(SignalRefTypeEnumeratorStrings);
	addChild(m_SignalRefType_RBVPV);

	//add enumeration for Signal Reference type
	enumerationStrings_t SignalTypeEnumeratorStrings;
	SignalTypeEnumeratorStrings.push_back("WaveForm");
	SignalTypeEnumeratorStrings.push_back("Spline");
	SignalTypeEnumeratorStrings.push_back("Sin");
	SignalTypeEnumeratorStrings.push_back("Pulse");
	SignalTypeEnumeratorStrings.push_back("Sawtooth");

	m_signalType_PV.reset(new PVDelegateOutImpl<std::int32_t>("SignalRefType",PV_SignalType_Writer));
	m_signalType_PV->setDescription("Type of signal: Waveform, Spline, Sin, Pulse, Sawtooth");
	m_signalType_PV->setEnumeration(SignalTypeEnumeratorStrings);
	addChild(m_signalType_PV);

	m_signalType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("SignalRefType_RBV",PV_SignalType_Reader));
	m_signalType_RBVPV->setDescription("Type of signal: Waveform, Spline, Sin, Pulse, Sawtooth ReadBack");
	m_signalType_RBVPV->setScanType(scanType_t::interrupt, 0);
	m_signalType_RBVPV->setEnumeration(SignalTypeEnumeratorStrings);
	addChild(m_signalType_RBVPV);

	//ad enumerator for ground state
	enumerationStrings_t groundEnumeratorStrings;
	groundEnumeratorStrings.push_back("On");
	groundEnumeratorStrings.push_back("Off");

	m_ground_PV.reset(new PVDelegateOutImpl<std::int32_t>("Ground",PV_Ground_Writer));
	m_ground_PV->setDescription("Ground State");
	m_ground_PV->setEnumeration(groundEnumeratorStrings);
	addChild(m_ground_PV);

	m_ground_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("Ground_RBV",PV_Ground_Reader));
	m_ground_RBVPV->setDescription("Ground State ReadBack");
	m_ground_RBVPV->setScanType(scanType_t::interrupt,0);
	m_ground_RBVPV->setEnumeration(groundEnumeratorStrings);
	addChild(m_ground_RBVPV);


    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&DataGenerationImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);
}

template<typename T>
timespec DataGenerationImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void DataGenerationImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void DataGenerationImpl<T>::write(const timespec& timestamp, const T& data)
{
    m_dataPV->write(timestamp,data);
}

template<typename T>
void DataGenerationImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}


template class DataGenerationImpl<std::int32_t>;
template class DataGenerationImpl<double>;
template class DataGenerationImpl<std::vector<std::int8_t> >;
template class DataGenerationImpl<std::vector<std::uint8_t> >;
template class DataGenerationImpl<std::vector<std::int32_t> >;
template class DataGenerationImpl<std::vector<double> >;
template class DataGenerationImpl<std::string >;


}
