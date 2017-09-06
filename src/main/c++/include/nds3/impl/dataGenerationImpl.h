/*
 * Nominal Device Support v.3 (NDS3)
 *  by GMV & UPM
 *
 */

#ifndef NDSDATAGENERATIONIMPL_H
#define NDSDATAGENERATIONIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;
//TODO ALBB modify this

template<typename T>
class DataGenerationImpl: public NodeImpl
{
public:
    DataGenerationImpl(const std::string& name,
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
						readerInt32_t PV_Ground_Reader);

    /**
     * @brief Specifies the function to call to get the start timestamp.
     *
     * The function is called only once at each start of the AWG and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    void write(const timespec& timestamp, const T& data);

        /**
     * @brief Returns the timestamp at the moment of the start of the acquisition.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when the acquisition started.
     */
    timespec getStartTimestamp() const;

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();


protected:
    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_onStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_startTimestampFunction;

    /**
     * @brief Generation start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs
    std::shared_ptr<PVVariableOutImpl<T> > m_dataPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_frequency_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_frequency_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_RefFrequency_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_RefFrequency_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_amplitude_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_amplitude_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_phase_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_phase_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_updateRate_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_updateRate_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_DutyCycle_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DutyCycle_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Gain_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_Gain_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_offset_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_offset_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_BW_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_BW_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Resolution_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_Resolution_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_Impedance_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_Impedance_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Coupling_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_Coupling_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_SignalRefType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_SignalRefType_RBVPV;

	std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ground_PV;
	std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_ground_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_signalType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_signalType_RBVPV;

    std::shared_ptr<StateMachineImpl> m_stateMachine;

};

}
#endif // NDSDATAGENERATIONIMPL_H

