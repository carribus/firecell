/**
 *  @defgroup worldcomms World Server Comms
 *  @brief This file contains packet sending functions for use by the FireCell World Service
 *  @ingroup worldcomms
 */
#ifndef _WORLD_COMMS_H_
#define _WORLD_COMMS_H_

#include <vector>
#include "../common/protocol/fcprotocol.h"
#include "../common/ServiceLogicBase.h"
#include "City.h"
#include "Country.h"
#include "FileSystem.h"
#include "ForumCategory.h"
#include "ForumPost.h"
#include "Player.h"


/*************************************************************************************************************** 
 *  CHARACTER SEND METHODS
 ***************************************************************************************************************/

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a character login attempt. The packet sent is: __FCPKT_SELECT_CHARACTER_RESP
 *  @param accountID The account ID that the character belongs to
 *  @param characterID The character ID of the character attempting to login
 *  @param status The result of the login operation
 */
void SendCharacterLoginStatus(FCULONG accountID, FCULONG characterID, e_SelectCharacterStatus status, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function causes the various parameters for character creation to be sent to the client
 *  @param countries A std::vector collection of available countries to send
 *  @param cities A std::vector collection of available cities to send
 */
void SendCharCreationParameters(std::vector<Country>& countries, std::vector<City>& cities, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the available country details for character creation. The packet sent is: __FCPKT_GET_CHAR_CREATION_PARAMS_COUNTRIES_RESP
 *  @param countries A std::vector collection of available countries to send
 */
void SendCharCreationParameters_Countries(std::vector<Country>& countries, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the available City details for character creation. The packet sent is: __FCPKT_GET_CHAR_CREATION_PARAMS_CITIES_RESP
 *  @param cities A std::vector collection of available cities to send
 */
void SendCharCreationParameters_Cities(std::vector<City>& cities, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends a character's assets to them. The packet sent is: __FCPKT_CHARACTER_ASSET_REQUEST_RESP
 *  @param pPlayer The player object that requested the assets list
 */
void SendCharacterAssetResponse(Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the available desktop options that a character has. The packet sent is: __FCPKT_GET_DESKTOP_OPTIONS_RESP
 *  @param pPlayer The character that this response pertains to
 */
void SendCharacterDesktopOptions(Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a player trying to activate a desktop option (app). The packet sent is: __FCPKT_ACTIVATE_DESKTOP_OPTION_RESP
 *  @param optionID The option ID of the desktop option that the player tried to activate
 *  @param pPlayer The player that tried to activate the desktop option
 */
void SendActivateDesktopOptionResponse(FCULONG optionID, Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket);

/*************************************************************************************************************** 
 *  CONSOLE SEND METHODS
 ***************************************************************************************************************/

/**
 *  @ingroup worldcomms
 *  @brief This function sends basic file system information to the player. The packet sent is: __FCPKT_CON_GET_FS_INFO_RESP
 *  @param fs The filesystem for which the info is being sent
 */
void SendConsoleFileSystemInfo(FileSystem& fs, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends a list of files for a specific directory. The packet sent is: __FCPKT_CON_GET_FILE_LIST_RESP
 *  @param currentDir The current directory that the console is in
 *  @param files a std::vector of FileSystem::File objects that are located in the 'current directory'.
 */
void SendConsoleFileList(string currentDir, vector<FileSystem::File> files, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the results of a console command being executed by the player. The packet sent is: __FCPKT_CON_COMMAND_RESP
 *  @param pPlayer The player that initiated the console command for this result to have been generated
 */
void SendConsoleCommandResult(Player* pPlayer, string result, BaseSocket* pRouter, FCSOCKET clientSocket);

/*************************************************************************************************************** 
 *  FORUM SEND METHODS
 ***************************************************************************************************************/

/**
 *  @ingroup worldcomms
 *  @brief This function sends the available categories in the forum to the player. The packet sent is: __FCPKT_FORUM_GET_CATEGORIES_RESP
 *  @param categories A std::vector of ForumCategory* pointers
 */
void SendForumCategories(vector<ForumCategory*>& categories, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends all threads that exist in the requested category. The packet sent is: __FCPKT_FORUM_GET_THREADS_RESP
 *  @param category_id The category ID to which the threads in this message belong
 *  @param threads A std::vector of ForumPost* pointers, each post representing a main thread in the category
 */
void SendForumThreads(FCULONG category_id, vector<ForumPost*>& threads, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the details of a specific forum thread to the client. The packet sent is: __FCPKT_FORUM_GET_THREAD_DETAILS_RESP
 *  @param category_id The category ID that owns the thread whose details we are sending
 *  @param pPost A pointer to the thread object that we need to send
 */
void SendForumThreadDetails(FCULONG category_id, ForumPost* pPost, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the content blob for the thread (and its replies). The packet sent is: __FCPKT_FORUM_GET_THREAD_CONTENTBLOB_RESP
 *  @param category_id The category ID that owns the thread whose details we are sending
 *  @param pPost A pointer to the thread object for which we are sending content
 */
void SendForumThreadContentBlob(FCULONG category_id, ForumPost* pPost, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the details of a specific forum thread to the client. The packet sent is: __FCPKT_FORUM_CREATE_NEW_THREAD_RESP
 *  @param category_id The category ID that owns the thread whose details we are sending
 *  @param pPost A pointer to the thread object that we need to send
 */
void SendForumCreateNewThreadResult(FCULONG category_id, FCULONG thread_id, bool bResult, BaseSocket* pRouter, FCSOCKET clientSocket);

/*************************************************************************************************************** 
 *  MISSION SEND METHODS
 ***************************************************************************************************************/

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a player attempting to accept a mission. The packet sent is: __FCPKT_MISSION_ACCEPT_RESP: 
 *  @param pPlayer The player that accepted the mission
 *  @param mission_id The Id of the mission that was accepted
 *  @bSuccessFlag A flag indicating the success of the player accepting the mission
 */
void SendMissionAcceptedResponse(Player* pPlayer, FCULONG mission_id, bool bSuccessFlag, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends a notification (FCPKT_COMMAND) to a player that they have completed a particular mission. The packet sent is: __FCPKT_MISSION_COMPLETE
 *  @param mission_id The Id of the mission that was completed
 */
void SendMissionComplete(FCULONG mission_id, BaseSocket* pRouter, FCSOCKET clientSocket);

#endif//_WORLD_COMMS_H_