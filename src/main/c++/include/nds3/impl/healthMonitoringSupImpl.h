/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSHEALTHMONITORINGSUPIMPL_H
#define NDSHEALTHMONITORINGSUPIMPL_H

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
class HealthMonitSupImpl: public NodeImpl
{
public:
	HealthMonitSupImpl( const std::string& name,
						stateChange_t switchOnFunction,
						stateChange_t switchOffFunction,
						stateChange_t startFunction,
						stateChange_t stopFunction,
						stateChange_t recoverFunction,
						allowChange_t allowStateChangeFunction,
						readerDouble_t PV_DevicePower_Reader,
						readerDouble_t PV_DeviceTemp_Reader,
						readerDouble_t PV_DeviceVoltage_Reader,
						readerDouble_t PV_DeviceCurrent_Reader,
						writerInt32_t PV_EnableSEU_Writer,
						readerInt32_t PV_EnableSEU_Reader,
						writerInt32_t PV_EnableMonitorDAQ_Writer,
						readerInt32_t PV_EnableMonitorDAQ_Reader,
						writerInt32_t PV_EnableShelfTest_Writer,
						readerInt32_t PV_EnableShelfTest_Reader,
						writerInt32_t PV_ShelfTestType_Writer,
						readerInt32_t PV_ShelfTestType_Reader,
						writerInt32_t PV_VerboseShelfTest_Writer,
						readerInt32_t PV_VerboseShelfTest_Reader,
						writerInt32_t PV_EnableShelfTestId_Writer,
						readerInt32_t PV_EnableShelfTestId_Reader,
						writerInt32_t PV_EnableShelfTestText_Writer,
						readerInt32_t PV_EnableShelfTestText_Reader,
						readerInt32_t PV_SignalQualityFlag_Reader,
						writerDouble_t PV_SignalQualityFlagLevel_Writer);


    /**
     * @brief Specifies the function to call to get the timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Returns the timestamp at start.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
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
     * @brief Delegate function that retrieves the start time.
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_startTimestampFunction;

    /**
     * @brief start time. via the delegate function declared in
     * m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs

    std::shared_ptr<PVDelegateInImpl<double> > m_DevPower_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DevTemperature_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DevVoltage_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DevCurrent_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_SEUEnable_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_SEUEnable_RBVPV;


    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_HQMonitorDAQEnable_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_HQMonitorDAQEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestEnable_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TestEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TestType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestVerboseEnable_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TestVerboseEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestIDEnable_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TestIDEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestTxtEnable_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TestTxtEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_TestCodeResultEnable_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_TestCodeResultEnable_RBVPV;

    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_SignalQFlag_PV;
    std::shared_ptr<PVDelegateOutImpl<double> > m_SignalQFlagTrigLevel_PV;
    std::shared_ptr<PVVariableInImpl<double> > m_SignalQFlagTrigLevel_RBVPV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Decimation_PV;

    std::shared_ptr<StateMachineImpl> m_StateMachine;


};

}
#endif // NDSHEALTHMONITORINGSUPIMPL_H

