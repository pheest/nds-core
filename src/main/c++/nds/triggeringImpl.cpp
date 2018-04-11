/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */


#include "nds3/definitions.h"
#include "nds3/impl/triggeringImpl.h"
#include "nds3/impl/stateMachineImpl.h"
#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvVariableOutImpl.h"
#include "nds3/impl/pvDelegateOutImpl.h"
#include "nds3/impl/pvDelegateInImpl.h"



namespace nds
{

template<typename T>
TriggeringImpl<T>::TriggeringImpl(
		 const std::string& name,
			writerDouble_t PV_DAQStartABSTime_Writer,
			readerDouble_t PV_DAQStartABSTime_Reader,
			writerDouble_t PV_DAQStartTimeDelay_Writer,
			readerDouble_t PV_DAQStartTimeDelay_Reader,
			writerInt32_t PV_TriggPeriodType_Writer,
			readerInt32_t PV_TriggPeriodType_Reader,
			writerInt32_t PV_EnableSWTrigg_Writer,
			readerInt32_t PV_EnableSWTrigg_Reader,
			writerDouble_t PV_TriggPeriod_Writer,
			readerDouble_t PV_TriggPeriod_Reader,
			writerInt32_t PV_TriggEventType_Writer,
			readerInt32_t PV_TriggEventType_Reader,
			writerInt32_t PV_LevelTrigg_Writer,
			readerInt32_t PV_LevelTrigg_Reader,
			writerInt32_t PV_EdgeTrigg_Writer,
			readerInt32_t PV_EdgeTrigg_Reader,
			writerInt32_t PV_CombineTrigg_Writer,
			readerInt32_t PV_CombineTrigg_Reader,
			writerInt32_t PV_TriggDelay_Writer,
			readerInt32_t PV_TriggDelay_Reader,
			writerInt32_t PV_PreTrigg_Writer,
			readerInt32_t PV_PreTrigg_Reader,
			writerDouble_t PV_SamplesToACQwhenTrigg_Writer,
			readerDouble_t PV_SamplesToACQwhenTrigg_Reader,
			writerDouble_t PV_SecondsToACQwhenTrigg_Writer,
			readerDouble_t PV_SecondsToACQwhenTrigg_Reader):
    NodeImpl(name, nodeType_t::dataSourceChannel)
{
	// Add the children PVs
    m_DAQStartABSTime_PV.reset(new PVDelegateOutImpl<double>("DAQStartABSTime",PV_DAQStartABSTime_Writer));
    m_DAQStartABSTime_PV->setDescription("Start DAQ after ABS Time");
	addChild(m_DAQStartABSTime_PV);

	m_DAQStartABSTime_RBVPV.reset(new PVDelegateInImpl<double>("DAQStartABSTime_RBV",PV_DAQStartABSTime_Reader));
	m_DAQStartABSTime_RBVPV->setDescription("Start DAQ after ABS Time ReadBack");
	m_DAQStartABSTime_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DAQStartABSTime_RBVPV);

    m_DAQStartTimeDelay_PV.reset(new PVDelegateOutImpl<double>("DAQStartTimeDelay",PV_DAQStartTimeDelay_Writer));
    m_DAQStartTimeDelay_PV->setDescription("Start DAQ after Time Delay");
	addChild(m_DAQStartTimeDelay_PV);

