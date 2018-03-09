/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/definitions.h"
#include "nds3/impl/FTEImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
FTEImpl<T>::FTEImpl(const std::string& name,
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
		readerTime_t PV_Time_Reader):
		NodeImpl(name, nodeType_t::dataSourceChannel),
		m_OnStartDelegate(startFunction),
		m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
		{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set FTE PVs
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_TerminalSet_PV.reset(new PVVariableInImpl<std::int32_t>("TerminalSet"));
	m_TerminalSet_PV->setDescription("Terminal to set FTE");
	m_TerminalSet_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TerminalSet_PV);

	m_ModeSet_PV.reset(new PVVariableInImpl<std::int32_t>("ModeSet"));
	m_ModeSet_PV->setDescription("Mode (Single, Pulse, Clk, InmLVL)");
	m_ModeSet_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ModeSet_PV);

	m_StartTimeSet_PV.reset(new PVVariableInImpl<timespec>("StartTimeSet"));
	m_StartTimeSet_PV->setDescription("Start Time of FTE");
	m_StartTimeSet_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_StartTimeSet_PV);

	m_StopTimeSet_PV.reset(new PVVariableInImpl<timespec>("StopTimeSet"));
	m_StopTimeSet_PV->setDescription("Stop Time of FTE");
	m_StopTimeSet_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_StopTimeSet_PV);

	m_LevelSet_PV.reset(new PVVariableInImpl<std::int32_t>("LevelSet"));
	m_LevelSet_PV->setDescription("Signal level of FTE");
	m_LevelSet_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_LevelSet_PV);

	m_PeriodNsecSet_PV.reset(new PVVariableInImpl<std::int32_t>("PeriodNsecSet"));
	m_PeriodNsecSet_PV->setDescription("Period in Nanoseconds");
	m_PeriodNsecSet_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_PeriodNsecSet_PV);

	m_DutyCycleSet_PV.reset(new PVVariableInImpl<std::int32_t>("DutyCycleSet"));
	m_DutyCycleSet_PV->setDescription("Duty Cycle percentage");
	m_DutyCycleSet_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DutyCycleSet_PV);

	m_Set_PV.reset(new PVDelegateOutImpl<std::int32_t>("Set",PV_Set_Writer));
	m_Set_PV->setDescription("Set FTE");
	m_Set_PV->setScanType(scanType_t::passive, 0);
	addChild(m_Set_PV);

	m_Set_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Set_RBV"));
	m_Set_RBVPV->setDescription("Set ReadBack");
	m_Set_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Set_RBVPV);

	m_SetStatus_PV.reset(new PVVariableInImpl<std::string>("SetStatus"));
	m_SetStatus_PV->setDescription("Report on FTE configuration");
	m_SetStatus_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SetStatus_PV);

	m_SetCode_PV.reset(new PVVariableInImpl<std::int32_t>("SetCode"));
	m_SetCode_PV->setDescription("Code of success/error setting FTE configuration");
	m_SetCode_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SetCode_PV);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Suppress FTE PVs
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_TerminalSuppress_PV.reset(new PVVariableInImpl<std::int32_t>("TerminalSuppress"));
	m_TerminalSuppress_PV->setDescription("Terminal to suppress FTE");
	m_TerminalSuppress_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TerminalSuppress_PV);

	m_ModeSuppress_PV.reset(new PVVariableInImpl<std::int32_t>("ModeSuppress"));
	m_ModeSuppress_PV->setDescription("Mode of suppress FTE/clock");
	m_ModeSuppress_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ModeSuppress_PV);

	m_AllSuppress_PV.reset(new PVVariableInImpl<std::int32_t>("AllSuppress"));
	m_AllSuppress_PV->setDescription("Suppress one or Suppress all FTEs");
	m_AllSuppress_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_AllSuppress_PV);

	m_StartTimeSuppress_PV.reset(new PVVariableInImpl<timespec>("StartTimeSuppress"));
	m_StartTimeSuppress_PV->setDescription("Start Time on FTE suppression");
	m_StartTimeSuppress_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_StartTimeSuppress_PV);

	m_Suppress_PV.reset(new PVDelegateOutImpl<std::int32_t>("Suppress",PV_Suppress_Writer));
	m_Suppress_PV->setDescription("Suppress");
	m_Suppress_PV->setScanType(scanType_t::passive, 0);
	addChild(m_Suppress_PV);

	m_Suppress_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Suppress_RBV"));
	m_Suppress_RBVPV->setDescription("Suppress ReadBack");
	m_Suppress_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Suppress_RBVPV);

	m_SuppressStatus_PV.reset(new PVVariableInImpl<std::string>("SuppressStatus"));
	m_SuppressStatus_PV->setDescription("Report on FTE suppression");
	m_SuppressStatus_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SuppressStatus_PV);

	m_SuppressCode_PV.reset(new PVVariableInImpl<std::int32_t>("SuppressCode"));
	m_SuppressCode_PV->setDescription("Code of success/error on FTE suppression");
	m_SuppressCode_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SuppressCode_PV);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Change Period FTE PVs
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_TerminalChgPeriod_PV.reset(new PVVariableInImpl<std::int32_t>("TerminalChgPeriod"));
	m_TerminalChgPeriod_PV->setDescription("Terminal to change the period of FTE");
	m_TerminalChgPeriod_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TerminalChgPeriod_PV);

	m_PeriodChgPeriod_PV.reset(new PVVariableInImpl<std::int32_t>("PeriodChgPeriod"));
	m_PeriodChgPeriod_PV->setDescription("Period in nanoseconds to change on FTE");
	m_PeriodChgPeriod_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_PeriodChgPeriod_PV);

	m_ChgPeriod_PV.reset(new PVDelegateOutImpl<std::int32_t>("ChgPeriod",PV_ChgPeriod_Writer));
	m_ChgPeriod_PV->setDescription("ChgPeriod");
	m_ChgPeriod_PV->setScanType(scanType_t::passive, 0);
	addChild(m_ChgPeriod_PV);

	m_ChgPeriod_RBVPV.reset(new PVVariableInImpl<std::int32_t>("ChgPeriod_RBV"));
	m_ChgPeriod_RBVPV->setDescription("ChgPeriod ReadBack");
	m_ChgPeriod_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ChgPeriod_RBVPV);

	m_ChgPeriodStatus_PV.reset(new PVVariableInImpl<std::string>("ChgPeriodStatus"));
	m_ChgPeriodStatus_PV->setDescription("Report on FTE clock period change");
	m_ChgPeriodStatus_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ChgPeriodStatus_PV);

	m_ChgPeriodCode_PV.reset(new PVVariableInImpl<std::int32_t>("ChgPeriodCode"));
	m_ChgPeriodCode_PV->setDescription("Code of success/error on FTE changing clock period");
	m_ChgPeriodCode_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_ChgPeriodCode_PV);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pending FTEs PV
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_TerminalPending_PV.reset(new PVDelegateOutImpl<std::int32_t>("TerminalPending",PV_PendingValue_Writer));
	m_TerminalPending_PV->setDescription("Terminal Pending");
	m_TerminalPending_PV->setScanType(scanType_t::passive, 0);
	addChild(m_TerminalPending_PV);

	m_PendingValue_PV.reset(new PVVariableInImpl<std::int32_t>("PendingValue"));
	m_PendingValue_PV->setDescription("Pending FTEs in the terminal");
	m_PendingValue_PV->setScanType(scanType_t::interrupt, 0);
	addChild(m_PendingValue_PV);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Maximum FTEs PV
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_Maximum_PV.reset(new PVDelegateOutImpl<std::int32_t>("Maximum",PV_Maximum_Writer));
	m_Maximum_PV->setDescription("Maximum FTEs");
	m_Maximum_PV->setScanType(scanType_t::passive, 0);
	addChild(m_Maximum_PV);

	m_Maximum_RBVPV.reset(new PVVariableInImpl<std::int32_t>("Maximum_RBV"));
	m_Maximum_RBVPV->setDescription("Maximum FTEs ReadBack");
	m_Maximum_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_Maximum_RBVPV);

	// Add state machine
	m_StateMachine.reset(new StateMachineImpl(true,
			switchOnFunction,
			switchOffFunction,
			std::bind(&FTEImpl::onStart, this),
			stopFunction,
			recoverFunction,
			allowStateChangeFunction));
	addChild(m_StateMachine);
		}

