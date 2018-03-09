/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSFTEIMPL_H
#define NDSFTEIMPL_H

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
class FTEImpl: public NodeImpl
{
public:
	FTEImpl(const std::string& name,
			stateChange_t switchOnFunction,
			stateChange_t switchOffFunction,
			stateChange_t startFunction,
			stateChange_t stopFunction,
			stateChange_t recoverFunction,
			allowChange_t allowStateChangeFunction,
			writerInt32_t PV_Set_Writer,
			writerInt32_t PV_Suppress_Writer,
			writerInt32_t PV_ChgPeriod_Writer,
			writerInt32_t PV_PendingValue_Writer,
			writerInt32_t PV_Maximum_Writer,
			readerTime_t PV_Time_Reader);


    /**
     * @brief Specifies the function to call to get the start timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    std::int32_t getTerminalSet();
    std::int32_t getModeSet();
	timespec getStartTimeSet();
	timespec getStopTimeSet();
	std::int32_t getLevelSet();
	std::int32_t getPeriodNsecSet();
	std::int32_t getDutyCycleSet();
	std::int32_t getSet();
	std::string getSetStatus();
	std::int32_t getSetCode();

    std::int32_t getTerminalSuppress();
    std::int32_t getModeSuppress();
	std::int32_t getAllSuppress();
	timespec getStartTimeSuppress();
	std::int32_t getSuppress();
	std::string getSuppressStatus();
	std::int32_t getSuppressCode();

    std::int32_t getTerminalChgPeriod();
    std::int32_t getPeriodChgPeriod();
	std::int32_t getChgPeriod();
	std::string getChgPeriodStatus();
	std::int32_t getChgPeriodCode();

	std::int32_t getTerminalPending();
	std::int32_t getPendingValue();

	std::int32_t getMaximum();

    void setTerminalSet(const timespec& timestamp, const std::int32_t& value);
    void setModeSet(const timespec& timestamp, const std::int32_t& value);
    void setStartTimeSet(const timespec& timestamp, const timespec& value);
	void setStopTimeSet(const timespec& timestamp, const timespec& value);
	void setLevelSet(const timespec& timestamp, const std::int32_t& value);
	void setPeriodNsecSet(const timespec& timestamp, const std::int32_t& value);
	void setDutyCycleSet(const timespec& timestamp, const std::int32_t& value);
	void setSet(const timespec& timestamp, const std::int32_t& value);
	void setSetStatus(const timespec& timestamp, const std::string& value);
	void setSetCode(const timespec& timestamp, const std::int32_t& value);

    void setTerminalSuppress(const timespec& timestamp, const std::int32_t& value);
    void setModeSuppress(const timespec& timestamp, const std::int32_t& value);
	void setAllSuppress(const timespec& timestamp, const std::int32_t& value);
	void setStartTimeSuppress(const timespec& timestamp, const timespec& value);
	void setSuppress(const timespec& timestamp, const std::int32_t& value);
	void setSuppressStatus(const timespec& timestamp, const std::string& value);
	void setSuppressCode(const timespec& timestamp, const std::int32_t& value);

    void setTerminalChgPeriod(const timespec& timestamp, const std::int32_t& value);
    void setPeriodChgPeriod(const timespec& timestamp, const std::int32_t& value);
	void setChgPeriod(const timespec& timestamp, const std::int32_t& value);
	void setChgPeriodStatus(const timespec& timestamp, const std::string& value);
	void setChgPeriodCode(const timespec& timestamp, const std::int32_t& value);

	void setPendingValue(const timespec& timestamp, const std::int32_t& value);

	void setMaximum(const timespec& timestamp, const std::int32_t& value);



    /**
     * @brief Returns the timestamp at start.
     *
     * This value is set when the state switches to running.
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
    stateChange_t m_OnStartDelegate;
    /**
     * @brief Delegate function that retrieves the start time.
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_StartTimestampFunction;

    /**
     * @brief Start time. Retrieved via the delegate
     *        function declared in  m_startTimestampFunction.
     */
    timespec m_StartTime;

    // PVs
    //////////////////////////////////////////////////////////////////////////////////////////
    // Set FTE PVs
    //////////////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<PVVariableInImpl<std::int32_t>> m_TerminalSet_PV; //TODO: Check PV type (int32 or vector int32)
    std::shared_ptr<PVVariableInImpl<std::int32_t>> m_ModeSet_PV;
    std::shared_ptr<PVVariableInImpl<timespec> > m_StartTimeSet_PV;
    std::shared_ptr<PVVariableInImpl<timespec> > m_StopTimeSet_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_LevelSet_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PeriodNsecSet_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_DutyCycleSet_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Set_PV;
    std::shared_ptr<PVVariableInImpl <std::int32_t> > m_Set_RBVPV;
    std::shared_ptr<PVVariableInImpl<std::string> >  m_SetStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SetCode_PV;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Suppress FTE PVs
	//////////////////////////////////////////////////////////////////////////////////////////
	std::shared_ptr<PVVariableInImpl<std::int32_t>> m_TerminalSuppress_PV; //TODO: Check PV type (int32 or vector int32)
    std::shared_ptr<PVVariableInImpl<std::int32_t>> m_ModeSuppress_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t>> m_AllSuppress_PV;
    std::shared_ptr<PVVariableInImpl<timespec> > m_StartTimeSuppress_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Suppress_PV;
    std::shared_ptr<PVVariableInImpl <std::int32_t> > m_Suppress_RBVPV;
    std::shared_ptr<PVVariableInImpl<std::string> >  m_SuppressStatus_PV;
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_SuppressCode_PV;

    //////////////////////////////////////////////////////////////////////////////////////////
    // Change Period FTE PVs
    //////////////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_TerminalChgPeriod_PV; //TODO: Check PV type (int32 or vector int32)
    std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PeriodChgPeriod_PV;
    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ChgPeriod_PV;
    std::shared_ptr<PVVariableInImpl <std::int32_t> > m_ChgPeriod_RBVPV;
    std::shared_ptr<PVVariableInImpl<std::string> >  m_ChgPeriodStatus_PV;
	std::shared_ptr<PVVariableInImpl<std::int32_t> > m_ChgPeriodCode_PV;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Pending FTEs PV
	//////////////////////////////////////////////////////////////////////////////////////////
	std::shared_ptr<PVDelegateOutImpl<std::int32_t>> m_TerminalPending_PV; //TODO: Check PV type (int32 or vector int32)
	std::shared_ptr<PVVariableInImpl<std::int32_t> > m_PendingValue_PV;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Maximum FTEs PV
	//////////////////////////////////////////////////////////////////////////////////////////
	std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_Maximum_PV;
	std::shared_ptr<PVVariableInImpl<std::int32_t> > m_Maximum_RBVPV; //TODO: ReadbackValue??


	std::shared_ptr<StateMachineImpl> m_StateMachine;

};

}
#endif // NDSFTEIMPL_H

