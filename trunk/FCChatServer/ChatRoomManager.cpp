#include "ChatRoomManager.h"

ChatRoomManager::ChatRoomManager(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

ChatRoomManager::~ChatRoomManager(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

ChatRoom* ChatRoomManager::CreateChatRoom(FCULONG id, std::string name, bool isPrivate, FCULONG minCharLevel, FCULONG minAccType, bool isOfficial)
{
  ChatRoom* pRoom = NULL;

  if ( !(pRoom = GetChatRoomByID(id)) )
  {
    pRoom = new ChatRoom(id, name, isPrivate, minCharLevel, minAccType, isOfficial);

    m_chatRooms.push_back(pRoom);
    m_mapIDs[id] = pRoom;
    m_mapNames[name] = pRoom;
  }
  else
  {
    // Chat room already exists
    pRoom = NULL;
  }

  return pRoom;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

ChatRoom* ChatRoomManager::GetChatRoomByName(std::string name)
{
  ChatRoomNameMap::iterator it = m_mapNames.find(name);

  if ( it != m_mapNames.end() )
    return it->second;

  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

ChatRoom* ChatRoomManager::GetChatRoomByID(FCULONG id)
{
  ChatRoomIDMap::iterator it = m_mapIDs.find(id);

  if ( it != m_mapIDs.end() )
    return it->second;

  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

size_t ChatRoomManager::getChatRoomsForPlayer(vector<ChatRoom>& target, Player* pPlayer)
{
  size_t numRooms = 0;
  std::vector<ChatRoom*>::iterator it = m_chatRooms.begin();
  std::vector<ChatRoom*>::iterator limit = m_chatRooms.end();

  for ( ; it != limit; ++it )
  {
    if ( (*it)->getMinCharLevel() > pPlayer->GetLevel() )
      continue;

    target.push_back( *(*it) );
  }

  return target.size();
}