template<typename T>
timespec FTEImpl<T>::getStartTimestamp() const
{
	return m_StartTime;
}

template<typename T>
void FTEImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
	m_StartTimestampFunction = timestampDelegate;
}

template<typename T>
void FTEImpl<T>::onStart()
{
	m_StartTime = m_StartTimestampFunction();
	m_OnStartDelegate();
}

///////////////////////////////////////////////////////////////
// Set FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTEImpl<T>::getTerminalSet()
{
	std::int32_t terminalSet;
	timespec timestamp;
	m_TerminalSet_PV->read(&timestamp, &terminalSet);
	return terminalSet;
}


template<typename T>
std::int32_t FTEImpl<T>::getModeSet()
{
	std::int32_t modeSet;
	timespec timestamp;
	m_ModeSet_PV->read(&timestamp, &modeSet);
	return modeSet;
}

template<typename T>
timespec FTEImpl<T>::getStartTimeSet()
{
	timespec startTimeSet;
	timespec timestamp;
	m_StartTimeSet_PV->read(&timestamp, &startTimeSet);
	return startTimeSet;
}

template<typename T>
timespec FTEImpl<T>::getStopTimeSet()
{
	timespec stopTimeSet;
	timespec timestamp;
	m_StopTimeSet_PV->read(&timestamp, &stopTimeSet);
	return stopTimeSet;
}

