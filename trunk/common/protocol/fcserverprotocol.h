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
 *  Status enumerations
 */
enum e_SelectCharacterStatus
{
  CharacterSelectFailed = 0,
  CharacterSelectSucceeded = 1,
  CharacterSelectFailedInWorld = 2
};

/*
 *  Message Identifiers
 */

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCSMSG_CLIENT_DISCONNECT                 = SERVERMSG_ID(1);
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from Router service to all connected services indicating
 *         that a connect client has disconnected
 *  @ingroup fcprotocol
 */
struct __FCSPKT_CLIENT_DISCONNECT
{
  FCUINT    clientSocket;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCSMSG_CHARACTER_LOGGEDIN                = SERVERMSG_ID(2);
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from Auth Service to World Service specifying that a specific character
 *         has logged in (includes character details)
 *  @ingroup fcprotocol
 */
struct __FCSPKT_CHARACTER_LOGGEDIN
{
  FCSOCKET clientSocket;
  FCUINT account_id;
  FCUINT character_id;
  char name[32];
  FCUINT xp;
  FCUINT level;
  FCINT fame_scale;
  FCBYTE country_id;
  FCBYTE city_id;
};

/**
 *  @brief Packet sent from World Service to Auth Service indicating that character creation failed
 *         in the world service. This will be sent in response to the __FCSPKT_CHARACTER_LOGGEDIN packet
 *  @ingroup fcprotocol
 */
struct __FCSPKT_CHARACTER_LOGGEDIN_ERROR
{
  FCUINT account_id;
  FCUINT character_id;
  e_SelectCharacterStatus status;
};

#endif//_FCSERVERPROTOCOL_H_