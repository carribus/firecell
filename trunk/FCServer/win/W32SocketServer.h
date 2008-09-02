#ifndef _W32SOCKETSERVER_H_
#define _W32SOCKETSERVER_H_

#include <winsock2.h>
#include "../../common/collections/DataMgr.h"
#include "../interfaces/ISocketServer.h"

#pragma comment(lib, "ws2_32.lib")

class CW32SocketServer : public ISocketServer
{
	class CSocketPool;
	struct stClientSocket
	{
		stClientSocket()
			: s(0)
			, hEvent(0)
			, bAvailable(true)
			, pPool(NULL)
		{
		}

		FCSOCKET s;
		HANDLE hEvent;
		bool bAvailable;
		CSocketPool* pPool;
	};

	class CSocketPool
	{
	public:
		CSocketPool(CW32SocketServer* pOwner);
		~CSocketPool();

		CW32SocketServer* GetOwner()				    { return m_pOwner; }
		void SetThreadHandle(HANDLE hThrd)			{ m_hPoolThrd = hThrd; }
		HANDLE GetThreadHandle()					      { return m_hPoolThrd; }
		DWORD HasAvailableSlots()					      { return WSA_MAXIMUM_WAIT_EVENTS - m_dwSocketCount; }
		DWORD GetSocketCount()						      { return m_dwSocketCount; }
		stClientSocket* GetSocket(int nIndex);
		bool AddSocket(int nIndex, FCSOCKET s, HANDLE hEvent);
		void RemoveSocket(stClientSocket* pSocket);

	private:
		CW32SocketServer* m_pOwner;
		HANDLE m_hPoolThrd;
		DWORD m_dwSocketCount;
		stClientSocket	m_sockets[WSA_MAXIMUM_WAIT_EVENTS];
	};

public:
	CW32SocketServer(void);
	virtual ~CW32SocketServer(void);

	//////////////////////////////////////////////////////////////////////
	// PUBLIC: ISocketServer implementation
	//////////////////////////////////////////////////////////////////////
	void Initialize(LPCSTR lpszBindToAddress, short sPortToBind);
	void Start();
	void Shutdown();
	UINT GetConnectionCount();
	int RegisterSink(ISocketServerSink* pSink);
	void UnregisterSink(ISocketServerSink* pSink);

private:

	bool IsSinkRegistered(ISocketServerSink* pSink);
	bool StartListening();
	bool AcceptConnection();
	bool AddConnectionToPool(FCSOCKET s);
	void DestroyPool(CSocketPool*& pPool);

	// event handlers
	void OnDataReceived(stClientSocket* pSocket);
	void OnClientSocketClosed(stClientSocket* pSocket, DWORD dwErrorCode);

	// threads
	static unsigned WINAPI thrdListenServer(LPVOID pParam);
	static unsigned WINAPI thrdClientServer(LPVOID pParam);

	LPSTR		m_lpszServer;
	short		m_sPort;
	// listener objects
	FCSOCKET		m_sockListener;
	HANDLE		m_hListenThrd;
	HANDLE		m_hListenEvent;
	// client socket pools
	CDataMgr< CSocketPool > m_socketPools;
	// global thread control
	bool		m_bRun;
	// misc variables
	DWORD		m_dwActiveConnections;

	CDataMgr< ISocketServerSink, false > m_arrSinks;
};

#endif//_W32SOCKETSERVER_H_