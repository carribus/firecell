#ifndef _CHATROOMMANAGER_H_
#define _CHATROOMMANAGER_H_

#include <map>
#include <string>
#include <vector>
#include "../common/fctypes.h"
#include "ChatRoom.h"
#include "Player.h"

class ChatRoomManager
{
public:
  ChatRoomManager(void);
  ~ChatRoomManager(void);

  ChatRoom* CreateChatRoom(FCULONG id, std::string name, bool isPrivate, FCULONG minCharLevel, FCULONG minAccType, bool isOfficial);
  ChatRoom* GetChatRoomByName(std::string name);
  ChatRoom* GetChatRoomByID(FCULONG id);

  /// return a copy of the chat rooms that a player is able to see
  size_t getChatRoomsForPlayer(vector<ChatRoom>& target, Player* pPlayer);

private:

  typedef std::vector< ChatRoom* > ChatRoomVector;
  ChatRoomVector m_chatRooms;

  /*
   * Map of Chat rooms by their ID
   */
  typedef std::map< FCULONG, ChatRoom* > ChatRoomIDMap;
  ChatRoomIDMap m_mapIDs;

  /*
   * Map of Chat rooms by their name
   */
  typedef std::map< std::string, ChatRoom* > ChatRoomNameMap;
  ChatRoomNameMap m_mapNames;
};

#endif//_CHATROOMMANAGER_H_