	m_DAQStartTimeDelay_RBVPV.reset(new PVDelegateInImpl<double>("DAQStartTimeDelay_RBV",PV_DAQStartTimeDelay_Reader));
	m_DAQStartTimeDelay_RBVPV->setDescription("Start DAQ after Time Delay ReadBack");
	m_DAQStartTimeDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_DAQStartTimeDelay_RBVPV);

    m_TriggPeriodType_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggPeriodType",PV_TriggPeriodType_Writer));
    m_TriggPeriodType_PV->setDescription("Trigger Period Type");
	addChild(m_TriggPeriodType_PV);

	m_TriggPeriodType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggPeriodType_RBV",PV_TriggPeriodType_Reader));
	m_TriggPeriodType_RBVPV->setDescription("Trigger period Type ReadBack");
	m_TriggPeriodType_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggPeriodType_RBVPV);

    m_EnableSWTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("EnableSWTrigg",PV_EnableSWTrigg_Writer));
    m_EnableSWTrigg_PV->setDescription("Enable SW Trigger");
	addChild(m_EnableSWTrigg_PV);

	m_EnableSWTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EnableSWTrigg_RBV",PV_EnableSWTrigg_Reader));
	m_EnableSWTrigg_RBVPV->setDescription("Enable SW Trigger ReadBack");
	m_EnableSWTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_EnableSWTrigg_RBVPV);

    m_TriggPeriod_PV.reset(new PVDelegateOutImpl<double>("TriggPeriod",PV_TriggPeriod_Writer));
    m_TriggPeriod_PV->setDescription("Trigger Period");
	addChild(m_TriggPeriod_PV);

	m_TriggPeriod_RBVPV.reset(new PVDelegateInImpl<double>("TriggPeriod_RBV",PV_TriggPeriod_Reader));
	m_TriggPeriod_RBVPV->setDescription("Trigger Period ReadBack");
	m_TriggPeriod_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggPeriod_RBVPV);

    m_TriggEventType_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggEventType",PV_TriggEventType_Writer));
    m_TriggEventType_PV->setDescription("Trigger Event Type");
	addChild(m_TriggEventType_PV);

	m_TriggEventType_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggEventType_RBV",PV_TriggEventType_Reader));
	m_TriggEventType_RBVPV->setDescription("Trigg Event Type ReadBack");
	m_TriggEventType_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggEventType_RBVPV);

    m_LevelTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("LevelTrigg",PV_LevelTrigg_Writer));
    m_LevelTrigg_PV->setDescription("Trigger Level");
	addChild(m_LevelTrigg_PV);

	m_LevelTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("LevelTrigg_RBV",PV_LevelTrigg_Reader));
	m_LevelTrigg_RBVPV->setDescription("Trigger Level ReadBack");
	m_LevelTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_LevelTrigg_RBVPV);

    m_EdgeTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("EdgeTrigg",PV_EdgeTrigg_Writer));
    m_EdgeTrigg_PV->setDescription("Trigger Edge");
	addChild(m_EdgeTrigg_PV);

	m_EdgeTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("EdgeTrigg_RBV",PV_EdgeTrigg_Reader));
	m_EdgeTrigg_RBVPV->setDescription("Trigger Edge ReadBack");
	m_EdgeTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_EdgeTrigg_RBVPV);

    m_CombineTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("CombineTrigg",PV_CombineTrigg_Writer));
    m_CombineTrigg_PV->setDescription("Combine Trigger ");
	addChild(m_CombineTrigg_PV);

	m_CombineTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("CombineTrigg_RBV",PV_CombineTrigg_Reader));
	m_CombineTrigg_RBVPV->setDescription("Combine Trigger ReadBack");
	m_CombineTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_CombineTrigg_RBVPV);

    m_TriggDelay_PV.reset(new PVDelegateOutImpl<std::int32_t>("TriggDelay",PV_TriggDelay_Writer));
    m_TriggDelay_PV->setDescription("Trigger Delay ");
	addChild(m_TriggDelay_PV);

	m_TriggDelay_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("TriggDelay_RBV",PV_TriggDelay_Reader));
	m_TriggDelay_RBVPV->setDescription("Trigger Delay ReadBack");
	m_TriggDelay_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_TriggDelay_RBVPV);

    m_PreTrigg_PV.reset(new PVDelegateOutImpl<std::int32_t>("PreTrigg",PV_PreTrigg_Writer));
    m_PreTrigg_PV->setDescription("PreTrigger ");
	addChild(m_PreTrigg_PV);

	m_PreTrigg_RBVPV.reset(new PVDelegateInImpl<std::int32_t>("PreTrigg_RBV",PV_PreTrigg_Reader));
	m_PreTrigg_RBVPV->setDescription("PreTrigger ReadBack");
	m_PreTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_PreTrigg_RBVPV);

    m_SamplesToACQwhenTrigg_PV.reset(new PVDelegateOutImpl<double>("SamplesToACQwhenTrigg",PV_SamplesToACQwhenTrigg_Writer));
    m_SamplesToACQwhenTrigg_PV->setDescription("Samples To ACQ when Trigger");
	addChild(m_SamplesToACQwhenTrigg_PV);

	m_SamplesToACQwhenTrigg_RBVPV.reset(new PVDelegateInImpl<double>("SamplesToACQwhenTrigg_RBV",PV_SamplesToACQwhenTrigg_Reader));
	m_SamplesToACQwhenTrigg_RBVPV->setDescription("Samples To ACQ when Trigger ReadBack");
	m_SamplesToACQwhenTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SamplesToACQwhenTrigg_RBVPV);

    m_SecondsToACQwhenTrigg_PV.reset(new PVDelegateOutImpl<double>("SecondsToACQwhenTrigg",PV_SecondsToACQwhenTrigg_Writer));
    m_SecondsToACQwhenTrigg_PV->setDescription("Samples To ACQ when Trigger");
	addChild(m_SecondsToACQwhenTrigg_PV);

	m_SecondsToACQwhenTrigg_RBVPV.reset(new PVDelegateInImpl<double>("SecondsToACQwhenTrigg_RBV",PV_SecondsToACQwhenTrigg_Reader));
	m_SecondsToACQwhenTrigg_RBVPV->setDescription("Samples To ACQ when Trigger ReadBack");
	m_SecondsToACQwhenTrigg_RBVPV->setScanType(scanType_t::interrupt, 0);
	addChild(m_SecondsToACQwhenTrigg_RBVPV);


}

template class TriggeringImpl<std::vector<timespec>>;


}
