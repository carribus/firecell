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

#include "../fctypes.h"
#include "../BinStream.h"

typedef CBinStream<FCBYTE, true> NetStream;

class ClientSocket
{
public:
	ClientSocket(FCSOCKET s);
	virtual ~ClientSocket(void);

	int Send(FCBYTE* pData, FCUINT nLen);
	int AddData(FCBYTE* pData, FCUINT nLen);
	CBinStream<FCBYTE, true>& GetDataStream()		{ return m_stream; }

  void IsDead(bool bDead) { m_bIsDead = bDead; }
  bool IsDead()           { return m_bIsDead; }

  bool IsLocked()         { return m_bLocked; }
  void Lock()             { m_bLocked = true; }
  void Unlock()           { m_bLocked = false; }

	operator FCSOCKET()							{ return m_sock; }

private:

	FCSOCKET				m_sock;
	NetStream				m_stream;
  bool            m_bIsDead;
  bool            m_bLocked;
};

#endif//_CLIENTSOCKET_H_