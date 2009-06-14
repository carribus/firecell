#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include <string>
#include "../common/game_objects/ComputerBase.h"
#include "FileSystem.h"
#include "../common/Database/FCDatabase.h"
#include "../common/fctypes.h"

using namespace std;

class Computer : public ComputerBase
{
public:
  Computer(FCObject* parent);
  ~Computer(void);

  FileSystem& GetFileSystem()             { return *m_fileSystem; }

private:

  FileSystem*      m_fileSystem;
};

#endif//_COMPUTER_H_