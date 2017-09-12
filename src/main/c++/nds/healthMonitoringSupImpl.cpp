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

template<typename T>
HealthMonitSupImpl<T>::HealthMonitSupImpl(  const std::string& name,
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
											readerDouble_t PV_SignalQualityFlagLevel_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel),
	m_onStartDelegate(startFunction),
    m_startTimestampFunction(std::bind(&BaseImpl::getTimestamp, this))
{
	// Add the children PVs
    m_DevicePower_PV.reset(new PVDelegateInImpl<double>("DevicePower",PV_DevicePower_Reader));
	m_DevicePower_PV->setDescription("Device Power");
	m_DevicePower_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_DevicePower_PV);

    m_DeviceTemp_PV.reset(new PVDelegateInImpl<double>("DeviceTemp",PV_DeviceTemp_Reader));
	m_DeviceTemp_PV->setDescription("Device Temp");
	m_DeviceTemp_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_DeviceTemp_PV);

	m_DeviceVoltage_PV.reset(new PVDelegateInImpl<double>("DeviceVoltage",PV_DeviceVoltage_Reader));
	m_DeviceVoltage_PV->setDescription("Device Volt");
	m_DeviceVoltage_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_DeviceVoltage_PV);

	m_DeviceCurrent_PV.reset(new PVDelegateInImpl<double>("DeviceCurrent",PV_DeviceCurrent_Reader));
	m_DeviceCurrent_PV->setDescription("Device Current");
	m_DeviceCurrent_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_DeviceCurrent_PV);

    m_EnableSEU_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableSEU",PV_EnableSEU_Writer));
    m_EnableSEU_PV->setDescription("Enable Detect SEU");
    addChild(m_EnableSEU_PV);

    m_EnableSEU_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableSEU_RBV",PV_EnableSEU_Reader));
	m_EnableSEU_RBVPV->setDescription("Enable Detect SEU ReadBack");
	m_EnableSEU_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_EnableSEU_RBVPV);


    m_EnableMonitorDAQ_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableMonitorDAQ",PV_EnableMonitorDAQ_Writer));
    m_EnableMonitorDAQ_PV->setDescription("Enable Monitor DAQ anomalies");
    addChild(m_EnableMonitorDAQ_PV);

    m_EnableMonitorDAQ_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableSEU_RBV",PV_EnableMonitorDAQ_Reader));
	m_EnableMonitorDAQ_RBVPV->setDescription("Enable Monitor DAQ anomalies ReadBack");
	m_EnableMonitorDAQ_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_EnableMonitorDAQ_RBVPV);


    m_EnableShelfTest_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableShelfTest",PV_EnableShelfTest_Writer));
    m_EnableShelfTest_PV->setDescription("Enable ShelfTest");
    addChild(m_EnableShelfTest_PV);

    m_EnableShelfTest_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableShelfTest_RBV",PV_EnableShelfTest_Reader));
	m_EnableShelfTest_RBVPV->setDescription("Enable ShelfTest ReadBack");
	m_EnableShelfTest_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_EnableShelfTest_RBVPV);

    enumerationStrings_t ShelfTestEnumeratorStrings;
    ShelfTestEnumeratorStrings.push_back("Quick-Test");
    ShelfTestEnumeratorStrings.push_back("Full-Test");

    m_ShelfTestType_PV.reset(new PVDelegateOutImpl<std::int32_t>("ShelfTestType",PV_ShelfTestType_Writer));
    m_ShelfTestType_PV->setDescription("Enable ShelfTest");
    m_ShelfTestType_PV->setEnumeration(ShelfTestEnumeratorStrings);
    addChild(m_ShelfTestType_PV);

    m_EnableShelfTest_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("ShelfTestType_RBV",PV_ShelfTestType_Reader));
	m_EnableShelfTest_RBVPV->setDescription("ShelfTest Type ReadBack");
	m_EnableShelfTest_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_EnableShelfTest_RBVPV);


    m_VerboseShelfTest_PV.reset(new PVDelegateOutImpl<std::int32_t>("VerboseShelfTest",PV_VerboseShelfTest_Writer));
    m_VerboseShelfTest_PV->setDescription("Enable Verbose ShelfTest");
    addChild(m_VerboseShelfTest_PV);

    m_VerboseShelfTest_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableShelfTest_RBV",PV_VerboseShelfTest_Reader));
	m_VerboseShelfTest_RBVPV->setDescription("Enable Verbose ShelfTest ReadBack");
	m_VerboseShelfTest_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_VerboseShelfTest_RBVPV);

    m_EnableShelfTestId_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableShelfTestId",PV_EnableShelfTestId_Writer));
    m_EnableShelfTestId_PV->setDescription("Enable ShelfTest Id");
    addChild(m_EnableShelfTestId_PV);

    m_EnableShelfTestId_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableShelfTestId_RBV",PV_EnableShelfTestId_Reader));
	m_EnableShelfTestId_RBVPV->setDescription("Enable ShelfTest Id ReadBack");
	m_EnableShelfTestId_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_EnableShelfTestId_RBVPV);

    m_EnableShelfTestText_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableShelfTestText",PV_EnableShelfTestText_Writer));
    m_EnableShelfTestText_PV->setDescription("Enable ShelfTest Out Text");
    addChild(m_EnableShelfTestText_PV);

    m_EnableShelfTestText_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableShelfTestText_RBV",PV_EnableShelfTestText_Reader));
	m_EnableShelfTestText_RBVPV->setDescription("Enable ShelfTest Out Text ReadBack");
	m_EnableShelfTestText_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_EnableShelfTestText_RBVPV);

    m_EnableShelfTestOutputNum_PV.reset(new PVDelegateOutImpl<std::int32_t>("m_EnableShelfTestTextOutputNum_RBVPV",PV_EnableShelfTestText_Writer));
    m_EnableShelfTestOutputNum_PV->setDescription("Enable ShelfTest Out Num");
    addChild(m_EnableShelfTestOutputNum_PV);

    m_EnableShelfTestTextOutputNum_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("m_EnableShelfTestTextOutputNum_RBVPV_RBV",PV_EnableShelfTestText_Reader));
	m_EnableShelfTestTextOutputNum_RBVPV->setDescription("Enable ShelfTest Out Num ReadBack");
	m_EnableShelfTestTextOutputNum_RBVPV-> setScanType(scanType_t::interrupt,0);
	addChild(m_EnableShelfTestTextOutputNum_RBVPV);

    m_SignalQualityFlag_PV.reset(new PVDelegateInImpl<std::int32_t>("SignalQualityFlag",PV_SignalQualityFlag_Reader));
	m_SignalQualityFlag_PV->setDescription("Flagged When Signal Quality decrease");
	m_SignalQualityFlag_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_SignalQualityFlag_PV);

    m_SignalQualityFlagLevel_PV.reset(new PVDelegateInImpl<double>("SignalQualityFlagLevel",PV_SignalQualityFlagLevel_Reader));
	m_SignalQualityFlagLevel_PV->setDescription("Quality Flag trigger level");
	m_SignalQualityFlagLevel_PV-> setScanType(scanType_t::interrupt,0);
	addChild(m_SignalQualityFlagLevel_PV);

    // Add state machine
    m_stateMachine.reset(new StateMachineImpl(true,
                                   switchOnFunction,
                                   switchOffFunction,
                                   std::bind(&HealthMonitSupImpl::onStart, this),
                                   stopFunction,
                                   recoverFunction,
                                   allowStateChangeFunction));
    addChild(m_stateMachine);
}


template<typename T>
timespec HealthMonitSupImpl<T>::getStartTimestamp() const
{
    return m_startTime;
}

template<typename T>
void HealthMonitSupImpl<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    m_startTimestampFunction = timestampDelegate;
}

template<typename T>
void HealthMonitSupImpl<T>::onStart()
{
    m_startTime = m_startTimestampFunction();
    //m_dataPV->setDecimation((std::uint32_t)(m_decimationPV->getValue()));
    m_onStartDelegate();
}

template class HealthMonitSupImpl<std::int32_t>;
template class HealthMonitSupImpl<double>;
template class HealthMonitSupImpl<std::vector<std::int8_t> >;
template class HealthMonitSupImpl<std::vector<std::uint8_t> >;
template class HealthMonitSupImpl<std::vector<std::int32_t> >;
template class HealthMonitSupImpl<std::vector<double> >;
template class HealthMonitSupImpl<std::string >;


}
