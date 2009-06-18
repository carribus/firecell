#ifndef _MODBANK_H_
#define _MODBANK_H_

#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"
#include "IGameModule.h"

class ModBank : public IGameModule
{
  enum BankState
  {
    Initialise = 1,
    Running,
    WaitingForResponse,
    NeedAuth
  };

public:
  ModBank(void);
  ~ModBank(void);

  void RegisterSink(IGameModuleSink* pSink)                       { m_pSink = pSink; }
  void SetCharacterID(FCULONG character_id)                       { m_characterID = character_id; }
  void SetServerObject(FCServerObj* pServer)                      { m_pServer = pServer; }
  void QueueForAction();
  bool OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseBankConnect(PEPacket* pPkt, BaseSocket* pSocket);

private:

  IGameModuleSink*    m_pSink;
  FCULONG             m_characterID;
  FCServerObj*        m_pServer;
  BankState           m_state;
};

#endif//_MODBANK_H_