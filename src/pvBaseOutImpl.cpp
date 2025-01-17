/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include <cstring>

#include "nds3/impl/pvBaseOutImpl.h"
#include "nds3/impl/ndsFactoryImpl.h"
#include "nds3/impl/factoryBaseImpl.h"
#include "nds3/impl/nodeImpl.h"

namespace nds
{

PVBaseOutImpl::PVBaseOutImpl(const std::string& name, const outputPvType_t pvType): PVBaseImpl(name), m_pvType(pvType)
{
    defineCommand("subscribe", "subscribe destination source", 1, std::bind(&PVBaseOutImpl::commandSubscribeTo, this, std::placeholders::_1));
}

void PVBaseOutImpl::initialize(FactoryBaseImpl &controlSystem)
{
    PVBaseImpl::initialize(controlSystem);
    NdsFactoryImpl::getInstance().registerOutputPV(this);
}

void PVBaseOutImpl::deinitialize()
{
    NdsFactoryImpl::getInstance().deregisterOutputPV(this);
    PVBaseImpl::deinitialize();
}

void PVBaseOutImpl::subscribeTo(const std::string &inputPVName)
{
    NdsFactoryImpl::getInstance().subscribe(inputPVName, this);
}


void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::int32_t* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::int64_t* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, float* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, double* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<bool>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* pTimestamp, std::vector<std::uint8_t>* pValue) const
{
    // TODO
    // Epics calls this also for strings.
    // If we arrive here maybe we really wanted to call the string function.
    // This is as ugly as it can get: consider modifying this
    std::string temporaryValue;
    read(pTimestamp, &temporaryValue);
    pValue->resize(temporaryValue.size());
    ::memcpy(pValue->data(), temporaryValue.data(), temporaryValue.size());
}

void PVBaseOutImpl::read(timespec* /*pTimestamp*/, std::vector<std::uint16_t>* /*pValue*/) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /*pTimestamp*/, std::vector<std::uint32_t>* /*pValue*/) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* pTimestamp, std::vector<std::int8_t>* pValue) const
{
    // TODO
    // Epics calls this also for unsigned-int and strings.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    read(pTimestamp, (std::vector<std::uint8_t>*) pValue);
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<std::int16_t>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<std::int32_t>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<std::int64_t>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<float>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<double>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::string* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, timespec* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, std::vector<timespec>* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::read(timespec* /* pTimestamp */, timestamp_t* /* pValue */) const
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::int32_t& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::int64_t& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const float& /* value */)
{
    throw;
}


void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const double& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<bool>& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& timestamp, const std::vector<std::uint8_t>& value)
{
    // TODO
    // Epics calls this also for strings.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    const std::string temporaryString((char*)value.data(), value.size());
    write(timestamp, temporaryString);
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<std::uint16_t>& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<std::uint32_t>& /* value */)
{

}

void PVBaseOutImpl::write(const timespec& pTimestamp, const std::vector<std::int8_t>& value)
{
    // TODO
    // Epics calls this also for unsigned-int and strings.
    // If we arrive here maybe we really wanted to call the unsigned int function.
    // This is as ugly as it can get: consider modifying this
    write(pTimestamp, (const std::vector<std::uint8_t>&) value);
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<std::int16_t>& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<std::int32_t>& /* value */)
{
    throw  std::logic_error("Incorrect data type");;;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<std::int64_t>& /* value */)
{
    throw  std::logic_error("Incorrect data type");;;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<float>& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<double>& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::string& /* value */)
{
    throw;
}

void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const timespec& /* value */)
{
    throw;
}
void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const std::vector<timespec>& /* value */)
{
    throw;
}
void PVBaseOutImpl::write(const timespec& /* pTimestamp */, const timestamp_t& /* value */)
{
    throw;
}

dataDirection_t PVBaseOutImpl::getDataDirection() const
{
    return dataDirection_t::output;
}

parameters_t PVBaseOutImpl::commandSubscribeTo(const parameters_t &parameters)
{
    subscribeTo(parameters[0]);
    return parameters_t();
}

std::string PVBaseOutImpl::buildFullExternalName(const FactoryBaseImpl& controlSystem) const
{
    return buildFullExternalName(controlSystem, false);
}

std::string PVBaseOutImpl::buildFullExternalNameFromPort(const FactoryBaseImpl& controlSystem) const
{
    return buildFullExternalName(controlSystem, true);
}

std::string PVBaseOutImpl::buildFullExternalName(const FactoryBaseImpl& controlSystem, const bool bStopAtPort) const
{
    std::shared_ptr<NodeImpl> temporaryPointer = m_pParent.lock();
    if(temporaryPointer == 0)
    {
        return controlSystem.getSeparator(0) + controlSystem.getRootNodeName(m_externalName);
    }

    std::string name;
    switch(m_pvType)
    {
    case outputPvType_t::generic:
        name = controlSystem.getOutputPVName(m_externalName);
        break;
    case outputPvType_t::setLocalState:
        name = controlSystem.getStateMachineSetStateName(m_externalName);
        break;
    }

    if(name.empty())
    {
        if(bStopAtPort)
        {
            return temporaryPointer->buildFullExternalNameFromPort(controlSystem);
        }
        else
        {
            return temporaryPointer->buildFullExternalName(controlSystem);
        }
    }

    if(bStopAtPort)
    {
        return temporaryPointer->buildFullExternalNameFromPort(controlSystem) + controlSystem.getSeparator(m_nodeLevel) + name;
    }

    return temporaryPointer->buildFullExternalName(controlSystem) + controlSystem.getSeparator(m_nodeLevel) + name;
}


}
