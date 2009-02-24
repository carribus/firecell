#include "../common/game_objects/FCObjectFactory.h"
#include "Computer.h"

Computer::Computer(FCObject* parent)
: ComputerBase(parent, "Computer")
{
  m_fileSystem = FCObjectFactory::instance().createObject<FileSystem>(this);
  m_fileSystem->SetComputer(this);
}

///////////////////////////////////////////////////////////////////////

Computer::~Computer(void)
{
}

