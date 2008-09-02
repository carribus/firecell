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
#ifndef _CLIENTSOCKET_H_
#define _CLIENTSOCKET_H_

#include "../common/fctypes.h"
#include "../common/BinStream.h"

typedef CBinStream<FCBYTE, true> NetStream;

class ClientSocket
{
public:
	ClientSocket(FCSOCKET s);
	virtual ~ClientSocket(void);

	int Send(FCBYTE* pData, FCUINT nLen);
	int AddData(FCBYTE* pData, FCUINT nLen);
	CBinStream<FCBYTE, true>& GetDataStream()		{ return m_stream; }

	operator FCSOCKET()							{ return m_sock; }
	void SetData(void* pData)			{ m_pData = pData; }
	void* GetData()								{ return m_pData; }

private:

	FCSOCKET				m_sock;
	NetStream				m_stream;
	void*					  m_pData;
};

#endif//_CLIENTSOCKET_H_