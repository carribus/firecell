#ifndef _IGAMEMODULE_H_
#define _IGAMEMODULE_H_

class FCServerObj;
class PEPacket;
class BaseSocket;

struct IGameModule;

struct IGameModuleSink
{
  virtual void CloseModule(IGameModule* pModule) = 0;
};

struct IGameModule
{
  virtual void RegisterSink(IGameModuleSink* pSink) = 0;
  virtual void SetCharacterID(FCULONG character_id) = 0;
  virtual void SetServerObject(FCServerObj* pServer) = 0;
  virtual void QueueForAction() = 0;
  virtual bool OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket) = 0;
};

#endif//_IGAMEMODULE_H_