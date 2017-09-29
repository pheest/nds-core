/*
 * Nominal Device Support v.3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * Modified by GMV & UPM
 */

#ifndef NDSDATAACQUISITIONIMPL_H
#define NDSDATAACQUISITIONIMPL_H

#include <memory>
#include "nds3/definitions.h"
#include "nds3/impl/nodeImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"

namespace nds
{

template <typename T> class PVVariableInImpl;
template <typename T> class PVVariableOutImpl;


template<typename T>
class DataAcquisitionImpl: public NodeImpl
{
public:
    DataAcquisitionImpl(const std::string& name,
            size_t maxElements,
            stateChange_t switchOnFunction,
            stateChange_t switchOffFunction,
            stateChange_t startFunction,
            stateChange_t stopFunction,
            stateChange_t recoverFunction,
            allowChange_t allowStateChangeFunction,
    		writerDouble_t PV_Gain_Writer,
    		writerDouble_t PV_Offset_Writer,
    		writerDouble_t PV_Bandwidth_Writer,
    		writerDouble_t PV_Resolution_Writer,
    		writerDouble_t PV_Impedance_Writer,
    		writerInt32_t PV_Coupling_Writer,
    		writerInt32_t PV_SignalRef_Writer,
    		writerInt32_t PV_Ground_Writer);

    /**
     * @brief Specifies the function to call to get the acquisition start timestamp.
     *
     * The function is called only once at each start of the acquisition and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    void push(const timespec& timestamp, const T& data);


    size_t getMaxElements();
    size_t getDecimation();
    size_t getGain();
    size_t getOffset();
    size_t getBandwidth();
    size_t getResolution();
    size_t getImpedance();
    size_t getCoupling();
    size_t getSignalRef();
    size_t getGround();

    void setGain(const timespec& timestamp, const double& value);
    void setOffset(const timespec& timestamp, const double& value);
    void setBandwidth(const timespec& timestamp, const double& value);
    void setResolution(const timespec& timestamp, const double& value);
    void setImpedance(const timespec& timestamp, const std::int32_t& value);
    void setCoupling(const timespec& timestamp, const std::int32_t& value);
    void setSignalRef(const timespec& timestamp, const std::int32_t& value);
    void setGround(const timespec& timestamp, const std::int32_t& value);
    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);

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
     * @brief Acquisition start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs
    std::shared_ptr<PVVariableInImpl<T> > m_data_PV;

    std::shared_ptr<StateMachineImpl> m_stateMachine;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_decimation_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_decimation_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<double> > m_offset_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_offset_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_Gain_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Gain_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_Bandwidth_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Bandwidth_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_Resolution_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_Resolution_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Impedance_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Impedance_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Coupling_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Coupling_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_SignalRefType_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SignalRefType_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ground_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ground_RBVPV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_NumberOfPushedDataBlocks;

};

}
#endif // NDSDATAACQUISITIONIMPL_H

