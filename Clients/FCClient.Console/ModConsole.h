/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _MODCONSOLE_H_
#define _MODCONSOLE_H_

#include <string>
#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"
#include "IGameModule.h"

using namespace std;

class ModConsole : public IGameModule
{
  enum ConsoleState
  {
    Initialise = 1,
    Running,
    WaitingForResponse
  };

public:
  ModConsole(void);
  ~ModConsole(void);

  void RegisterSink(IGameModuleSink* pSink)                       { m_pSink = pSink; }
  void SetCharacterID(FCULONG character_id)                       { m_characterID = character_id; }
  void SetServerObject(FCServerObj* pServer)                      { m_pServer = pServer; }
  void QueueForAction();
  bool OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseConGetFSInfo(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseConGetFileList(PEPacket* pPkt, BaseSocket* pSocket);  
    bool OnResponseConCommand(PEPacket* pPkt, BaseSocket* pSocket);

private:

  bool IsLocalCommand(const char* cmd);
  void HandleLocalCommand(const char* cmd);

  IGameModuleSink*    m_pSink;
  FCULONG             m_characterID;
  FCServerObj*        m_pServer;
  ConsoleState        m_state;
  string              m_currentDir;
  string              m_dirSeperator;
  FCBYTE              m_fsStyle;
};

#endif//_MODCONSOLE_H_