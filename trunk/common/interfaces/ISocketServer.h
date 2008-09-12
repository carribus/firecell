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