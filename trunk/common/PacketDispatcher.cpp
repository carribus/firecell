#include "../common/Logging/DynLog.h"
#include "PacketDispatcher.h"

PacketDispatcher* PacketDispatcher::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

PacketDispatcher::PacketDispatcher(void)
: m_numThreadsPerSocket(1)
{
}

///////////////////////////////////////////////////////////////////////

PacketDispatcher::~PacketDispatcher(void)
{
	stopWorkerThreads();
}

///////////////////////////////////////////////////////////////////////

PacketDispatcher& PacketDispatcher::instance()
{
  if ( !m_pThis )
  {
    m_pThis = new PacketDispatcher;
  }
  return *m_pThis;
}

///////////////////////////////////////////////////////////////////////

void PacketDispatcher::destroy()
{
  if ( m_pThis )
  {
    delete m_pThis;
    m_pThis = NULL;
  }
}

///////////////////////////////////////////////////////////////////////

bool PacketDispatcher::initialise(FCULONG numThreadsPerSocket)
{
  m_numThreadsPerSocket = numThreadsPerSocket;

  return true;
}

///////////////////////////////////////////////////////////////////////

void PacketDispatcher::queuePacket(PEPacket* pkt, BaseSocket* pSocket)
{
  if ( !pkt || !pSocket )
    return;

  PacketList* pList = addSocketToDispatchMap(pSocket);

  if ( pList )
  {
    pList->rwLock.LockForWrite();
    pList->packets.push_back(pkt);
    pList->rwLock.Unlock();
    pList->event.Signal();
  }
}

///////////////////////////////////////////////////////////////////////

PacketDispatcher::PacketList* PacketDispatcher::addSocketToDispatchMap(BaseSocket* pSocket)
{
  if ( !pSocket )
    return NULL;

  DispatchMap::iterator it = m_mapDispatch.find(pSocket);
  PacketList* pList = NULL;

  // check if the dispatch map exists for this socket
  if ( it == m_mapDispatch.end() )
  {
    // the mapping doesn't exist, so we create a new mapping
    pList = new PacketList;
    pList->pSocket = pSocket;
    m_mapDispatch[pSocket] = pList;
    // and kick off the required number of threads for the socket
    for ( FCULONG i = 0; i < m_numThreadsPerSocket; i++ )
    {
      pthread_t thrd;
      
      if ( pthread_create(&thrd, NULL, thrdPacketDispatcher, (void*)pList) != 0 )
      {
        DYNLOG_ADDLOG( DYNLOG_FORMAT("PacketDispatcher::addSocketToDispatchMap(): Failed to create thread %ld for socket %08X", i+1, pSocket) );
      }
      else
      {
        pList->workerThreads.push_back(thrd);
      }
    }
  }
  else
  {
    pList = it->second;
  }

  return pList;
}

///////////////////////////////////////////////////////////////////////

void PacketDispatcher::stopWorkerThreads()
{
	DispatchMap::iterator it = m_mapDispatch.begin();
	DispatchMap::iterator limit = m_mapDispatch.end();

	for ( ; it != limit; it++ )
	{
		it->second->bRunning = false;
		while ( it->second->workerThreads.size() )
		{
			it->second->event.Signal();
			pthread_join( it->second->workerThreads[0], 0 );
			it->second->workerThreads.erase( it->second->workerThreads.begin() );
		}
		delete it->second;
	}
}

///////////////////////////////////////////////////////////////////////

void* PacketDispatcher::thrdPacketDispatcher(void* pData)
{
  if ( !pData )
    return NULL;

  PacketList* pList = (PacketList*)pData;
  PEPacket* pPkt = NULL;
  size_t elemCount = 0;

  pList->bRunning = true;

  while ( pList->bRunning )
  {
    pList->rwLock.LockForRead();
    elemCount = pList->packets.size();
    pList->rwLock.Unlock();

    while ( elemCount )
    {
      // get the next packet from the front of the list
      pList->rwLock.LockForWrite();
      if ( pList->packets.size() == 0 )
        break;

      pPkt = pList->packets.front();
      pList->packets.pop_front();
      pList->rwLock.Unlock();

      // send it and delete it
      pList->pSocket->Send(pPkt);
      delete pPkt;

      // get the element count
      pList->rwLock.LockForRead();
      elemCount = pList->packets.size();
      pList->rwLock.Unlock();
    }

    // wait for more packets to be queued
    pList->event.WaitForSignal();
  }

  return NULL;
}