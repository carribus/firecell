#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include <string>
#include "../common/fctypes.h"

using namespace std;

class Computer
{
public:
  Computer(void);
  ~Computer(void);

  string GetName()                        { return m_name; }
  FCUINT GetTotalRAM()                    { return m_memorySizeMB; }
  FCUINT GetTotalHDD()                    { return m_hddSizeMB; }
  FCUINT GetNetworkSpeed()                { return m_networkSpeedMBits; }
  OperatingSystem& GetOS()                { return m_os; }
  Processor& GetProcessor()               { return m_processor; }
  FileSystem& GetFileSystem()             { return m_fileSystem; }

  void SetName(const string& name)        { m_name = name; }
  void SetHDDSize(FCUINT sizeMB)          { m_hddSizeMB = sizeMB; }
  void SetRAMSize(FCUINT sizeMB)          { m_memorySizeMB = sizeMB; }
  void SetNetworkSpeed(FCUINT speedMBits) { m_networkSpeedMBits = speedMBits; }

private:

  FCUINT          m_id;
  string          m_name;
  FCUINT          m_memorySizeMB;           // number of megabytes of RAM
  FCUINT          m_hddSizeMB;
  FCUINT          m_networkSpeedMBits;
  OperatingSystem m_os;
  Processor       m_processor;
  FileSystem      m_fileSystem;
};

#endif//_COMPUTER_H_