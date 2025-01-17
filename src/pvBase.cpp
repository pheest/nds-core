/*
 * Nominal Device Support v3 (NDS3)
 *
 * Copyright (c) 2015 Cosylab d.d.
 *
 * For more information about the license please refer to the license.txt
 * file included in the distribution.
 */

#include "nds3/pvBase.h"
#include "nds3/impl/pvBaseImpl.h"

namespace nds
{

/*
 * Default constructor.
 *
 **********************/
PVBase::PVBase()
{

}


/*
 * Construct a PV with a reference to an already existing implementation.
 *
 ************************************************************************/
PVBase::PVBase(std::shared_ptr<PVBaseImpl> pvImpl): Base(std::static_pointer_cast<BaseImpl>(pvImpl))
{

}


/*
 * Set the enumeration strings
 *
 *****************************/
void PVBase::setEnumeration(const enumerationStrings_t &enumerations)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setEnumeration(enumerations);
}

/*
 * Specifies if the PV has to be processed during the device initialization
 *
 **************************************************************************/
void PVBase::processAtInit(const bool bProcessAtInit)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->processAtInit(bProcessAtInit);
}



/*
 * Set the description
 *
 *********************/
void PVBase::setDescription(const std::string& description)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setDescription(description);
}


/*
 * Set the units
 *
 ***************/
void PVBase::setUnits(const std::string &units)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setUnits(units);
}


/*
 * Set the scan type
 *
 *******************/
void PVBase::setScanType(const scanType_t scanType, const double periodSeconds)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setScanType(scanType, periodSeconds);
}


/*
 * Set the maximum number of elements that can be written or read from the PV
 *
 ****************************************************************************/
void PVBase::setMaxElements(const size_t maxElements)
{
    std::static_pointer_cast<PVBaseImpl>(m_pImplementation)->setMaxElements(maxElements);
}

}
