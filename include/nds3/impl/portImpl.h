/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#ifndef NDSPORTIMPL_H
#define NDSPORTIMPL_H

#include "nds3/impl/nodeImpl.h"

namespace nds
{

class FactoryBaseImpl;
class InterfaceBaseImpl;
class PVBaseImpl;


/**
 * @brief Node that holds an ASYN port
 *
 * When initialize() is called then registers the port and all the children AsynPVBase
 * (also the ones not directly owned but owned by its children).
 */
class PortImpl: public NodeImpl
{
public:
    /**
     * @brief Construct an AsynPort.
        *
        * @param name
        * @param nodeType
        */
    PortImpl(const std::string& name, const nodeType_t nodeType);

    virtual ~PortImpl();

    virtual void initialize(FactoryBaseImpl& controlSystem);

    virtual void deinitialize();

    /**
     * @brief Return a pointer to this object.
     * @return a pointer to this object
     */
    virtual std::shared_ptr<PortImpl> getPort();

    void registerReporter(reporter_t reporter);

    void registerPV(std::shared_ptr<PVBaseImpl> pv);

    void deregisterPV(std::shared_ptr<PVBaseImpl> pv);

    template<typename T>
    void push(std::shared_ptr<PVBaseImpl> pv, const timespec& timestamp, const T& value, const statusPV_t& status = statusPV_t::success);

    virtual std::string buildFullNameFromPort(const FactoryBaseImpl& controlSystem) const;
    virtual std::string buildFullExternalNameFromPort(const FactoryBaseImpl& controlSystem) const;

private:
    std::unique_ptr<InterfaceBaseImpl> m_pInterface;

    typedef std::map<int, std::shared_ptr<PVBaseImpl> > tRecords;
    tRecords m_records;
};

}
#endif // NDSPORTIMPL_H
