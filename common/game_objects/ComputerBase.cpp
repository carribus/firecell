#include "ComputerBase.h"

ComputerBase::ComputerBase(FCObject* parent, const std::string& typeName)
: FCObject(typeName, parent)
, m_id(0)
, m_hddSizeMB(0)
, m_networkSpeedMBits(0)
{
}

///////////////////////////////////////////////////////////////////////

ComputerBase::~ComputerBase(void)
{
}

