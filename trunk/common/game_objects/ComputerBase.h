#ifndef _COMPUTERBASE_H_
#define _COMPUTERBASE_H_

#include <string>
#include "ItemProcessor.h"
#include "ItemOS.h"
#include "ItemMemory.h"
#include "../fctypes.h"

using namespace std;

class ComputerBase
{
public:
  ComputerBase(void);
  ~ComputerBase(void);

  FCULONG GetID()                         { return m_id; }
  std::string GetName()                   { return m_name; }
  FCUINT GetTotalHDD()                    { return m_hddSizeMB; }
  FCUINT GetNetworkSpeed()                { return m_networkSpeedMBits; }
  ItemProcessor& GetProcessor()           { return m_processor; }
  ItemOS& GetOS()                         { return m_os; }
  ItemMemory& GetMemory()                 { return m_memory; }
  void SetID(FCULONG id)                  { m_id = id; }
  void SetName(const std::string& name)   { m_name = name; }
  void SetHDDSize(FCUINT sizeMB)          { m_hddSizeMB = sizeMB; }
  void SetNetworkSpeed(FCUINT speedMBits) { m_networkSpeedMBits = speedMBits; }

private:

  FCULONG         m_id;
  std::string     m_name;
  FCUINT          m_hddSizeMB;
  FCUINT          m_networkSpeedMBits;
  ItemProcessor   m_processor;
  ItemOS          m_os;
  ItemMemory      m_memory;
};

#endif//_COMPUTERBASE_H_