#ifndef _PACKETDISPATCHER_H_
#define _PACKETDISPATCHER_H_

#include <list>
#include <map>
#include <vector>
#include "fctypes.h"
#include "PEPacket.h"
#include "PThreadRWLock.h"
#include "PThreadCond.h"
#include "threading.h"
#include "../Clients/common/Socket/ClientSocket.h"

#define QueuePacket(pkt, sock)  PacketDispatcher::instance().queuePacket( (pkt), (sock) )

/**
 *  @class PacketDispatcher
 *  @brief The PacketDispatcher class is responsible for dispatching queues of packets to their respective destinations.
 */
class PacketDispatcher
{
	PacketDispatcher(void);
	~PacketDispatcher(void);

public:

	static PacketDispatcher& instance();
	static void destroy();

  bool initialise(FCULONG numThreadsPerSocket = 1);
	void queuePacket(PEPacket* pkt, BaseSocket* pSocket);

private:

  /*
   *  Private structures
   */
  struct PacketList
  {
    BaseSocket* pSocket;
    std::list<PEPacket*> packets;
    std::vector<pthread_t> workerThreads;
    bool bRunning;
    PThreadRWLock rwLock;
    PThreadCond event;
  };

  /*
   *  Private methods
   */
  PacketList* addSocketToDispatchMap(BaseSocket* pSocket);
	void stopWorkerThreads();
  
  // worker thread method
  static void* thrdPacketDispatcher(void* pData);

  /*
   *  Private members
   */

  static PacketDispatcher* m_pThis;
  FCULONG m_numThreadsPerSocket;

  typedef std::map<BaseSocket*, PacketList*> DispatchMap;
  DispatchMap m_mapDispatch;
};

#endif//_PACKETDISPATCHER_H_