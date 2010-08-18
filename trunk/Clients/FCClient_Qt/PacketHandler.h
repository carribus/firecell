#ifndef _PACKETHANDLER_H_
#define _PACKETHANDLER_H_

#include <QObject>
#include "../../common/fctypes.h"
#include "../../common/PEPacket.h"
class FCApp;

class PacketHandler : public QObject
{
  Q_OBJECT

public:
  PacketHandler(FCApp* pApp);
  ~PacketHandler(void);

  void onCommand(PEPacket* pPkt);

  void onResponse(PEPacket* pPkt);
    bool onResponseServerInfo(PEPacket* pPkt);
    bool onResponseLogin(PEPacket* pPkt);
    bool onResponseGetCharacters(PEPacket* pPkt);
    bool onResponseSelectCharacter(PEPacket* pPkt);
    bool onResponseCharacterItemsRequest(PEPacket* pPkt);
    bool onResponseCharacterAssetsRequest(PEPacket* pPkt);
    bool onResponseCharacterMissionsRequest(PEPacket* pPkt);
    bool onResponseSoftwareInstallRequest(PEPacket* pPkt);
    bool onResponseSoftwareUninstallRequest(PEPacket* pPkt);
    bool onResponseSoftwareNetworkPortEnableRequest(PEPacket* pPkt);
    bool onResponseActivateSoftware(PEPacket* pPkt);
    bool onResponseForumCategories(PEPacket* pPkt);
    bool onResponseForumThreads(PEPacket* pPkt);

  void onError(PEPacket* pPkt);

signals:

  void serverInfoReceived(unsigned char verMajor, unsigned char verMinor);

private:

  template <class TTargetStruct>
  void getPacketData(PEPacket* pPkt, TTargetStruct& target)
  {
    size_t dataLen;

    pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
    pPkt->GetField("data", &target, dataLen);
  }

  template <class TTargetStruct>
  void getDynamicPacketData(PEPacket* pPkt, TTargetStruct*& pTarget)
  {
    size_t dataLen;

    pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
    pTarget = (TTargetStruct*) new FCBYTE[ dataLen ];
    pPkt->GetField("data", pTarget, dataLen);
  }

  FCApp*          m_pApp;
};

#endif//_PACKETHANDLER_H_