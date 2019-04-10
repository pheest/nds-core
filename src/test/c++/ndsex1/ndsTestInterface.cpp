#include "../include/ndsTestInterface.h"

namespace nds
{

namespace tests
{

static std::map<std::string, TestControlSystemInterfaceImpl*> m_interfacesMap;
static std::mutex m_lockInterfacesMap;


TestControlSystemInterfaceImpl::TestControlSystemInterfaceImpl(const std::string &fullName):
    m_name(fullName)
{
    std::lock_guard<std::mutex> lock(m_lockInterfacesMap);
    if(m_interfacesMap.find(fullName) != m_interfacesMap.end())
    {
        throw std::logic_error("Interface with the same name already allocated. This should not happen");
    }
    m_interfacesMap[fullName] = this;
}

TestControlSystemInterfaceImpl::~TestControlSystemInterfaceImpl()
{
    std::lock_guard<std::mutex> lock(m_lockInterfacesMap);
    m_interfacesMap.erase(m_name);

}

TestControlSystemInterfaceImpl* TestControlSystemInterfaceImpl::getInstance(const std::string &fullName)
{
    std::lock_guard<std::mutex> lock(m_lockInterfacesMap);

    std::map<std::string, TestControlSystemInterfaceImpl*>::const_iterator findInterface = m_interfacesMap.find(fullName);
    if(findInterface == m_interfacesMap.end())
    {
        return 0;
    }
    return findInterface->second;

}

void TestControlSystemInterfaceImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_registeredPVs[pv->getFullExternalName()] = pv.get();
    //Added for testing purposes.
    //dataType_t dataType = pv->getDataType();

}

void TestControlSystemInterfaceImpl::deregisterPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_registeredPVs.erase(pv->getFullExternalName());
}

void TestControlSystemInterfaceImpl::registrationTerminated()
{

}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::int32_t& value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedInt32, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const double& value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedDouble, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<bool> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorBool, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint8_t> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorUint8, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint16_t> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorUint16, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::uint32_t> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorUint32, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int8_t> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorInt8, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int16_t> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorInt16, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<std::int32_t> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorInt32, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<double> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorDouble, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::string & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedString, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const timespec & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedTimespec, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const std::vector<timespec> & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedVectorTimespec, timestamp, value);
}

void TestControlSystemInterfaceImpl::push(const PVBaseImpl& pv, const timespec& timestamp, const timestamp_t & value, const statusPV_t&)
{
    storePushedData(pv.getFullExternalName(), m_pushedTimestamp, timestamp, value);
}


template<typename T>
void TestControlSystemInterfaceImpl::readCSValue(const std::string& pvName, timespec* pTimestamp, T* pValue)
{
    registeredPVs_t::iterator findPV = m_registeredPVs.find(pvName);
    if(findPV == m_registeredPVs.end())
    {
        throw std::runtime_error("PV not found");
    }
    findPV->second->read(pTimestamp, pValue);
}

