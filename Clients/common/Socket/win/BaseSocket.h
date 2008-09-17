#ifndef _BASESOCKET_H_
#define _BASESOCKET_H_

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include "../../../../common/fctypes.h"

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

	void Attach(SOCKET s);
	bool Create(int nType = SOCK_STREAM, long lEvents = FD_CLOSE | FD_READ | FD_WRITE | FD_CONNECT);
	void Connect(LPCSTR lpszServer, short sPort);
	void Disconnect();
	bool Listen(short sPort);
	int Send(FCBYTE* pData, int nLen);
	void Subscribe(IBaseSocketSink* pSink)						{ m_pSink = pSink; }
	void Unsubscribe(IBaseSocketSink* pSink)					{ if ( m_pSink == pSink ) m_pSink = NULL; }

	operator SOCKET()											{ return m_socket; }

private:

	void OnAccept(int nErrorCode);
	void OnConnect(int nErrorCode);
	void OnDataReceived(int nErrorCode);
	void OnDisconnect(int nErrorCode);

	static DWORD WINAPI thrdSocket(LPVOID pParam);

	SOCKET		m_socket;
	HANDLE		m_hEvent;
	HANDLE		m_hThrd;
	bool		  m_bRun;

	IBaseSocketSink* m_pSink;
};

#endif//_BASESOCKET_H_