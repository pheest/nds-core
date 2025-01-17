/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/impl/portImpl.h"
#include "nds3/impl/pvBaseImpl.h"
#include "nds3/impl/factoryBaseImpl.h"
#include "nds3/impl/interfaceBaseImpl.h"

namespace nds
{


PortImpl::PortImpl(const std::string& name, const nodeType_t nodeType): NodeImpl(name, nodeType)
{
}

PortImpl::~PortImpl()
{
}



std::shared_ptr<PortImpl> PortImpl::getPort()
{
    return std::static_pointer_cast<PortImpl>(shared_from_this());
}

std::string PortImpl::buildFullNameFromPort(const FactoryBaseImpl& /* controlSystem */) const
{
    return "";
}

std::string PortImpl::buildFullExternalNameFromPort(const FactoryBaseImpl& /* controlSystem */) const
{
    return "";
}

void PortImpl::initialize(FactoryBaseImpl& controlSystem)
{
    if(m_pInterface.get() == 0)
    {
        m_pInterface.reset(controlSystem.getNewInterface(buildFullName(controlSystem)));
    }
    NodeImpl::initialize(controlSystem);

    m_pInterface->registrationTerminated();
}

void PortImpl::deinitialize()
{
    if(m_pInterface.get() == 0)
    {
        throw std::logic_error("deinitialize called on non initialized port");
    }
    NodeImpl::deinitialize();
}

void PortImpl::registerPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_pInterface->registerPV(pv);
}

void PortImpl::deregisterPV(std::shared_ptr<PVBaseImpl> pv)
{
    m_pInterface->deregisterPV(pv);
}

void PortImpl::registerReporter(reporter_t reporter){
  m_pInterface->registerReporter(reporter);
}

template<typename T>
void PortImpl::push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const T& value, const statusPV_t& status)
{
    m_pInterface->push(*(pv.get()), timestamp, value, status);
}

template void PortImpl::push<std::int32_t>(std::shared_ptr<PVBaseImpl>, const timespec&, const std::int32_t&, const statusPV_t&);
template void PortImpl::push<std::int64_t>(std::shared_ptr<PVBaseImpl>, const timespec&, const std::int64_t&, const statusPV_t&);
template void PortImpl::push<float>(std::shared_ptr<PVBaseImpl>, const timespec&, const float&, const statusPV_t&);
template void PortImpl::push<double>(std::shared_ptr<PVBaseImpl>, const timespec&, const double&, const statusPV_t&);
template void PortImpl::push<std::vector<bool> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<bool>&, const statusPV_t&);
template void PortImpl::push<std::vector<std::uint8_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::uint8_t>&, const statusPV_t&);
template void PortImpl::push<std::vector<std::uint16_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::uint16_t>&, const statusPV_t&);
template void PortImpl::push<std::vector<std::uint32_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::uint32_t>&, const statusPV_t&);
template void PortImpl::push<std::vector<std::int8_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::int8_t>&, const statusPV_t&);
template void PortImpl::push<std::vector<std::int16_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::int16_t>&, const statusPV_t&);
template void PortImpl::push<std::vector<std::int32_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::int32_t>&, const statusPV_t&);
template void PortImpl::push<std::vector<std::int64_t> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<std::int64_t>&, const statusPV_t&);
template void PortImpl::push<std::vector<float> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<float>&, const statusPV_t&);
template void PortImpl::push<std::vector<double> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<double>&, const statusPV_t&);
template void PortImpl::push<std::string >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::string&, const statusPV_t&);
template void PortImpl::push<timespec >(std::shared_ptr<PVBaseImpl>, const timespec&, const timespec&, const statusPV_t&);
template void PortImpl::push<std::vector<timespec> >(std::shared_ptr<PVBaseImpl>, const timespec&, const std::vector<timespec>&, const statusPV_t&);
template void PortImpl::push<timestamp_t>(std::shared_ptr<PVBaseImpl>, const timespec&, const timestamp_t&, const statusPV_t&);

}
