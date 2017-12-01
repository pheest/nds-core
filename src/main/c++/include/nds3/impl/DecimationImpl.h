/*
 * Nominal Device Support v.3 (NDS3)
 *
 *
 * by GMV & UPM
 */

#ifndef NDSDECIMATIONIMPL_H
#define NDSDECIMATIONIMPL_H

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
class DecimationImpl: public NodeImpl
{
public:
    DecimationImpl( const std::string& name,
						size_t maxElements,
						stateChange_t switchOnFunction,
						stateChange_t switchOffFunction,
						stateChange_t startFunction,
						stateChange_t stopFunction,
						stateChange_t recoverFunction,
						allowChange_t allowStateChangeFunction,
						writerInt32_t PV_DecimationEnable_Writer,
						writerInt32_t PV_DecimationType_Writer,
						writerInt32_t PV_DecimationFactor_Writer,
						writerInt32_t PV_DecimationOffset_Writer
						);



    /**
     * @brief Specifies the function to call to get the starting timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    void push(const timespec& timestamp, const T& data);


    /**
     * @brief Returns the timestamp at start.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;
    size_t getMaxElements();

    /**
     * @brief Called by the state machine. Store the current timestamp and then calls the
     *        delegated onStart function.
     */
    void onStart();

    size_t getDecimationEnable();
    size_t getDecimationType();
    size_t getDecimationFactor();
    size_t getDecimationOffset();

    void setDecimationEnable(const timespec& timestamp, const std::int32_t& value);
    void setDecimationType(const timespec& timestamp, const std::int32_t& value);
    void setDecimationFactor(const timespec& timestamp, const std::int32_t& value);
    void setDecimationOffset(const timespec& timestamp, const std::int32_t& value);



protected:

    /**
     * @brief In the state machine we set the start function to onStart(), so we
     *        remember here what to call from onStart().
     */
    stateChange_t m_OnStartDelegate;

    /**
     * @brief Delegate function that retrieves the start time. Executed
     *        by onStart().
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Acquisition start time. Retrieved during onStart() via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;

    // PVs

    std::shared_ptr<PVVariableInImpl<T> > m_DataIn_PV;
    std::shared_ptr<PVVariableInImpl<T> > m_DataOut_PV;

    std::shared_ptr<PVVariableOutImpl<std::int32_t> > m_Decimation_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_DecimationEnable_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DecimationEnable_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_DecimationType_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DecimationType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_DecimationFactor_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DecimationFactor_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_DecimationOffset_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DecimationOffset_RBVPV;



    std::shared_ptr<StateMachineImpl> m_StateMachine;


};

}
#endif // NDSDECIMATIONIMPL_H

