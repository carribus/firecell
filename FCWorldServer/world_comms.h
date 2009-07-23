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
#include "../common/game_objects/BankAccount.h"
#include "City.h"
#include "Country.h"
#include "FileSystem.h"
#include "ForumCategory.h"
#include "ForumPost.h"
#include "ItemManager.h"
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
 *  @brief This function sends a character's items to them. The packet sent is: __FCPKT_CHARACTER_ITEMS_REQUEST_RESP
 *  @param pPlayer The player object that requested the assets list
 */
void SendCharacterItemsResponse(Player* pPlayer, ItemManager& itemMgr, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends a character's missions to them. The packet sent is: __FCPKT_CHARACTER_MISSIONS_REQUEST_RESP
 *  @param pPlayer The player object that requested the missions list
 */
void SendCharacterMissionsResponse(Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the available desktop options that a character has. The packet sent is: __FCPKT_GET_DESKTOP_OPTIONS_RESP
 *  @param pPlayer The character that this response pertains to
 */
void SendCharacterDesktopOptions(Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a player trying to activate a desktop option (app). The packet sent is: __FCPKT_ACTIVATE_SOFTWARE_RESP
 *  @param optionID The option ID of the desktop option that the player tried to activate
 *  @param pPlayer The player that tried to activate the desktop option
 */
void SendActivateSoftwareResponse(ItemSoftware* pSoftware, FCULONG result, Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket);

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

/*************************************************************************************************************** 
 *  BANK SEND METHODS
 ***************************************************************************************************************/

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a player attempting to connect to the bank. The packet sent is: __FCPKT_BANK_CONNECT_RESP
 *  @param ticket The ticket generated for this bank account connection
 *  @param status The status of the connection attempt
 */
void SendBankConnectResponse(FCULONG ticket, BankStatus status, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a player attempting to create a new bank account. The packet sent is: __FCPKT_BANK_CREATE_ACCOUNT_RESP
 *  @param ticket The ticket generated for this bank account connection
 *  @param bResult The result of the attempted account creation
 */
void SendBankCreateAccountResponse(FCULONG ticket, bool bResult, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a player attempting to authenticate for their bank account. The packet sent is: __FCPKT_BANK_AUTHENTICATE_RESP
 *  @param ticket The ticket generated for this bank account connection
 *  @param bResult The result of the attempted account authentication
 */
void SendBankAuthenticateResponse(FCULONG ticket, bool bResult, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the details of a player's bank account to them. The packet sent is: __FCPKT_BANK_GET_DETAILS_RESP
 *  @param pAccount The account that should have its details sent
 */
void SendBankAccountDetailsResponse(BankAccount* pAccount, BaseSocket* pRouter, FCSOCKET clientSocket);

/*************************************************************************************************************** 
 *  SOFTWARE INSTALLATION SEND METHODS
 ***************************************************************************************************************/

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a player attempting to install software to a specific port. The packet sent is: __FCPKT_SOFTWARE_INSTALL_RESP
 *  @param itemID The ID of the item that was attempted to be installed
 *  @param portNum The port number that the installation was attempted on
 *  @param bSuccess The success flag (true == success, false == failure)
 */
void SendSoftwareInstallResponse(FCULONG itemID, FCSHORT portNum, bool bSuccess, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This function sends the result of a player attempting to uninstall software from a specific port. The packet sent is: __FCPKT_SOFTWARE_UNINSTALL_RESP
 *  @param portNum The port number that the uninstall was attempted on
 */
void SendSoftwareUninstallResponse(FCSHORT portNum, bool bSuccess, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This functions sends teh result of a player attempting to enable/disable a specific port. The packet sent is: __FCPKT_SOFTWARE_NETWORK_PORT_ENABLE_RESP
 *  @param portNum The port number that the enable/disable was attempted on
 *  @param result The result of the action
 */
void SendNetworkPortEnabledResponse(FCSHORT portNum, bool bEnabled, FCSHORT result, BaseSocket* pRouter, FCSOCKET clientSocket);

/**
 *  @ingroup worldcomms
 *  @brief This functions sends the result of a player stopped a specific software app. The packet sent is: __FCPKT_SOFTWARE_STOPPED
 *  @param itemID The ID of the software item that was stopped
 *  @param cpuReleased The amount of CPU capacity released by stopping the software
 *  @param memReleased The amount of memory capacity released by stopping the software
 */
void SendSoftwareStoppedResponse(FCULONG itemID, FCSHORT cpuReleased, FCULONG memReleased, BaseSocket* pRouter, FCSOCKET clientSocket);

/*************************************************************************************************************** 
 *  MISCELLANEOUS SEND METHODS
 ***************************************************************************************************************/

/**
 *	@ingroup worldcomms
 *	@brief This function sends a notification of XP gain (or loss) to a player. The packet sent is: __FCPKT_XP_GAIN
 *	@param xpGained Amount of xp gained by the player
 */
void SendXPGained(FCSHORT xpGained, FCULONG xpTotal, BaseSocket* pRouter, FCSOCKET clientSocket);

#endif//_WORLD_COMMS_H_