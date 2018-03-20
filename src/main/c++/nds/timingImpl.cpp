/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */

#include <ctime>

#include "nds3/definitions.h"
#include "nds3/impl/timingImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds {

  // TimingImpl Constructor
  TimingImpl::TimingImpl( const std::string& name,  
        stateChange_t switchOnFunction,
        stateChange_t switchOffFunction,
        stateChange_t startFunction,
        stateChange_t stopFunction,
        stateChange_t recoverFunction,
        allowChange_t allowStateChangeFunction,
        readerTime_t PV_Time_Reader): 
        NodeImpl(name, nodeType_t::dataSourceChannel),
	      m_OnStartDelegate(startFunction),
        m_StartTimestampFunction(std::bind(&BaseImpl::getTimestamp, this)) {

   	// Add the children PVs
  
    // PV: time 
  	m_Time_PV.reset(new PVDelegateInImpl<timespec>("Time", PV_Time_Reader));
  	m_Time_PV->setDescription("Get Time");
  	m_Time_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_Time_PV);
  
    // PV: human readable time 
  	m_HumanTime_PV.reset(new PVDelegateInImpl<std::string>("HTime", std::bind(&TimingImpl::PV_HTime_Reader, this,  std::placeholders::_1, std::placeholders::_2 )));
  	m_HumanTime_PV->setDescription("Get Time in UTC format");
  	m_HumanTime_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_HumanTime_PV);
  
    // PV: Clock Frequency  
  	m_ClkFrequency_PV.reset(new PVVariableInImpl<double>("ClkFrequency"));
  	m_ClkFrequency_PV->setDescription("Clock frequency");
  	m_ClkFrequency_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_ClkFrequency_PV);
  
    // PV: Clock Multiplier 
  	m_ClkMultiplier_PV.reset(new PVVariableInImpl<std::int32_t>("ClkMultiplier"));
  	m_ClkMultiplier_PV->setDescription("Clock multiplier");
  	m_ClkMultiplier_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_ClkMultiplier_PV);
  
    // PV: Sync Status 
    enumerationStrings_t SyncStatusEnumerationStrings;
    SyncStatusEnumerationStrings.push_back("NOT_SYNCED");
    SyncStatusEnumerationStrings.push_back("SYNCING");
    SyncStatusEnumerationStrings.push_back("SYNCED");
    SyncStatusEnumerationStrings.push_back("LOST_SYNC");
  
  	m_SyncStatus_PV.reset(new PVVariableInImpl<std::int32_t>("SyncStatus"));
  	m_SyncStatus_PV->setDescription("Synchronization status");
  	m_SyncStatus_PV->setScanType(scanType_t::interrupt, 0);
    m_SyncStatus_PV->setEnumeration(SyncStatusEnumerationStrings);
  	addChild(m_SyncStatus_PV);
  
    // PV: SecsLastSync
  	m_SecsLastSync_PV.reset(new PVVariableInImpl<std::int32_t>("SecsLastSync"));
  	m_SecsLastSync_PV->setDescription("Seconds since last synchronisation");
  	m_SecsLastSync_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_SecsLastSync_PV);
  
    // PV: Reference Time Base  
  	m_RefTimeBase_PV.reset(new PVVariableInImpl<timespec>("RefTimeBase"));
  	m_RefTimeBase_PV->setDescription("Reference time Base");
  	m_RefTimeBase_PV->setScanType(scanType_t::interrupt, 0);
    addChild(m_RefTimeBase_PV);
  
    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                     switchOnFunction,
                                     switchOffFunction,
                                     std::bind(&TimingImpl::onStart, this),
                                     stopFunction,
                                     recoverFunction,
                                     allowStateChangeFunction));
     addChild(m_StateMachine);
  }

  // Commoon  functions
  timespec TimingImpl::getStartTimestamp() const
  {
    return m_StartTime;
  }

  void TimingImpl::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
  {
    m_StartTimestampFunction = timestampDelegate;
  }

  void TimingImpl::push(const timespec& timestamp, const timespec& data)
  {
    // TODO is empty
	  m_Time_PV->push(timestamp, data);
  }


  void TimingImpl::onStart()
  {
    m_StartTime = m_StartTimestampFunction();
    // Esto por qu'e es as'i? no me queda claro
    m_Time_PV->setDecimation(1);
    // Pensar en c'omo asociar el HumanTime al Time
    m_OnStartDelegate();
  }
  
  // ------------------------ Delegate Functions ---------------------------- //
  void TimingImpl::PV_HTime_Reader(timespec* /*timestamp*/, std::string *value) {
    timespec curtime = getTime();
    tm *gmttm = gmtime(&curtime.tv_sec); 
    *value = asctime(gmttm);
  }


  // ---------------------------- Getters ----------------------------------- //
  timespec TimingImpl::getTime() {
    timespec curtime;
    timespec timestamp; 
    m_Time_PV -> read( &timestamp, &curtime);
    return curtime;   
  }

  std::string TimingImpl::getHumanTime() {
    std::string UTCTime;
    timespec timestamp; 
    m_HumanTime_PV -> read( &timestamp, &UTCTime);
    return UTCTime;   
  }
  double TimingImpl::getClkFrequency() {
  	double ClkFrequency;
  	timespec timestamp;
  	m_ClkFrequency_PV -> read(&timestamp, &ClkFrequency);
  	return ClkFrequency;
  }

  std::int32_t TimingImpl::getClkMultiplier() {
    std::int32_t ClkMultiplier;
  	timespec timestamp;
  	m_ClkMultiplier_PV-> read(&timestamp, &ClkMultiplier);
  	return ClkMultiplier;
  }

  std::int32_t TimingImpl::getSyncStatus() {
    std::int32_t SyncStatus;
  	timespec timestamp;
  	m_SyncStatus_PV -> read(&timestamp, &SyncStatus);
  	return SyncStatus;
  }

  std::int32_t TimingImpl::getSecsLastSync() {
    std::int32_t SecsLastSync;
  	timespec timestamp;
  	m_SecsLastSync_PV -> read(&timestamp, &SecsLastSync);
  	return SecsLastSync;
  }
  
  timespec TimingImpl::getRefTimeBase() {
    timespec getRefTimeBase;
  	timespec timestamp;
  	m_RefTimeBase_PV -> read(&timestamp, &getRefTimeBase);
  	return getRefTimeBase;
  }

  // --------------------------- Setters ----------------------------------- //
  
  void TimingImpl::setTime(const timespec& timestamp, const timespec& value) {
     m_Time_PV -> push(timestamp, value);  
  }

  void TimingImpl::setHumanTime(const timespec& timestamp, const std::string& value) {
     m_HumanTime_PV -> push(timestamp, value);  
  }

  void TimingImpl::setClkFrequency(const timespec& timestamp, const double& value) {
     m_ClkFrequency_PV -> setValue(timestamp, value);  
     m_ClkFrequency_PV -> push(timestamp, value);  
  }

  void TimingImpl::setClkMultiplier(const timespec& timestamp, const std::int32_t& value) {
     m_ClkMultiplier_PV -> setValue(timestamp, value);  
     m_ClkMultiplier_PV -> push(timestamp, value);  
  }

  void TimingImpl::setSyncStatus(const timespec& timestamp, const std::int32_t& value) {
     m_SyncStatus_PV -> setValue(timestamp, value);  
     m_SyncStatus_PV -> push(timestamp, value);  
  }

  void TimingImpl::setSecsLastSync(const timespec& timestamp, const std::int32_t& value) {
     m_SecsLastSync_PV -> setValue(timestamp, value);  
     m_SecsLastSync_PV -> push(timestamp, value);  
  }

  void TimingImpl::setRefTimeBase(const timespec& timestamp, const timespec& value) {
     m_RefTimeBase_PV -> setValue(timestamp, value);  
     m_RefTimeBase_PV -> push(timestamp, value);  
  }
}
