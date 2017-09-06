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
HealthMonitSup<T>::HealthMonitSup(
				const std::string& name,
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
    Node(std::shared_ptr<HealthMonitSupImpl<T> >(new HealthMonitSupImpl<T>(	name,
																			PV_DevicePower_Reader,
																			PV_DeviceTemp_Reader,
																			PV_DeviceVoltage_Reader,
																			PV_DeviceCurrent_Reader,
																			PV_EnableSEU_Writer,
																			PV_EnableSEU_Reader,
																			PV_EnableMonitorDAQ_Writer,
																			PV_EnableMonitorDAQ_Reader,
																			PV_EnableShelfTest_Writer,
																			PV_EnableShelfTest_Reader,
																			PV_ShelfTestType_Writer,
																			PV_ShelfTestType_Reader,
																			PV_VerboseShelfTest_Writer,
																			PV_VerboseShelfTest_Reader,
																			PV_EnableShelfTestId_Writer,
																			PV_EnableShelfTestId_Reader,
																			PV_EnableShelfTestText_Writer,
																			PV_EnableShelfTestText_Reader,
																			PV_SignalQualityFlag_Reader,
																			PV_SignalQualityFlagLevel_Reader)))
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
