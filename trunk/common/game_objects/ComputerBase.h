#ifndef _COMPUTERBASE_H_
#define _COMPUTERBASE_H_

#include <list>
#include <string>
#include "FCObject.h"
#include "ItemProcessor.h"
#include "ItemOS.h"
#include "ItemMemory.h"
#include "NetworkPorts.h"
#include "../fctypes.h"
#include "../fcconstants.h"

using namespace std;

class ComputerBase : public FCObject
{
public:
  ComputerBase(FCObject* parent, const std::string& typeName = "ComputerBase");
  ~ComputerBase(void);

  FCULONG GetID()                         { return m_id; }
  std::string GetName()                   { return m_name; }
  FCUINT GetTotalHDD()                    { return m_hddSizeMB; }
  FCUINT GetNetworkSpeed()                { return m_networkSpeedMBits; }
  ItemProcessor& GetProcessor()           { return m_processor; }
  ItemOS& GetOS()                         { return m_os; }
  ItemMemory& GetMemory()                 { return m_memory; }
	NetworkPorts& GetNetworkPorts()					{ return m_ports; }
  void SetID(FCULONG id)                  { m_id = id; }
  void SetName(const std::string& name)   { m_name = name; }
  void SetHDDSize(FCUINT sizeMB)          { m_hddSizeMB = sizeMB; }
  void SetNetworkSpeed(FCUINT speedMBits) { m_networkSpeedMBits = speedMBits; }

  size_t AddProcess(DesktopOptionType type, FCULONG cpuCost, FCULONG memCost);
  FCSHORT GetAvailableCPU();
  FCULONG GetAvailableMemory();

private:

  FCULONG         m_id;
  std::string     m_name;
  FCUINT          m_hddSizeMB;
  FCUINT          m_networkSpeedMBits;
  ItemProcessor   m_processor;
  ItemOS          m_os;
  ItemMemory      m_memory;
	NetworkPorts		m_ports;
  FCSHORT         m_usageCPU;
  FCULONG         m_usageMem;

  struct stProcess
  {
    DesktopOptionType optionType;
    FCULONG memCost;
    FCULONG cpuCost;
  };
  std::list<stProcess>  m_processes;
};

#endif//_COMPUTERBASE_H_