template<typename T>
std::int32_t FTEImpl<T>::getLevelSet()
{
	std::int32_t levelSet;
	timespec timestamp;
	m_LevelSet_PV->read(&timestamp, &levelSet);
	return levelSet;
}

template<typename T>
std::int32_t FTEImpl<T>::getPeriodNsecSet()
{
	std::int32_t periodNsecSet;
	timespec timestamp;
	m_PeriodNsecSet_PV->read(&timestamp, &periodNsecSet);
	return periodNsecSet;
}


template<typename T>
std::int32_t FTEImpl<T>::getDutyCycleSet()
{
	std::int32_t dutyCycleSet;
	timespec timestamp;
	m_DutyCycleSet_PV->read(&timestamp, &dutyCycleSet);
	return dutyCycleSet;
}

template<typename T>
std::int32_t FTEImpl<T>::getSet()
{
	std::int32_t set;
	timespec timestamp;
	m_Set_RBVPV->read(&timestamp, &set);
	return set;
}

template<typename T>
std::string FTEImpl<T>::getSetStatus()
{
	std::string setStatus;
	timespec timestamp;
	m_SetStatus_PV->read(&timestamp, &setStatus);
	return setStatus;
}

template<typename T>
std::int32_t FTEImpl<T>::getSetCode()
{
	std::int32_t setCode;
	timespec timestamp;
	m_SetCode_PV->read(&timestamp, &setCode);
	return setCode;
}

///////////////////////////////////////////////////////////////
// Suppress FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTEImpl<T>::getTerminalSuppress()
{
	std::int32_t terminalSuppress;
	timespec timestamp;
	m_TerminalSuppress_PV->read(&timestamp, &terminalSuppress);
	return terminalSuppress;
}


template<typename T>
std::int32_t FTEImpl<T>::getModeSuppress()
{
	std::int32_t modeSuppress;
	timespec timestamp;
	m_ModeSuppress_PV->read(&timestamp, &modeSuppress);
	return modeSuppress;
}

template<typename T>
std::int32_t FTEImpl<T>::getAllSuppress()
{
	std::int32_t allSuppress;
	timespec timestamp;
	m_AllSuppress_PV->read(&timestamp, &allSuppress);
	return allSuppress;
}

template<typename T>
timespec FTEImpl<T>::getStartTimeSuppress()
{
	timespec startTimeSuppress;
	timespec timestamp;
	m_StartTimeSuppress_PV->read(&timestamp, &startTimeSuppress);
	return startTimeSuppress;
}

