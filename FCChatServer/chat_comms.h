/**
 *  @defgroup chatcomms Chat Server Comms
 *  @brief This file contains packet sending functions for use by the FireCell Chat Service
 *  @ingroup chatcomms
 */
#ifndef _CHAT_COMMS_H_
#define _CHAT_COMMS_H_

#include <vector>
#include "../common/protocol/fcprotocol.h"
#include "ChatRoom.h"
#include "Player.h"

/**
 *  @ingroup chatcomms
 *  @brief This function sends the available chat rooms to the player. The packet sent is: __FCPKT_CHAT_LIST_ROOMS_RESP
 *  @param chatrooms A std::vector of ChatRoom* pointers
 */
void SendChatRoomList(vector< ChatRoom >& chatRooms, BaseSocket* pRouter, FCSOCKET clientSocket);

#endif//_CHAT_COMMS_H_
