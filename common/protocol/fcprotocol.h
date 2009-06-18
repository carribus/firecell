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
/**
 *  @defgroup fcprotocol FireCell Protocol
 *  @file fcprotocol.h
 *  @ingroup fcprotocol
 */
#ifndef _FCPROTOCOL_H_
#define _FCPROTOCOL_H_

#include "../fctypes.h"
#include "../fcconstants.h"
// ensure 1 byte packing
#pragma pack(push, 1)

const FCBYTE      FCPKT_COMMAND = 1;
const FCBYTE      FCPKT_RESPONSE = 2;
const FCBYTE      FCPKT_ERROR = 3;

#include "fcserverprotocol.h"

/*
  
  NB: Protocol definitions must be accompanied with an extraction plan.

*/

/**
 *  @brief This is the PEPacketExtractor packet defintion for a firecell packet
 *  @ingroup fcprotocol
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
/*                      
struct __FCPACKET
{
  FCBYTE          magic[8];           // must be [0A 0A 00 10 00 20 00 40]
  FCBYTE          pktType;
  ServiceType     target;             // the target service for the message
  unsigned short  msg;                // msg code
  unsigned long   lDataLen;
  char*           pData;
};
*/

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
/**
 *  @brief Packet that is sent from server to client giving general server information
 *  @ingroup fcprotocol
 */

