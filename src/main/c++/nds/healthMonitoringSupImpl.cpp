/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/definitions.h"
#include "nds3/impl/healthMonitoringSupImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

HealthMonitSupImpl::HealthMonitSupImpl(  const std::string& name,
											stateChange_t switchOnFunction,
											stateChange_t switchOffFunction,
											stateChange_t startFunction,
											stateChange_t stopFunction,
											stateChange_t recoverFunction,
											allowChange_t allowStateChangeFunction,
											readerDouble_t PV_DevicePower_Reader,
											readerDouble_t PV_DeviceTemperature_Reader,
											readerDouble_t PV_DeviceVoltage_Reader,
											readerDouble_t PV_DeviceCurrent_Reader,
											writerInt32_t PV_SEUEnable_Writer,
											writerInt32_t PV_DAQEnable_Writer,
											writerInt32_t PV_SelfTestEnable_Writer,
											writerInt32_t PV_SelfTestType_Writer,
											writerInt32_t PV_SelfTestVerboseEnable_Writer,
											writerInt32_t PV_SelfTestIDEnable_Writer,
											writerInt32_t PV_SelfTestTxtEnable_Writer,
											readerInt32_t PV_SignalQualityFlag_Reader,
											writerDouble_t PV_SignalQualityFlagLevel_Writer):
    NodeImpl(name, nodeType_t::dataSourceChannel),
	m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_DevPower_PV.reset(new PVDelegateInImpl<double>("DevPower",PV_DevicePower_Reader));
	m_DevPower_PV->setDescription("Device Power");
	m_DevPower_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_DevPower_PV);

    m_DevTemperature_PV.reset(new PVDelegateInImpl<double>("DevTemperature",PV_DeviceTemperature_Reader));
	m_DevTemperature_PV->setDescription("Device Temperature");
	m_DevTemperature_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_DevTemperature_PV);

	m_DevVoltage_PV.reset(new PVDelegateInImpl<double>("DevVoltage",PV_DeviceVoltage_Reader));
	m_DevVoltage_PV->setDescription("Device Volt");
	m_DevVoltage_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_DevVoltage_PV);

	m_DevCurrent_PV.reset(new PVDelegateInImpl<double>("DevCurrent",PV_DeviceCurrent_Reader));
	m_DevCurrent_PV->setDescription("Device Current");
	m_DevCurrent_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_DevCurrent_PV);

    m_SEUEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("SEUEnable",PV_SEUEnable_Writer));
    m_SEUEnable_PV->setDescription("Enable Detection of Single Events Upsets");
    m_SEUEnable_PV->write(getTimestamp(), (std::int32_t)0);
    addChild(m_SEUEnable_PV);

    m_SEUEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("SEUEnable_RBV"));
	m_SEUEnable_RBVPV->setDescription("Enable Detection of SEU ReadBack");
	m_SEUEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_SEUEnable_RBVPV);


    m_HQMonitorDAQEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("DAQEnable",PV_DAQEnable_Writer));
    m_HQMonitorDAQEnable_PV->setDescription("Enable Monitoring of DAQ anomalies");
    m_HQMonitorDAQEnable_PV->write(getTimestamp(), (std::int32_t)0);
    addChild(m_HQMonitorDAQEnable_PV);

    m_HQMonitorDAQEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("DAQEnable_RBV"));
	m_HQMonitorDAQEnable_RBVPV->setDescription("Enable Monitor of DAQ anomalies ReadBack");
	m_HQMonitorDAQEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_HQMonitorDAQEnable_RBVPV);


    m_TestEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestEnable",PV_SelfTestEnable_Writer));
    m_TestEnable_PV->setDescription("Enable (start) the Self-Test");
    m_TestEnable_PV->write(getTimestamp(), (std::int32_t)0);
    addChild(m_TestEnable_PV);

    m_TestEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestEnable_RBV"));
	m_TestEnable_RBVPV->setDescription("Enable the Self-Test ReadBack");
	m_TestEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_TestEnable_RBVPV);

    enumerationStrings_t SelfTestEnumeratorStrings;
    SelfTestEnumeratorStrings.push_back("Quick-Test");
    SelfTestEnumeratorStrings.push_back("Full-Test");

    m_TestType_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestType",PV_SelfTestType_Writer));
    m_TestType_PV->setDescription("Type of Self-Test");
    m_TestType_PV->setEnumeration(SelfTestEnumeratorStrings);
    addChild(m_TestType_PV);

    m_TestType_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestType_RBV"));
    m_TestType_RBVPV->setDescription("Type of Self-Test ReadBack");
    m_TestType_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_TestType_RBVPV);


    m_TestVerboseEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestVerboseEnable",PV_SelfTestVerboseEnable_Writer));
    m_TestVerboseEnable_PV->setDescription("Enable the Self-Test Verbose");
    m_TestVerboseEnable_PV->write(getTimestamp(), (std::int32_t)0);
    addChild(m_TestVerboseEnable_PV);

    m_TestVerboseEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestVerboseEnable_RBV"));
	m_TestVerboseEnable_RBVPV->setDescription("Enable Verbose Self-Test Verbose ReadBack");
	m_TestVerboseEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_TestVerboseEnable_RBVPV);

    m_TestIDEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestIDEnable",PV_SelfTestIDEnable_Writer));
    m_TestIDEnable_PV->setDescription("Enable the Self-Test ID");
    m_TestIDEnable_PV->write(getTimestamp(), (std::int32_t)0);
    addChild(m_TestIDEnable_PV);

    m_TestIDEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestIDEnable_RBV"));
	m_TestIDEnable_RBVPV->setDescription("Enable the Self-Test ID ReadBack");
	m_TestIDEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_TestIDEnable_RBVPV);

    m_TestTxtEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestTxtEnable",PV_SelfTestTxtEnable_Writer));
    m_TestTxtEnable_PV->setDescription("Enable the Self-Test text description");
    m_TestTxtEnable_PV->write(getTimestamp(), (std::int32_t)0);
    addChild(m_TestTxtEnable_PV);

    m_TestTxtEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestTxtEnable_RBV"));
	m_TestTxtEnable_RBVPV->setDescription("Enable the Self-Test text description ReadBack");
	m_TestTxtEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_TestTxtEnable_RBVPV);

    m_TestCodeResultEnable_PV.reset(new PVDelegateOutImpl<std::int32_t>("TestCodeResultEnable",PV_SelfTestTxtEnable_Writer));
    m_TestCodeResultEnable_PV->setDescription("Enable the Self-Test result number");
    m_TestCodeResultEnable_PV->write(getTimestamp(), (std::int32_t)0);
    addChild(m_TestCodeResultEnable_PV);

    m_TestCodeResultEnable_RBVPV.reset(new PVVariableInImpl<std::int32_t>("TestCodeResultEnable_RBV"));
	m_TestCodeResultEnable_RBVPV->setDescription("Enable the Self-Test result number ReadBack");
	m_TestCodeResultEnable_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_TestCodeResultEnable_RBVPV);

    m_SignalQFlag_PV.reset(new PVDelegateInImpl<std::int32_t>("SignalQFlag",PV_SignalQualityFlag_Reader));
	m_SignalQFlag_PV->setDescription("Read the flag of low quality signal");
	m_SignalQFlag_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_SignalQFlag_PV);

	m_SignalQFlagTrigLevel_PV.reset(new PVDelegateOutImpl<double>("SignalQFlagTrigLevel", PV_SignalQualityFlagLevel_Writer));
	m_SignalQFlagTrigLevel_PV->setDescription("Quality Flag trigger level ReadBack");
	m_SignalQFlagTrigLevel_PV-> setScanType(scanType_t::passive,0);
	addChild(m_SignalQFlagTrigLevel_PV);

    m_SignalQFlagTrigLevel_RBVPV.reset(new PVVariableInImpl<double>("SignalQFlagTrigLevel_RBV"));
	m_SignalQFlagTrigLevel_RBVPV->setDescription("Quality Flag trigger level ReadBack");
	m_SignalQFlagTrigLevel_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_SignalQFlagTrigLevel_RBVPV);

    m_Decimation_PV.reset(new PVVariableOutImpl<std::int32_t>("Decimation"));
    m_Decimation_PV->setDescription("Decimation");
    m_Decimation_PV->setScanType(scanType_t::passive, 0);
    m_Decimation_PV->write(getTimestamp(), (std::int32_t)1);
    addChild(m_Decimation_PV);

    // Add state machine
    m_StateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&HealthMonitSupImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_StateMachine);
}


timespec HealthMonitSupImpl::getStartTimestamp() const
{
    return m_startTime;
}

void HealthMonitSupImpl::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

void HealthMonitSupImpl::onStart()
{
    m_startTime = m_startTimestampFunction();
    m_onStartDelegate();
}

}
