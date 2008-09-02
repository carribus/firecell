#ifndef _ISOCKETSERVER_H_
#define _ISOCKETSERVER_H_

#include "../../common/fctypes.h"

struct ISocketServerSink
{
	virtual void OnConnect(FCSOCKET s) = 0;
	virtual void OnDisconnect(FCSOCKET s, FCDWORD dwCode) = 0;
	virtual void OnDataReceived(FCSOCKET s, FCBYTE* pData, FCINT nLen) = 0;
};

struct ISocketServer
{
	virtual void Initialize(FCCSTR lpszBindToAddress, FCSHORT sPortToBind) = 0;
	virtual void Start() = 0;
	virtual void Shutdown() = 0;
	virtual FCUINT GetConnectionCount() = 0;
	virtual int RegisterSink(ISocketServerSink* pSink) = 0;
	virtual void UnregisterSink(ISocketServerSink* pSink) = 0;
};

#endif//_ISOCKETSERVER_H_