struct __FCPKT_INFO_SERVER
{
  FCBYTE    verMajor;
  FCBYTE    verMinor;
  FCUINT    connectionCountRouter;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_REGISTER_SERVICE            = 2;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent/received when a service attempts to register with another service
 *  @ingroup fcprotocol
 */
struct __FCPKT_REGISTER_SERVER
{
  ServiceType     type;
  bool            status;         // only used in response to indicate whether registration succeeded or not
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_LOGIN                       = 3;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from client to auth server when a player attempts to login
 *  @ingroup fcprotocol
 */
struct __FCPKT_LOGIN
{
  char username[65];
  char password[65];
};

/**
 *  @brief Packet sent from server to client in response to a login attempt
 *  @ingroup fcprotocol
 */
struct __FCPKT_LOGIN_RESP
{
  e_LoginStatus loginStatus;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_GETCHARACTERS               = 20;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from server to client listing the available characters for the player's account
 *  @ingroup fcprotocol
 */
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
const FCSHORT FCMSG_SELECT_CHARACTER            = 21;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from client to server to select a specific character
 *  @ingroup fcprotocol
 */
struct __FCPKT_SELECT_CHARACTER
{
  FCUINT character_id;
};

/**
 *  @brief Response packet from server to indicate character selection result
 *  @ingroup fcprotocol
 */
struct __FCPKT_SELECT_CHARACTER_RESP
{
  FCUINT character_id;
  e_SelectCharacterStatus status;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_GET_CHAR_CREATION_PARAMS    = 22;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from client to server to retrieve character creation parameters
 *  @ingroup fcprotocol
 */
struct __FCPKT_GET_CHAR_CREATION_PARAMS
{
  FCUINT reserved1;           // not used
};

struct __FCPKT_GET_CHAR_CREATION_PARAMS_RESP
{
  FCBYTE respType;            // 0 = countries, 1 = cities
};

/**
 *  @brief Packet send from server to client detailing available in-game countries for character location
 *  @ingroup fcprotocol
 */
struct __FCPKT_GET_CHAR_CREATION_PARAMS_COUNTRIES_RESP : public __FCPKT_GET_CHAR_CREATION_PARAMS_RESP
{
  FCULONG numCountries;
  struct Country
  {
    FCULONG country_id;
    char name[32];
  } countries[1];
};

/**
 *  @brief Packet sent from server to client detailing available in-game cities for character creation
 *  @ingroup fcprotocol
 */
struct __FCPKT_GET_CHAR_CREATION_PARAMS_CITIES_RESP : public __FCPKT_GET_CHAR_CREATION_PARAMS_RESP
{
  FCULONG numCities;
  struct City
  {
    FCULONG country_id;
    FCULONG city_id;
    char name[32];
  } cities[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CHARACTER_ASSET_REQUEST     = 23;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from client to server requesting a character's assets
 *  @ingroup fcprotocol
 */
struct __FCPKT_CHARACTER_ASSET_REQUEST
{
  FCUINT character_id;
};

/**
 *  @brief Response packet sent from server to client containing a characters assets
 *  @ingroup fcprotocol
 */
struct __FCPKT_CHARACTER_ASSET_REQUEST_RESP
{
  struct _ip_address
  {
    FCSHORT a, b, c, d;
  } ip_address;

  /**
   *  @brief Defines the computer equipment available to a character
   */

  struct _computer
  {
    FCULONG id;
    char name[33];
    FCUINT hddSize;
    FCUINT networkSpeed;
		FCSHORT availablePorts;

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

		struct _networkports
		{
			bool enabled;
			FCSHORT portNum;
			FCULONG softwareType;
			FCULONG itemID;
      FCSHORT portMaxHealth;
			FCSHORT portHealth;
		} network_ports[8];
  } computer;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CHARACTER_ITEMS_REQUEST     = 24;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from client to server requesting a character's items
 *  @ingroup fcprotocol
 */
struct __FCPKT_CHARACTER_ITEMS_REQUEST
{
  FCUINT character_id;
};

/**
 *  @brief Response packet sent from server to client containing a characters items
 *  @ingroup fcprotocol
 */
struct __FCPKT_CHARACTER_ITEMS_REQUEST_RESP
{
	FCULONG itemCount;
	struct _software : public _game_item
	{
		FCSHORT softwareTypeID;
		bool is_service;
		FCULONG scriptID;
    FCULONG itemCount;
	} software[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CHARACTER_MISSIONS_REQUEST  = 25;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from client to server requesting a character's missions
 *  @ingroup fcprotocol
 */
struct __FCPKT_CHARACTER_MISSIONS_REQUEST
{
  FCUINT character_id;
};

/**
 *  @brief Response packet sent from server to client containing a character's missions
 *  @ingroup fcprotocol
 */
struct __FCPKT_CHARACTER_MISSIONS_REQUEST_RESP
{
  FCULONG numMissions;
  struct _missions
  {
    FCULONG mission_id;
    FCULONG parent_id;
    FCSHORT success_count;
    FCSHORT failure_count;
    bool completed;
  } missions[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_GET_DESKTOP_OPTIONS         = 40;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Request packet from client to server for available desktop options for the character
 *  @ingroup fcprotocol
 */
struct __FCPKT_GET_DESKTOP_OPTIONS
{
  FCUINT character_id;
};

/**
 *  @brief Response packet from server to client listing available desktop options
 *  @ingroup fcprotocol
 */
struct __FCPKT_GET_DESKTOP_OPTIONS_RESP
{
  FCUSHORT  numOptions;
  struct DesktopOptions
  {
    FCULONG optionID;                     //!> ID of the application/desktop option
    DesktopOptionType type;               //!> Application type that desktop option represents
    char name[32];                        //!> Name of the desktop option/application
  } Options[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_ACTIVATE_SOFTWARE      = 41;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Request packet from client to server to attempt to activate an application
 *  @ingroup fcprotocol
 */
struct __FCPKT_ACTIVATE_SOFTWARE
{
  FCULONG itemID;
};

/**
 *  @brief Response packet from server to client for desktop option activation
 *  @ingroup fcprotocol
 */
struct __FCPKT_ACTIVATE_SOFTWARE_RESP
{
	FCULONG itemID;
	FCULONG result;
	FCSHORT cpu_cost;
	FCULONG mem_cost;
};

/*
 *  CONSOLE module specific commands
 *
 *  These messages are specifically for use by the console 'application' on the user's desktop and give access to the user's (or target's) file system
 */

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CON_GET_FS_INFO             = 100;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Request packet from client to server to retrieve file system information
 *  @ingroup fcprotocol
 */
struct __FCPKT_CON_GET_FS_INFO
{
  FCULONG character_id;
};

/**
 *  @brief Response packet from server to client providing filesystem information for a particular host
 *  @ingroup fcprotocol
 */
struct __FCPKT_CON_GET_FS_INFO_RESP
{
  FCINT fsStyle;
  char dirSeperator[16];
  char currentDir[256];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CON_GET_FILE_LIST           = 101;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Request packet from client to server to retrieve a structured list of files in a specific directory
 *  @ingroup fcprotocol
 */
struct __FCPKT_CON_GET_FILE_LIST
{
  char currentDir[256];
};

/**
 *  @brief Response packet from server to client providing a list of available files in a specific directory
 *  @ingroup fcprotocol
 */
struct __FCPKT_CON_GET_FILE_LIST_RESP
{
  FCULONG numFiles;
  struct _files
  {
    char filename[64];
    FCULONG fizesize;
    bool is_mutable;
    bool is_dir;
  } Files[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_CON_COMMAND                 = 102;
///////////////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief Packet sent from client to server with an explicit console command
 *  @ingroup fcprotocol
 */
struct __FCPKT_CON_COMMAND
{
  char currentDir[256];
  char command[64];
};

/**
 *  @brief Response packet sent from server to client with result string of the explicit console command
 *  @ingroup fcprotocol
 */
struct __FCPKT_CON_COMMAND_RESP
{
  FCINT len;
  char currentDir[256];
  char result[1];
};

/*
 *  FORUM module specific commands
 *
 *  These messages are specifically for use by the forum 'application' on the user's desktop
 */

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_FORUM_GET_CATEGORIES        = 200;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_FORUM_GET_CATEGORIES
{
  FCULONG character_id;
};

struct __FCPKT_FORUM_GET_CATEGORIES_RESP
{
	FCSHORT category_count;
	struct category_info
	{
		FCULONG category_id;
		FCULONG parent_id;
		FCULONG order;
    FCULONG thread_count;
		char name[32];
		char desc[256];
	} categories[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_FORUM_GET_THREADS           = 201;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_FORUM_GET_THREADS
{
	FCULONG category_id;
};

struct __FCPKT_FORUM_GET_THREADS_RESP
{
	FCULONG thread_count;
	FCULONG category_id;
	struct thread_data
	{
		FCULONG thread_id;
		FCULONG parent_id;
		FCULONG order;
		char title[255];
		FCULONG author_id;
		char author_name[32];
		char date_created[32];
		FCULONG mission_id;
	} threads[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_FORUM_GET_THREAD_DETAILS    = 202;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_FORUM_GET_THREAD_DETAILS
{
	FCULONG category_id;
	FCULONG thread_id;
};

struct __FCPKT_FORUM_GET_THREAD_DETAILS_RESP
{
  //! category that this thread belongs to
  FCULONG category_id;              
  //! Number of posts in this thread
  FCULONG post_count;               
  //! size of the content blob
  FCULONG contentSize;              

  struct _thread_data
  {
    FCULONG thread_id;
    FCULONG parent_id;
    FCULONG author_id;
    char author_name[32];
		char title[255];
    char date_created[32];
    FCULONG contentIndex;           // index into blob at the end of this packet for this posts's content
    FCULONG contentLen;
  } ThreadData[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_FORUM_GET_THREAD_CONTENTBLOB     = 203;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_FORUM_GET_THREAD_CONTENTBLOB
{
  FCULONG category_id;
  FCULONG thread_id;
};

struct __FCPKT_FORUM_GET_THREAD_CONTENTBLOB_RESP
{
  FCULONG category_id;
  FCULONG thread_id;
  FCULONG contentSize;              //!> size of the content blob
  FCBYTE contentBlob[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_FORUM_CREATE_NEW_THREAD     = 204;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_FORUM_CREATE_NEW_THREAD
{
  FCULONG category_id;
	FCULONG thread_id;
  char title[255];
  FCULONG contentLength;
  char content[1];
};

struct __FCPKT_FORUM_CREATE_NEW_THREAD_RESP
{
	FCULONG category_id;
	FCULONG thread_id;
	bool bSuccess;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_BANK_CONNECT			          = 250;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_BANK_CONNECT
{
  FCULONG character_id;
};

struct __FCPKT_BANK_CONNECT_RESP
{
  bool bNeedAuth;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_BANK_AUTHENTICATE           = 251;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_BANK_AUTHENTICATE
{
  char password[33];
};

struct __FCPKT_BANK_AUTHENTICATE_RESP
{
  bool bResult;
};

/*
 *  MISSION module specific commands
 *
 *  These messages are specifically for missions
 */

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_MISSION_ACCEPT			        = 500;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_MISSION_ACCEPT
{
	FCULONG mission_id;
};

struct __FCPKT_MISSION_ACCEPT_RESP
{
  bool bSuccessFlag;
  FCULONG mission_id;
  FCULONG numChildMissions;
  struct _child_missions
  {
    FCULONG mission_id;
  } child_missions[1];
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_MISSION_COMPLETE		        = 501;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_MISSION_COMPLETE
{
  FCULONG mission_id;
};

/*
 *  SOFTWARE module specific commands
 *
 *  These messages are specifically for software (un)installation
 */

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_SOFTWARE_INSTALL		        = 550;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_SOFTWARE_INSTALL
{
  FCSHORT portNum;
  FCULONG itemID;
};

struct __FCPKT_SOFTWARE_INSTALL_RESP
{
  FCSHORT portNum;
  FCULONG itemID;
  bool bResult;             // flag indicating success/failure
//  e_SoftwareInstallResult result;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_SOFTWARE_UNINSTALL		      = 551;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_SOFTWARE_UNINSTALL
{
  FCSHORT portNum;
};

struct __FCPKT_SOFTWARE_UNINSTALL_RESP
{
  FCSHORT portNum;
  bool bResult;
};

///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_SOFTWARE_NETWORK_PORT_ENABLE = 552;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_SOFTWARE_NETWORK_PORT_ENABLE
{
  FCSHORT portNum;
  bool bEnable;
};

struct __FCPKT_SOFTWARE_NETWORK_PORT_ENABLE_RESP
{
  FCSHORT portNum;
  bool bEnabled;
  FCSHORT result;
};

/*
 *  MISCELLANEOUS module specific commands
 */
///////////////////////////////////////////////////////////////////////////////////////////
const FCSHORT FCMSG_XP_GAINED						        = 600;
///////////////////////////////////////////////////////////////////////////////////////////
struct __FCPKT_XP_GAINED
{
  FCSHORT xpGained;								//!> The amount of xp gained
	FCULONG xpTotal;								//!> The new total of XP the player has
};

#pragma pack(pop)

#endif//_FCPROTOCOL_H_