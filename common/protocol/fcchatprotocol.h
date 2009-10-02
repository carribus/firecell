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
#ifndef _FCCHATPROTOCOL_H_
#define _FCCHATPROTOCOL_H_

#define FCCHAT_BASE_MSGID           7000

#define CHATMSG_ID(x) \
  FCCHAT_BASE_MSGID + x

/*
 *  Message Identifiers
 */

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CHAT_CONNECT                    = CHATMSG_ID(1);
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from client to chat server to 'simulate' a connection and fetch the MOTD
 *  @ingroup fcprotocol
 */
struct __FCPKT_CHAT_CONNECT
{
  FCULONG character_id;
};

struct __FCPKT_CHAT_CONNECT_RESP
{
  e_ChatConnectStatus result;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CHAT_LIST_ROOMS                 = CHATMSG_ID(2);
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from Client to Chat server requesting a list of available chat rooms
 *  @ingroup fcprotocol
 */
struct __FCPKT_CHAT_LIST_ROOMS
{
  FCULONG character_id;
};

struct __FCPKT_CHAT_LIST_ROOMS_RESP
{
  FCULONG numRooms;
  typedef struct
  {
    FCULONG id;
    char name[32];
    char topic[255];
    bool is_private;
    char password[32];
    FCULONG min_char_level;
    bool is_official;
  } ChatRoom;
  ChatRoom rooms[1];
};

#endif//_FCCHATPROTOCOL_H_