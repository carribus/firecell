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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "BSDSocketServer.h"

//////////////////////////////////////////////////////////////////////////////////////////

BSDSocketServer::CSocketPool::CSocketPool(BSDSocketServer* pOwner)
: m_pOwner(pOwner)
, m_dwSocketCount(0)
, m_nFdsMax(0)
{
  FD_ZERO(&m_fdsSockets);
  
  for ( int i = 0; i < MAX_WAIT_SOCKETS; i++ )
  {
    m_sockets[i].pPool = this;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

BSDSocketServer::CSocketPool::~CSocketPool()
{
}

//////////////////////////////////////////////////////////////////////////////////////////

BSDSocketServer::stClientSocket* BSDSocketServer::CSocketPool::GetSocket(int nIndex)
{
  if ( nIndex < 0 || nIndex >= MAX_WAIT_SOCKETS )
    return NULL;
  
  return &m_sockets[nIndex];
}

//////////////////////////////////////////////////////////////////////////////////////////

bool BSDSocketServer::CSocketPool::AddSocket(int nIndex, FCSOCKET s)
{
  bool bResult = false;
  
  if ( m_sockets[nIndex].bAvailable )
  {
    m_sockets[nIndex].bAvailable = false;
    m_sockets[nIndex].s = s;
    
    // add the socket to the fd_set
    FD_SET(s, &m_fdsSockets);
    if ( s > m_nFdsMax )
      m_nFdsMax = s;
    
    m_dwSocketCount++;
  }
  
  return bResult;
}

//////////////////////////////////////////////////////////////////////////////////////////

void BSDSocketServer::CSocketPool::RemoveSocket(BSDSocketServer::stClientSocket* pSocket)
{
  for ( int i = 0; i < MAX_WAIT_SOCKETS; i++ )
  {
    if ( &m_sockets[i] == pSocket )
    {
      // remove the socket from the fd_set
      FD_CLR( pSocket->s, &m_fdsSockets);

      pSocket->s = 0;
      pSocket->bAvailable = true;
      
      m_dwSocketCount--;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

BSDSocketServer::BSDSocketServer()
: m_lpszServer(NULL)
, m_sPort(0)
, m_bRun(false)
, m_sockListener(0)
, m_dwActiveConnections(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

BSDSocketServer::~BSDSocketServer()
{
  if ( m_lpszServer )
    free(m_lpszServer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void BSDSocketServer::Initialize(FCCSTR lpszBindToAddress, FCSHORT sPortToBind)
{
  if ( m_lpszServer )
    free(m_lpszServer);
  
  if ( lpszBindToAddress )
  {
    hostent* pLocalHost = gethostbyname(lpszBindToAddress);
//    m_lpszServer = strdup(lpszBindToAddress);
    m_lpszServer = strdup( inet_ntoa( *(in_addr*)*pLocalHost->h_addr_list) );
   
  }
  else
  {
    hostent* pLocalHost = gethostbyname(NULL);
    m_lpszServer = strdup( inet_ntoa( *(in_addr*)*pLocalHost->h_addr_list) );
  }
  m_sPort = sPortToBind;
}

//////////////////////////////////////////////////////////////////////////////////////////

void BSDSocketServer::Start()
{
  StartListening();
}

//////////////////////////////////////////////////////////////////////////////////////////

void BSDSocketServer::Shutdown()
{
  // stop all listening and serving activities
  
  // free up the resources
  delete this;
}

//////////////////////////////////////////////////////////////////////////////////////////

FCUINT BSDSocketServer::GetConnectionCount()
{
  return m_dwActiveConnections;
}

//////////////////////////////////////////////////////////////////////////////////////////

int BSDSocketServer::RegisterSink(ISocketServerSink* pSink)
{
  if ( !IsSinkRegistered(pSink) )
    m_arrSinks.Add(pSink);
  
  return m_arrSinks.GetSize();
}

//////////////////////////////////////////////////////////////////////////////////////////

void BSDSocketServer::UnregisterSink(ISocketServerSink* pSink)
{
  int nCount = m_arrSinks.GetSize();
  
  for ( int i = 0; i < nCount; i++ )
  {
    if ( m_arrSinks.GetAt(i) == pSink )
    {
      m_arrSinks.RemoveAt(i);
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

bool BSDSocketServer::IsSinkRegistered(ISocketServerSink* pSink)
{
  m_arrSinks.Lock();
  int nCount = m_arrSinks.GetSize();
  
  for ( int i = 0; i < nCount; i++ )
  {
    if ( m_arrSinks.GetAt(i) == pSink )
      return true;
  }
  
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool BSDSocketServer::StartListening()
{
  if ( !(m_sockListener = socket( PF_INET, SOCK_STREAM, 0 )) )
  {
    return false;
  }
  
	sockaddr_in addr;
  
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; //inet_addr(m_lpszServer);
	addr.sin_port = htons(m_sPort);
	memset( addr.sin_zero, 0, sizeof(addr.sin_zero) );
  
	if ( bind( m_sockListener, (sockaddr*)&addr, sizeof(addr) ) != 0 )
	{
		// an error occurred
		return false;
	}
  
  if ( listen(m_sockListener, 5 ) == -1 )
  {
    return false;
  }
  
  int nRet = pthread_create( &m_hListenThrd, NULL, thrdListenServer, (void*)this );
  if ( nRet != 0 )
  {
    // failed to start the thread...
    return false;
  }
  
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool BSDSocketServer::AcceptConnection()
{
  sockaddr addrClient;
  socklen_t addrSize = sizeof(addrClient);
  
  // accep the new connection
  FCSOCKET s = (FCSOCKET) accept( m_sockListener, (sockaddr*)&addrClient, &addrSize );
  
  if ( s == -1 )
    return false;
  
  if ( AddConnectionToPool(s) )
  {
    m_dwActiveConnections++;
    
    // fire a notification
    int nCount = m_arrSinks.GetSize();
    for ( int i = 0; i < nCount; i++ )
    {
      m_arrSinks.GetAt(i)->OnConnect( s );
    }
  }
  
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool BSDSocketServer::AddConnectionToPool(FCSOCKET s)
{
  if ( s <= 0 )
    return false;
  
  bool bSocketAdded = false;
  int nCount = m_socketPools.GetSize();
  stClientSocket* pSocket = NULL;
  CSocketPool* pPool = NULL;
  
  for ( int i = 0; i < nCount && !bSocketAdded; i++ )
  {
    if ( !(pPool = m_socketPools.GetAt(i)) )
      continue;
    if ( pPool->HasAvailableSlots() )
    {
      for ( int j = 0; j < MAX_WAIT_SOCKETS; j++ )
      {
        if ( (pSocket = pPool->GetSocket(j)) && pSocket->bAvailable )
        {
          // TODO: need to replace the NULL below with an event sync object
          if ( pPool->AddSocket( j, s ) )
          {
            bSocketAdded = true;
            // need to do some form of select'ing on the socket now
            break;
          }
        }
      }
    }
  }
  
  // check if the socket was placed
  if ( !bSocketAdded )
  {
    pthread_t hPoolThrd;
    // if not, then we need to create an additional connection pool
    pPool = new CSocketPool(this);
    
    if ( pPool->AddSocket( 0, s ) )
    {
      pSocket = pPool->GetSocket(0);
      // select'ing here
      
      // start a thread for the pool
      int nRet = pthread_create( &hPoolThrd, NULL, thrdClientServer, (void*)pPool );
      if ( nRet != 0 )
      {
        // error occurred
      }
      pPool->SetThreadHandle(hPoolThrd);
      bSocketAdded = true;
      m_socketPools.Add(pPool);
    }
    else
      delete pPool;
  }
  
  return bSocketAdded;
}

//////////////////////////////////////////////////////////////////////////////////////////

void BSDSocketServer::DestroyPool(CSocketPool*& pPool)
{
  if ( !pPool )
    return;
  
  int nCount = m_socketPools.GetSize();
  
  for ( int i = 0; i < nCount; i++ )
  {
    if ( m_socketPools.GetAt(i) == pPool )
    {
      m_socketPools.RemoveAt(i);
      delete pPool;
      pPool = NULL;
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
  
void BSDSocketServer::OnDataReceived(stClientSocket* pSocket)
{
  int nBytesRead  = 0;
  char buffer[4096];
  
  memset(buffer, 0, sizeof(buffer));
  nBytesRead = recv( pSocket->s, buffer, sizeof(buffer), 0);
  if ( nBytesRead <= 0 )
  {
    // something went wrong...
    OnClientSocketClosed(pSocket, 0);
  }
  else
  {
    // fire an event to all subscribers
    int nCount = m_arrSinks.GetSize();
    for ( int i = 0; i < nCount; i++ )
    {
      m_arrSinks.GetAt(i)->OnDataReceived(pSocket->s, (FCBYTE*)buffer, nBytesRead);
    }
  }
}
  
//////////////////////////////////////////////////////////////////////////////////////////
  
void BSDSocketServer::OnClientSocketClosed(stClientSocket* pSocket, FCDWORD dwErrorCode)
{
  if ( !pSocket )
    return;
  
  CSocketPool* pPool = pSocket->pPool;
  
  // fire an event to all subscribers
  int nCount = m_arrSinks.GetSize();
  for ( int i = 0; i < nCount; i++ )
  {
    m_arrSinks.GetAt(i)->OnDisconnect(pSocket->s, dwErrorCode);
  }
  
	pPool->RemoveSocket(pSocket);
	if ( pPool->GetSocketCount() == 0 )
	{
		DestroyPool(pPool);
    delete pPool;
	}
  m_dwActiveConnections--;
}

//////////////////////////////////////////////////////////////////////////////////////////
  
void* BSDSocketServer::thrdListenServer(void* pParam)
{
  BSDSocketServer* pThis = (BSDSocketServer*) pParam;
  fd_set master_fds, read_fds;
  timeval tv;
  
  if ( !pThis )
    return (void*)-1;
  
  // initialise
  FD_ZERO(&master_fds);
  FD_ZERO(&read_fds);
  
  FD_SET( pThis->m_sockListener, &master_fds );
  
  // setup the 500ms delay for select( ) 
  tv.tv_sec = 0;
  tv.tv_usec = 500000;
  
  // flag the thread to run
  pThis->m_bRun = true;
  
  while ( pThis->m_bRun )
  {
    // copy the master over
    read_fds = master_fds;
    
    if ( select( pThis->m_sockListener+1, &read_fds, NULL, NULL, &tv) == -1 )
    {
      // the select broke
      break;
    }

    if ( FD_ISSET( pThis->m_sockListener, &read_fds ) )
    {
      // a connection came through on the listener
      pThis->AcceptConnection();
    }
  }
  
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
  
void* BSDSocketServer::thrdClientServer(void* pParam)
{
	CSocketPool* pPool = (CSocketPool*) pParam;
	BSDSocketServer* pThis = pPool->GetOwner();
  stClientSocket* pSocket = NULL;
  fd_set read_fds;
  int nBytes = 0;
  char buf[4096];
  timeval tv;
  
  FD_ZERO(&read_fds);
  
  // setup the 500ms timeout for the select( )
  tv.tv_sec = 0;
  tv.tv_usec = 500000;
  
  while ( pThis->m_bRun && pPool->GetSocketCount() )
  {
    int fdMax = pPool->GetMaxFDS();
    read_fds = pPool->GetFDS();
    
    if ( select(fdMax+1, &read_fds, NULL, NULL, &tv) == -1 )
    {
      // something broke in the select( )
      break;
    }
    
    for ( int i = 0; i < fdMax; i++ )
    {
      if ( FD_ISSET(i, &read_fds) )
      {
        FCDWORD nSockCount = pPool->GetSocketCount();
        for ( FCDWORD nSock = 0; nSock < nSockCount; nSock++ )
        {
          pSocket = pPool->GetSocket(nSock);
          if ( pSocket )
          {
            if ( pSocket->s == i )
            {
              break;
            }
            pSocket = NULL;
          }
        }
        
        if ( pSocket )
        {
          // handle data from the client
          if ( (nBytes = recv(i, buf, sizeof(buf), 0)) <= 0 )
          {
            // an error occurred or the connection was closed by the client
            pThis->OnClientSocketClosed(pSocket, 0);;
            
            close(i);
          }
          else
          {
            // got some data
            pThis->OnDataReceived(pSocket);
          }
        }
      }
    }
  }

	delete pPool;
  
	return 0;  
}