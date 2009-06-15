#include "ComputerBase.h"

ComputerBase::ComputerBase(FCObject* parent, const std::string& typeName)
: FCObject(typeName, parent)
, m_id(0)
, m_hddSizeMB(0)
, m_networkSpeedMBits(0)
, m_usageCPU(0)
, m_usageMem(0)
{
}

///////////////////////////////////////////////////////////////////////

ComputerBase::~ComputerBase(void)
{
}

///////////////////////////////////////////////////////////////////////

size_t ComputerBase::AddProcess(ItemSoftware* pSoftware)
{
  m_usageCPU += pSoftware->GetCPUCost();
  m_usageMem += pSoftware->GetMemCost();

  m_processes.push_back(pSoftware);
  return m_processes.size();
}

///////////////////////////////////////////////////////////////////////

FCSHORT ComputerBase::GetAvailableCPU()
{
  FCSHORT maxMem = m_processor.GetCoreSpeed() * m_processor.GetCoreCount();

  return (maxMem - m_usageCPU);
}

///////////////////////////////////////////////////////////////////////

FCULONG ComputerBase::GetAvailableMemory()
{
  return ( m_memory.GetMemorySize() - m_usageMem );
}


