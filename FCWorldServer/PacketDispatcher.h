#ifndef _PACKETDISPATCHER_H_
#define _PACKETDISPATCHER_H_

#include <vector>
#include "../common/PEPacket.h"
#include "../Clients/common/Socket/ClientSocket.h"

class PacketDispatcher
{
	PacketDispatcher(void);
	~PacketDispatcher(void);

public:

	static PacketDispatcher& instance();
	static void destroy();

	void queuePacket(PEPacket* pkt, BaseSocket* pSocket);

private:


};

#endif//_PACKETDISPATCHER_H_