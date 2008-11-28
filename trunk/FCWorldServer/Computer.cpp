#include "Computer.h"

Computer::Computer(void)
: m_id(0)
, m_hddSizeMB(0)
, m_networkSpeedMBits(0)
{
  m_fileSystem.SetComputer(this);
}

Computer::~Computer(void)
{
}