template<typename T>
std::int32_t FTEImpl<T>::getSuppress()
{
	std::int32_t suppress;
	timespec timestamp;
	m_Suppress_RBVPV->read(&timestamp, &suppress);
	return suppress;
}

template<typename T>
std::string FTEImpl<T>::getSuppressStatus()
{
	std::string setStatus;
	timespec timestamp;
	m_SuppressStatus_PV->read(&timestamp, &setStatus);
	return setStatus;
}

template<typename T>
std::int32_t FTEImpl<T>::getSuppressCode()
{
	std::int32_t suppressCode;
	timespec timestamp;
	m_SuppressCode_PV->read(&timestamp, &suppressCode);
	return suppressCode;
}

///////////////////////////////////////////////////////////////
// ChgPeriod FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTEImpl<T>::getTerminalChgPeriod()
{
	std::int32_t terminalChgPeriod;
	timespec timestamp;
	m_TerminalChgPeriod_PV->read(&timestamp, &terminalChgPeriod);
	return terminalChgPeriod;
}


template<typename T>
std::int32_t FTEImpl<T>::getPeriodChgPeriod()
{
	std::int32_t periodChgPeriod;
	timespec timestamp;
	m_PeriodChgPeriod_PV->read(&timestamp, &periodChgPeriod);
	return periodChgPeriod;
}

template<typename T>
std::int32_t FTEImpl<T>::getChgPeriod()
{
	std::int32_t chgPeriod;
	timespec timestamp;
	m_ChgPeriod_RBVPV->read(&timestamp, &chgPeriod);
	return chgPeriod;
}

template<typename T>
std::string FTEImpl<T>::getChgPeriodStatus()
{
	std::string chgPeriodStatus;
	timespec timestamp;
	m_ChgPeriodStatus_PV->read(&timestamp, &chgPeriodStatus);
	return chgPeriodStatus;
}

template<typename T>
std::int32_t FTEImpl<T>::getChgPeriodCode()
{
	std::int32_t chgPeriodCode;
	timespec timestamp;
	m_ChgPeriodCode_PV->read(&timestamp, &chgPeriodCode);
	return chgPeriodCode;
}

///////////////////////////////////////////////////////////////
// Pending FTE getters
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTEImpl<T>::getTerminalPending()
{
	std::int32_t terminalPending;
	timespec timestamp;
	m_TerminalPending_PV->read(&timestamp, &terminalPending);
	return terminalPending;
}

template<typename T>
std::int32_t FTEImpl<T>::getPendingValue()
{
	std::int32_t pendingValue;
	timespec timestamp;
	m_PendingValue_PV->read(&timestamp, &pendingValue);
	return pendingValue;
}

///////////////////////////////////////////////////////////////
// Maximum FTE getter
///////////////////////////////////////////////////////////////
template<typename T>
std::int32_t FTEImpl<T>::getMaximum()
{
	std::int32_t maximum;
	timespec timestamp;
	m_Maximum_PV->read(&timestamp, &maximum);
	return maximum;
}

///////////////////////////////////////////////////////////////
// Set FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setTerminalSet(const timespec& timestamp, const std::int32_t& value)
{
	m_TerminalSet_PV->setValue(timestamp, value);
	m_TerminalSet_PV->push(timestamp, value);
}


