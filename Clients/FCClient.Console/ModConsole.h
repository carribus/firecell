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