/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/impl/pvVariableInImpl.h"
#include "nds3/impl/pvBaseOutImpl.h"

namespace nds
{

/*
 * Constructor
 *
 *************/
template <typename T>
PVVariableInImpl<T>::PVVariableInImpl(const std::string& name, const inputPvType_t pvType): PVBaseInImpl(name, pvType), m_value()
{
    m_timestamp.tv_sec = 0;
    m_timestamp.tv_nsec = 0;

}


/*
 * Called by the control system to read the stored value
 *
 *******************************************************/
template <typename T>
void PVVariableInImpl<T>::read(timespec* pTimestamp, T* pValue) const
{
    std::unique_lock<std::mutex> lock(m_pvMutex);
    *pValue = m_value;
    *pTimestamp = m_timestamp;
}


/*
 * Store a new value and its timestamp in the PV
 *
 ***********************************************/
template <typename T>
void PVVariableInImpl<T>::setValue(const timespec& timestamp, const T& value)
{
    {
        // Store the value
        //////////////////
        std::unique_lock<std::mutex> lock(m_pvMutex);
        m_value =value;
        m_timestamp = timestamp;
    }

    // Push the value to the outputs
    ////////////////////////////////
    std::lock_guard<nds::recursive_mutex_counter> lock(m_lockSubscribersList);
    for(subscribersList_t::iterator scanOutputs(m_subscriberOutputPVs.begin()), endOutputs(m_subscriberOutputPVs.end());
        scanOutputs != endOutputs;
        ++scanOutputs)
    {
        (*scanOutputs)->write(timestamp, value);
    }

    //If this function was the first to lock the recursive mutex, carry out unsubscription.
    if(m_lockSubscribersList.get_count()==1){
        while(!m_unsubscribeOutputPVs.empty()){
            m_subscriberOutputPVs.erase(m_unsubscribeOutputPVs.front());
            m_unsubscribeOutputPVs.pop();
        }
    }
}


/*
 * Store a new value in the PV
 *
 *****************************/
template <typename T>
void PVVariableInImpl<T>::setValue(const T& value)
{
    setValue(getTimestamp(), value);
}


/*
 * Return the PV's data type
 *
 ***************************/
template <typename T>
dataType_t PVVariableInImpl<T>::getDataType() const
{
    return getDataTypeForCPPType<T>();
}


// Instantiate all the needed data types
////////////////////////////////////////
template class PVVariableInImpl<std::int32_t>;
template class PVVariableInImpl<std::int64_t>;
template class PVVariableInImpl<float>;
template class PVVariableInImpl<double>;
template class PVVariableInImpl<std::vector<bool> >;
template class PVVariableInImpl<std::vector<std::uint8_t> >;
template class PVVariableInImpl<std::vector<std::uint16_t> >;
template class PVVariableInImpl<std::vector<std::uint32_t> >;
template class PVVariableInImpl<std::vector<std::int8_t> >;
template class PVVariableInImpl<std::vector<std::int16_t> >;
template class PVVariableInImpl<std::vector<std::int32_t> >;
template class PVVariableInImpl<std::vector<std::int64_t> >;
template class PVVariableInImpl<std::vector<float> >;
template class PVVariableInImpl<std::vector<double> >;
template class PVVariableInImpl<std::string>;
template class PVVariableInImpl<timespec>;
template class PVVariableInImpl<std::vector<timespec>>;
template class PVVariableInImpl<timestamp_t>;


}