template<typename T>
void FTEImpl<T>::setModeSet(const timespec& timestamp, const std::int32_t& value)
{
	m_ModeSet_PV->setValue(timestamp, value);
	m_ModeSet_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setStartTimeSet(const timespec& timestamp, const timespec& value)
{
	m_StartTimeSet_PV->setValue(timestamp, value);
	m_StartTimeSet_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setStopTimeSet(const timespec& timestamp, const timespec& value)
{
	m_StopTimeSet_PV->setValue(timestamp, value);
	m_StopTimeSet_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setLevelSet(const timespec& timestamp, const std::int32_t& value)
{
	m_LevelSet_PV->setValue(timestamp, value);
	m_LevelSet_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setPeriodNsecSet(const timespec& timestamp, const std::int32_t& value)
{
	m_PeriodNsecSet_PV->setValue(timestamp, value);
	m_PeriodNsecSet_PV->push(timestamp, value);
}


template<typename T>
void FTEImpl<T>::setDutyCycleSet(const timespec& timestamp, const std::int32_t& value)
{
	m_DutyCycleSet_PV->setValue(timestamp, value);
	m_DutyCycleSet_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setSet(const timespec& timestamp, const std::int32_t& value)
{
	m_Set_RBVPV->setValue(timestamp, value);
	m_Set_RBVPV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setSetStatus(const timespec& timestamp, const std::string& value)
{
	m_SetStatus_PV->setValue(timestamp, value);
	m_SetStatus_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setSetCode(const timespec& timestamp, const std::int32_t& value)
{
	m_SetCode_PV->setValue(timestamp, value);
	m_SetCode_PV->push(timestamp, value);
}

///////////////////////////////////////////////////////////////
// Suppress FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setTerminalSuppress(const timespec& timestamp, const std::int32_t& value)
{
	m_TerminalSuppress_PV->setValue(timestamp, value);
	m_TerminalSuppress_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setModeSuppress(const timespec& timestamp, const std::int32_t& value)
{
	m_ModeSuppress_PV->setValue(timestamp, value);
	m_ModeSuppress_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setAllSuppress(const timespec& timestamp, const std::int32_t& value)
{
	m_AllSuppress_PV->setValue(timestamp, value);
	m_AllSuppress_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setStartTimeSuppress(const timespec& timestamp, const timespec& value)
{
	m_StartTimeSuppress_PV->setValue(timestamp, value);
	m_StartTimeSuppress_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setSuppress(const timespec& timestamp, const std::int32_t& value)
{
	m_Suppress_RBVPV->setValue(timestamp, value);
	m_Suppress_RBVPV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setSuppressStatus(const timespec& timestamp, const std::string& value)
{
	m_SuppressStatus_PV->setValue(timestamp, value);
	m_SuppressStatus_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setSuppressCode(const timespec& timestamp, const std::int32_t& value)
{
	m_SuppressCode_PV->setValue(timestamp, value);
	m_SuppressCode_PV->push(timestamp, value);
}

///////////////////////////////////////////////////////////////
// ChgPeriod FTE setters
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setTerminalChgPeriod(const timespec& timestamp, const std::int32_t& value)
{
	m_TerminalChgPeriod_PV->setValue(timestamp, value);
	m_TerminalChgPeriod_PV->push(timestamp, value);
}


template<typename T>
void FTEImpl<T>::setPeriodChgPeriod(const timespec& timestamp, const std::int32_t& value)
{
	m_PeriodChgPeriod_PV->setValue(timestamp, value);
	m_PeriodChgPeriod_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setChgPeriod(const timespec& timestamp, const std::int32_t& value)
{
	m_ChgPeriod_RBVPV->setValue(timestamp, value);
	m_ChgPeriod_RBVPV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setChgPeriodStatus(const timespec& timestamp, const std::string& value)
{
	m_ChgPeriodStatus_PV->setValue(timestamp, value);
	m_ChgPeriodStatus_PV->push(timestamp, value);
}

template<typename T>
void FTEImpl<T>::setChgPeriodCode(const timespec& timestamp, const std::int32_t& value)
{
	m_ChgPeriodCode_PV->setValue(timestamp, value);
	m_ChgPeriodCode_PV->push(timestamp, value);
}

///////////////////////////////////////////////////////////////
// Pending FTE setter
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setPendingValue(const timespec& timestamp, const std::int32_t& value)
{
	m_PendingValue_PV->setValue(timestamp, value);
	m_PendingValue_PV->push(timestamp, value);
}

///////////////////////////////////////////////////////////////
// Maximum FTE setter
///////////////////////////////////////////////////////////////
template<typename T>
void FTEImpl<T>::setMaximum(const timespec& timestamp, const std::int32_t& value)
{
	m_Maximum_RBVPV->setValue(timestamp, value);
	m_Maximum_RBVPV->push(timestamp, value);
}


template class FTEImpl<timespec>;
}