template void TestControlSystemInterfaceImpl::readCSValue<std::int32_t>(const std::string& pvName, timespec* timestamp, std::int32_t* value);
template void TestControlSystemInterfaceImpl::readCSValue<double>(const std::string& pvName, timespec* timestamp, double* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<bool> >(const std::string& pvName, timespec* timestamp, std::vector<bool>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::uint8_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::uint8_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::uint16_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::uint16_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::uint32_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::uint32_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::int8_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::int8_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::int16_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::int16_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<std::int32_t> >(const std::string& pvName, timespec* timestamp, std::vector<std::int32_t>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<double> >(const std::string& pvName, timespec* timestamp, std::vector<double>* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::string>(const std::string& pvName, timespec* timestamp, std::string* value);
template void TestControlSystemInterfaceImpl::readCSValue<timespec>(const std::string& pvName, timespec* timestamp, timespec* value);
template void TestControlSystemInterfaceImpl::readCSValue<std::vector<timespec>>(const std::string& pvName, timespec* timestamp, std::vector<timespec>* value);
template void TestControlSystemInterfaceImpl::readCSValue<timestamp_t>(const std::string& pvName, timespec* timestamp, timestamp_t* value);


template<typename T>
void TestControlSystemInterfaceImpl::writeCSValue(const std::string& pvName, const timespec& timestamp, const T& value)
{
    registeredPVs_t::iterator findPV = m_registeredPVs.find(pvName);
    if(findPV == m_registeredPVs.end())
    {
        throw std::runtime_error("PV not found");
    }
    findPV->second->write(timestamp, value);
}

template void TestControlSystemInterfaceImpl::writeCSValue<std::int32_t>(const std::string& pvName, const timespec& timestamp, const std::int32_t& value);
template void TestControlSystemInterfaceImpl::writeCSValue<double>(const std::string& pvName, const timespec& timestamp, const double& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<bool> >(const std::string& pvName, const timespec& timestamp, const std::vector<bool>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::uint8_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::uint8_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::uint16_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::uint16_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::uint32_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::uint32_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::int8_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::int8_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::int16_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::int16_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<std::int32_t> >(const std::string& pvName, const timespec& timestamp, const std::vector<std::int32_t>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<double> >(const std::string& pvName, const timespec& timestamp, const std::vector<double>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::string>(const std::string& pvName, const timespec& timestamp, const std::string& value);
template void TestControlSystemInterfaceImpl::writeCSValue<timespec>(const std::string& pvName, const timespec& timestamp, const timespec& value);
template void TestControlSystemInterfaceImpl::writeCSValue<std::vector<timespec>>(const std::string& pvName, const timespec& timestamp, const std::vector<timespec>& value);
template void TestControlSystemInterfaceImpl::writeCSValue<timestamp_t>(const std::string& pvName, const timespec& timestamp, const timestamp_t& value);


void TestControlSystemInterfaceImpl::getPushedInt32(const std::string& pvName, const timespec*& pTime, const std::int32_t*& pValue)
{
    getPushedData(pvName, m_pushedInt32, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedDouble(const std::string& pvName, const timespec*& pTime, const double*& pValue)
{
    return getPushedData(pvName, m_pushedDouble, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorBool(const std::string& pvName, const timespec*& pTime, const std::vector<bool>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorBool, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorUint8(const std::string& pvName, const timespec*& pTime, const std::vector<std::uint8_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorUint8, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorUint16(const std::string& pvName, const timespec*& pTime, const std::vector<std::uint16_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorUint16, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorUint32(const std::string& pvName, const timespec*& pTime, const std::vector<std::uint32_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorUint32, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorInt8(const std::string& pvName, const timespec*& pTime, const std::vector<std::int8_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorInt8, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorInt16(const std::string& pvName, const timespec*& pTime, const std::vector<std::int16_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorInt16, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorInt32(const std::string& pvName, const timespec*& pTime, const std::vector<std::int32_t>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorInt32, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorDouble(const std::string& pvName, const timespec*& pTime, const std::vector<double>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorDouble, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedString(const std::string& pvName, const timespec*& pTime, const std::string*& pValue)
{
    return getPushedData(pvName, m_pushedString, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedTimespec(const std::string& pvName, const timespec*& pTime, const timespec*& pValue)
{
    return getPushedData(pvName, m_pushedTimespec, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedVectorTimespec(const std::string& pvName, const timespec*& pTime, const std::vector<timespec>*& pValue)
{
    return getPushedData(pvName, m_pushedVectorTimespec, pTime, pValue);
}

void TestControlSystemInterfaceImpl::getPushedTimestamp(const std::string& pvName, const timespec*& pTime, const timestamp_t*& pValue)
{
    return getPushedData(pvName, m_pushedTimestamp, pTime, pValue);
}

void TestControlSystemInterfaceImpl::registerReporter(reporter_t reporter_in)
{
  reporter=reporter_in;
}
void TestControlSystemInterfaceImpl::report(FILE* file , int details)
{
  reporter(file,details);
}

}

}
