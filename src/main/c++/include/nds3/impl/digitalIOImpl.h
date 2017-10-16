/*
 * Nominal Device Support v.3 (NDS3)
 *
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSDIGITALIOIMPL_H
#define NDSDIGITALIOIMPL_H

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
class DigitalIOImpl: public NodeImpl
{
public:
    DigitalIOImpl(const std::string& name,
                    size_t maxElements,
                    stateChange_t switchOnFunction,
                    stateChange_t switchOffFunction,
                    stateChange_t startFunction,
                    stateChange_t stopFunction,
                    stateChange_t recoverFunction,
                    allowChange_t allowStateChangeFunction,
					writerInt32_t PV_voltLevelHigh_Writer,
					writerInt32_t PV_voltLevelLow_Writer,
					writerInt32_t PV_ChannelDir_Writer);


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
    size_t getVoltLevelHigh();
    size_t getVoltLevelLow();
    size_t getChannelDir();

    void setNumberOfPushedDataBlocks(const timespec& timestamp, const std::int32_t& value);
    void setVoltLevelHigh(const timespec& timestamp, const std::int32_t& value);
    void setVoltLevelLow(const timespec& timestamp, const std::int32_t& value);
    void setChannelDir(const timespec& timestamp, const std::int32_t& value);

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
    std::shared_ptr<PVVariableInImpl<T> > m_dataIn_PV;
    std::shared_ptr<PVVariableOutImpl<T> > m_dataOut_PV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_decimation_PV;

    std::shared_ptr<StateMachineImpl> m_stateMachine;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_voltLevelHigh_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_voltLevelHigh_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_voltLevelLow_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_voltLevelLow_RBVPV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_channelDir_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_channelDir_RBVPV;

    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_NumberOfPushedDataBlocks;


};

}
#endif // NDSDIGITALIOIMPL_H

