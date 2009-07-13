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

#ifndef _BASESOCKET_H_
#define _BASESOCKET_H_

#include <string>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include "../../../../common/fctypes.h"
#include "../../../../common/PEPacket.h"

using namespace std;

#ifndef interface
#define interface struct
#endif//interface

class BaseSocket;
interface IBaseSocketSink
{
	virtual void OnAccepted(BaseSocket* pSocket, int nErrorCode) = 0;
	virtual void OnConnected(BaseSocket* pSocket, int nErrorCode) = 0;
	virtual void OnDisconnected(BaseSocket* pSocket, int nErrorCode) = 0;
	virtual void OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen) = 0;
};

class BaseSocket
{
public:
	BaseSocket(void);
	virtual ~BaseSocket(void);

	void Attach(FCSOCKET s);
	bool Create(int nType = SOCK_STREAM);
	void Connect(std::string strServer, short sPort);
	void Disconnect();
	bool Listen(short sPort);
	int Send(FCBYTE* pData, int nLen);
  int Send(PEPacket* pkt);
	void Subscribe(IBaseSocketSink* pSink)						{ m_pSink = pSink; }
	void Unsubscribe(IBaseSocketSink* pSink)					{ if ( m_pSink == pSink ) m_pSink = NULL; }

	operator FCSOCKET()                               { return m_socket; }

private:

	void OnAccept(int nErrorCode);
	void OnConnect(int nErrorCode);
	void OnDataReceived(int nErrorCode);
	void OnDisconnect(int nErrorCode);

	static void* thrdSocket(void* pParam);

	FCSOCKET		m_socket;
  pthread_t   m_hThrd;
  pthread_cond_t m_hEvent;
	bool        m_bRun;

	IBaseSocketSink* m_pSink;
};

#endif//_BASESOCKET_H_
