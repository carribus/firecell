#include "../common/PEPacket.h"
#include "../common/PEPacketHelper.h"
#include "../common/PacketDispatcher.h"
#include "chat_comms.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendChatRoomList(vector< ChatRoom >& chatRooms, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
  __FCPKT_CHAT_LIST_ROOMS_RESP* d = NULL;
  std::vector< ChatRoom >::iterator it = chatRooms.begin();
  std::vector< ChatRoom >::iterator limit = chatRooms.end();
  size_t pktSize = sizeof(__FCPKT_CHAT_LIST_ROOMS_RESP);
  size_t numRooms = chatRooms.size(), index = 0;

  // calculate the size of the payload and allocate it
  pktSize += (numRooms-1) * sizeof(__FCPKT_CHAT_LIST_ROOMS_RESP::ChatRoom);

  // create the packet
  d = (__FCPKT_CHAT_LIST_ROOMS_RESP*)new FCBYTE[pktSize];
  d->numRooms = numRooms;
  for ( ; it != limit; ++it, index++ )
  {
    d->rooms[index].id = it->getID();
    d->rooms[index].is_official = it->isOfficial();
    d->rooms[index].is_private = it->isPrivate();
    d->rooms[index].min_char_level = it->getMinCharLevel();
    strcpy( d->rooms[index].name, it->getName().c_str() );
    strcpy( d->rooms[index].password, it->getPassword().c_str() );
    strcpy( d->rooms[index].topic, it->getTopic().c_str() );
  }

  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_CHAT_LIST_ROOMS, ST_None);
  PEPacketHelper::SetPacketData(*pkt, (void*)d, pktSize);

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

