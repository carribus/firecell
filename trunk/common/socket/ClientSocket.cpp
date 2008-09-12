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
#ifdef _WIN32
  #include <winsock2.h>
#else
  #include <sys/socket.h>
#endif//_WIN32
#include "../binstream.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket(FCSOCKET s)
: m_sock(s)
, m_bIsDead(false)
, m_bLocked(false)
{
}

/////////////////////////////////////////////////////////////////////////////////

ClientSocket::~ClientSocket(void)
{
}

/////////////////////////////////////////////////////////////////////////////////

int ClientSocket::Send(FCBYTE* pData, FCUINT nLen)
{
	FCUINT nBytesSent = 0, nTotalSent = 0;
	FCBYTE* pOfs = pData;

	while ( nTotalSent < nLen )
	{
		nBytesSent = send( m_sock, (FCCSTR)pOfs, nLen-nTotalSent, 0 );
		nTotalSent += nBytesSent;
		pOfs += nBytesSent;
	}

	return nTotalSent;
}

/////////////////////////////////////////////////////////////////////////////////

int ClientSocket::AddData(FCBYTE* pData, FCUINT nLen)
{
	if ( !pData || !nLen )
		return -1;

	m_stream.Concat(pData, nLen);
	return m_stream.GetLength();
}