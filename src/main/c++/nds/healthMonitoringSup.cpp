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

template <typename T>
HealthMonitSup<T>::HealthMonitSup(): Node()
{
}

/**
 * @brief Constructs the data acquisition node.
 *
 * @param name        the node name
 * @param maxElements if the data type is an array, then indicated
 *                    the maximum size (in elements) of the acquired array
 */
template <typename T>
HealthMonitSup<T>::HealthMonitSup(  const std::string& name,
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
									readerInt32_t PV_SEUEnable_Reader,
									writerInt32_t PV_DAQEnable_Writer,
									readerInt32_t PV_DAQEnable_Reader,
									writerInt32_t PV_SelfTestEnable_Writer,
									readerInt32_t PV_SelfTestEnable_Reader,
									writerInt32_t PV_SelfTestType_Writer,
									readerInt32_t PV_SelfTestType_Reader,
									writerInt32_t PV_SelfTestVerboseEnable_Writer,
									readerInt32_t PV_SelfTestVerboseEnable_Reader,
									writerInt32_t PV_SelfTestIDEnable_Writer,
									readerInt32_t PV_SelfTestIDEnable_Reader,
									writerInt32_t PV_SelfTestTxtEnable_Writer,
									readerInt32_t PV_SelfTestTxtEnable_Reader,
									readerInt32_t PV_SignalQualityFlag_Reader,
									writerDouble_t PV_SignalQualityFlagLevel_Writer):
    Node(std::shared_ptr<HealthMonitSupImpl<T> >(new HealthMonitSupImpl<T>(	name,
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
																			PV_SEUEnable_Reader,
																			PV_DAQEnable_Writer,
																			PV_DAQEnable_Reader,
																			PV_SelfTestEnable_Writer,
																			PV_SelfTestEnable_Reader,
																			PV_SelfTestType_Writer,
																			PV_SelfTestType_Reader,
																			PV_SelfTestVerboseEnable_Writer,
																			PV_SelfTestVerboseEnable_Reader,
																			PV_SelfTestIDEnable_Writer,
																			PV_SelfTestIDEnable_Reader,
																			PV_SelfTestTxtEnable_Writer,
																			PV_SelfTestTxtEnable_Reader,
																			PV_SignalQualityFlag_Reader,
																			PV_SignalQualityFlagLevel_Writer)))
{
}

template <typename T>
HealthMonitSup<T>::HealthMonitSup(const HealthMonitSup<T>& right): Node(std::static_pointer_cast<NodeImpl>(right.m_pImplementation))
{
}

template <typename T>
HealthMonitSup<T>& HealthMonitSup<T>::operator=(const HealthMonitSup<T>& right)
{
    m_pImplementation = right.m_pImplementation;
    return *this;
}

template <typename T>
void HealthMonitSup<T>::setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate)
{
    std::static_pointer_cast<HealthMonitSupImpl<T> >(m_pImplementation)->setStartTimestampDelegate(timestampDelegate);
}

template <typename T>
timespec HealthMonitSup<T>::getStartTimestamp() const
{
    return std::static_pointer_cast<HealthMonitSupImpl<T> >(m_pImplementation)->getStartTimestamp();
}

template class HealthMonitSup<std::int32_t>;
template class HealthMonitSup<double>;
template class HealthMonitSup<std::vector<std::int8_t> >;
template class HealthMonitSup<std::vector<std::uint8_t> >;
template class HealthMonitSup<std::vector<std::int32_t> >;
template class HealthMonitSup<std::vector<double> >;
template class HealthMonitSup<std::string >;


}
