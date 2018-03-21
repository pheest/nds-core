/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 *  By GMV & UPM
 */

#include "nds3/healthMonitoringSup.h"
#include "nds3/impl/healthMonitoringSupImpl.h"

namespace nds
{

HealthMonitSup::HealthMonitSup(): Node()
{
}

/**
 * @brief Constructs the data acquisition node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
HealthMonitSup::HealthMonitSup(  const std::string& name,
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
									writerInt32_t PV_SelfTestCodeResultEnable_Writer,
									readerString_t PV_SelfTestTextResult_Reader,
									readerInt32_t PV_SignalQualityFlag_Reader,
									writerDouble_t PV_SignalQualityFlagLevel_Writer):
    Node(std::shared_ptr<HealthMonitSupImpl >(new HealthMonitSupImpl(	name,
									switchOnFunction,
									switchOffFunction,
									startFunction,
									stopFunction,
									recoverFunction,
									allowStateChangeFunction,
									PV_DevicePower_Reader,
									PV_DeviceTemperature_Reader,
									PV_DeviceVoltage_Reader,
									PV_DeviceCurrent_Reader,
									PV_SEUEnable_Writer,
									PV_DAQEnable_Writer,
									PV_SelfTestEnable_Writer,
									PV_SelfTestType_Writer,
									PV_SelfTestVerboseEnable_Writer,
									PV_SelfTestIDEnable_Writer,
									PV_SelfTestTxtEnable_Writer,
									PV_SelfTestCodeResultEnable_Writer,
									PV_SelfTestTextResult_Reader,
									PV_SignalQualityFlag_Reader,
									PV_SignalQualityFlagLevel_Writer)))
{
}

HealthMonitSup::HealthMonitSup(const HealthMonitSup& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

HealthMonitSup& HealthMonitSup::operator=(const HealthMonitSup& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

void HealthMonitSup::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<HealthMonitSupImpl >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

void HealthMonitSup::push(const timespec& timestamp, const std::int32_t& data)
{
    std::static_pointer_cast<HealthMonitSupImpl >(m_pImplementation)->push(timestamp, data);
}

timespec HealthMonitSup::getStartTimestamp() const
{
    return std::static_pointer_cast<HealthMonitSupImpl >(m_pImplementation)->getStartTimestamp();
}


/**
 * ---------------------------------------------------
 * Getter functions
 * ---------------------------------------------------
 */

double HealthMonitSup::getDevicePower()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) ->getDevicePower();
}

double HealthMonitSup::getDeviceTemperature()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) ->getDeviceTemperature();
}

double HealthMonitSup::getDeviceVoltage()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) ->getDeviceVoltage();
}


double HealthMonitSup::getDeviceCurrent()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) ->getDeviceCurrent();
}


size_t HealthMonitSup::getSEUEnable()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSEUEnable();
}

size_t HealthMonitSup::getDAQMonitorEnable()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getDAQMonitorEnable();
}

size_t HealthMonitSup::getSelfTestEnable()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSelfTestEnable();
}

size_t HealthMonitSup::getSelfTestType()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSelfTestType();
}

size_t HealthMonitSup::getSelfTestVerboseEnable()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSelfTestVerboseEnable();
}

size_t HealthMonitSup::getSelfTestIDEnable()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSelfTestIDEnable();
}

size_t HealthMonitSup::getSelfTestTextEnable()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSelfTestTextEnable();
}

size_t HealthMonitSup::getSelfTestCodeResultEnable()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSelfTestCodeResultEnable();
}

size_t HealthMonitSup::getSignalQualityFlag()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSignalQualityFlag();
}

double HealthMonitSup::getSignalQualityFlagLevel()
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> getSignalQualityFlagLevel();
}

/**
 * ---------------------------------------------------
 * Setter functions
 * ---------------------------------------------------
 */

void HealthMonitSup::setDevicePower(const timespec& timestamp, const double& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setDevicePower(timestamp, value);
}

void HealthMonitSup::setDeviceTemperature(const timespec& timestamp, const double& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setDeviceTemperature(timestamp, value);
}

void HealthMonitSup::setDeviceVoltage(const timespec& timestamp, const double& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setDeviceVoltage(timestamp, value);
}

void HealthMonitSup::setDeviceCurrent(const timespec& timestamp, const double& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setDeviceCurrent(timestamp, value);
}

void HealthMonitSup::setSEUEnable(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSEUEnable(timestamp, value);
}

void HealthMonitSup::setDAQMonitorEnable(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setDAQMonitorEnable(timestamp, value);
}

void HealthMonitSup::setSelfTestEnable(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSelfTestEnable(timestamp, value);
}


void HealthMonitSup::setSelfTestType(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSelfTestType(timestamp, value);
}

void HealthMonitSup::setSelfTestVerboseEnable(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSelfTestVerboseEnable(timestamp, value);
}

void HealthMonitSup::setSelfTestIDEnable(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSelfTestIDEnable(timestamp, value);
}

void HealthMonitSup::setSelfTestTextEnable(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSelfTestTextEnable(timestamp, value);
}

void HealthMonitSup::setSelfTestCodeResultEnable(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSelfTestCodeResultEnable(timestamp, value);
}

void HealthMonitSup::setSignalQualityFlag(const timespec& timestamp, const std::int32_t& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSignalQualityFlag(timestamp, value);
}

void HealthMonitSup::setSignalQualityFlagLevel(const timespec& timestamp, const double& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSignalQualityFlagLevel(timestamp, value);
}

void HealthMonitSup::setSelfTextTxtResult(const timespec& timestamp, const std::string& value)
{
	return std::static_pointer_cast<HealthMonitSupImpl> (m_pImplementation) -> setSelfTextTxtResult(timestamp, value);
}

}
