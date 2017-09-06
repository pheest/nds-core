/*
 * Nominal Device Support v.3 (NDS3)
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 *
 * By GMV & UPM
 */

#ifndef NDSHEALTHMONITORINGSUPIMPL_H
#define NDSHEALTHMONITORINGSUPIMPL_H

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
class HealthMonitSupImpl: public NodeImpl
{
public:
	HealthMonitSupImpl(const std::string& name,
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
			readerDouble_t PV_SignalQualityFlagLevel_Reader);


    /**
     * @brief Specifies the function to call to get the timestamp.
     *
     * The function is called only once at each start and its result
     * is stored in a local variable that can be retrieved with getStartTimestamp().
     *
     * If this function is not called then getTimestamp() is used to get the start time.
     *
     * @param timestampDelegate the function to call to get the start time
     */
    void setStartTimestampDelegate(getTimestampPlugin_t timestampDelegate);

    /**
     * @brief Returns the timestamp at start.
     *
     * This value is set by the state machine when the state switches to running.
     * If a timing plugin is active then the timestamp is taken from the plugin.
     *
     * @return the time when started.
     */
    timespec getStartTimestamp() const;

protected:

    /**
     * @brief Delegate function that retrieves the start time.
     *
     * By default points to BaseImpl::getTimestamp().
     *
     * Use setStartTimestampDelegate() to change the delegate function.
     */
    getTimestampPlugin_t m_startTimestampFunction;

    /**
     * @brief start time. via the delegate function declared in
     * m_startTimestampFunction.
     */
    timespec m_startTime;

    // PVs

    std::shared_ptr<PVDelegateInImpl<double> > m_DevicePower_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DeviceTemp_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DeviceVoltage_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_DeviceCurrent_PV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableSEU_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableSEU_RBVPV;


    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableMonitorDAQ_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableMonitorDAQ_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableShelfTest_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableShelfTest_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_ShelfTestType_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_ShelfTestType_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_VerboseShelfTest_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_VerboseShelfTest_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableShelfTestId_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableShelfTestId_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableShelfTestText_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableShelfTestText_RBVPV;

    std::shared_ptr<PVDelegateOutImpl<std::int32_t> > m_EnableShelfTestOutputNum_PV;
    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_EnableShelfTestTextOutputNum_RBVPV;

    std::shared_ptr<PVDelegateInImpl<std::int32_t> > m_SignalQualityFlag_PV;
    std::shared_ptr<PVDelegateInImpl<double> > m_SignalQualityFlagLevel_PV;

};

}
#endif // NDSHEALTHMONITORINGSUPIMPL_H

