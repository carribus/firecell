#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include <string>
#include "ItemProcessor.h"
#include "ItemOS.h"
#include "ItemMemory.h"
#include "FileSystem.h"
#include "../common/Database/FCDatabase.h"
#include "../common/fctypes.h"

using namespace std;

class Computer
{
public:
  Computer(void);
  ~Computer(void);

  FCULONG GetID()                         { return m_id; }
  string GetName()                        { return m_name; }
  FCUINT GetTotalHDD()                    { return m_hddSizeMB; }
  FCUINT GetNetworkSpeed()                { return m_networkSpeedMBits; }
  ItemProcessor& GetProcessor()           { return m_processor; }
  ItemOS& GetOS()                         { return m_os; }
  ItemMemory& GetMemory()                 { return m_memory; }
  FileSystem& GetFileSystem()             { return m_fileSystem; }
  void SetID(FCULONG id)                  { m_id = id; }
  void SetName(const string& name)        { m_name = name; }
  void SetHDDSize(FCUINT sizeMB)          { m_hddSizeMB = sizeMB; }
  void SetNetworkSpeed(FCUINT speedMBits) { m_networkSpeedMBits = speedMBits; }

private:

  FCULONG         m_id;
  string          m_name;
  FCUINT          m_hddSizeMB;
  FCUINT          m_networkSpeedMBits;
  ItemProcessor   m_processor;
  ItemOS          m_os;
  ItemMemory      m_memory;
  FileSystem      m_fileSystem;
};

#endif//_COMPUTER_H_