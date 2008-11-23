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
#ifndef _FCPROTOCOL_H_
#define _FCPROTOCOL_H_

#include "../fctypes.h"
// ensure 1 byte packing
#pragma pack(push, 1)

const FCBYTE      FCPKT_COMMAND = 1;
const FCBYTE      FCPKT_RESPONSE = 2;
const FCBYTE      FCPKT_ERROR = 3;

enum ServiceType
{
  ST_None,
  ST_Router,
  ST_Auth,
  ST_World,
  ST_Community,
  ST_Client       // this should always be the last in the enum
};

#include "fcserverprotocol.h"

/*
  
  NB: Protocol definitions must be accompanied with an extraction plan.

*/

static const char* __FCPACKET_DEF = 
"[" \
":magic:1:8|" \
":type:1:1|" \
":target:4:1|" \
":msg:2:1|" \
":dataLen:4:1|" \
":data:*dataLen:1|" \
"]";
                      
struct __FCPACKET
{
  FCBYTE          magic[8];           // must be [0A 0A 00 10 00 20 00 40]
  FCBYTE          pktType;
  ServiceType     target;             // the target service for the message
  unsigned short  msg;                // msg code
  unsigned long   lDataLen;
  char*           pData;
};

/*
 *  Basic structures for serialisation
 */
struct _game_item
{
  FCULONG item_id;
  char name[65];
  FCINT itemtype_id;
  FCINT min_level;
  FCINT max_level;
  FCULONG npc_value;
};

/*
 *  Standard Message Identifiers
 */

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_INFO_SERVER                 = 1;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_INFO_SERVER
{
  FCBYTE    verMajor;
  FCBYTE    verMinor;
  FCUINT    connectionCountRouter;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_REGISTER_SERVICE            = 2;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_REGISTER_SERVER
{
  ServiceType     type;
  bool            status;         // only used in response to indicate whether registration succeeded or not
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_LOGIN                       = 3;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_LOGIN
{
  char username[65];
  char password[65];
};

enum e_LoginStatus
{
  LoginFailed = 0,
  LoginSuccess = 1,
  LoginAccountLoggedInAlready = 2
};

struct __FCPKT_LOGIN_RESP
{
  e_LoginStatus loginStatus;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_GETCHARACTERS               = 4;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_CHARACTER_LIST
{
  FCBYTE numCharacters;

  struct Character
  {
    FCUINT character_id;
    char name[32];
    FCUINT xp;
    FCUINT level;
    FCUINT fame_scale;
    FCBYTE country_id;
    FCBYTE city_id;
  };
  Character characters[12];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_SELECT_CHARACTER            = 5;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_SELECT_CHARACTER
{
  FCUINT character_id;
};

struct __FCPKT_SELECT_CHARACTER_RESP
{
  FCUINT character_id;
  e_SelectCharacterStatus status;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CHARACTER_ASSET_REQUEST     = 6;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_CHARACTER_ASSET_REQUEST
{
  FCUINT character_id;
};

struct __FCPKT_CHARACTER_ASSET_REQUEST_RESP
{
  struct _ip_address
  {
    FCSHORT a, b, c, d;
  } ip_address;

  struct _computer
  {
    FCULONG id;
    char name[33];
    FCUINT hddSize;
    FCUINT networkSpeed;

    struct _processor : public _game_item
    {
      FCSHORT core_count;
      FCSHORT core_speed;
    } processor;

    struct _os : public _game_item
    {
      FCSHORT kernel_id;
      char kernel_name[33];
    } os;

    struct _memory : public _game_item
    {
      FCULONG mem_size;
    } memory;
  } computer;
};


#pragma pack(pop)

#endif//_FCPROTOCOL_H_