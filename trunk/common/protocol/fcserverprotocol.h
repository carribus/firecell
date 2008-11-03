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
#ifndef _FCSERVERPROTOCOL_H_
#define _FCSERVERPROTOCOL_H_

#define FCSERVER_BASE_MSGID           5000

#define SERVERMSG_ID(x) \
  FCSERVER_BASE_MSGID + x

/*
 *  Message Identifiers
 */

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCSMSG_CLIENT_DISCONNECT                 = SERVERMSG_ID(1);
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCSPKT_CLIENT_DISCONNECT
{
  FCUINT    clientSocket;
};


#endif//_FCSERVERPROTOCOL_H_