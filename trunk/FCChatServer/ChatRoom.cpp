#include "ChatRoom.h"

ChatRoom::ChatRoom(FCULONG id, std::string name, bool isPrivate, FCULONG minCharLevel, FCULONG minAccType, bool isOfficial)
: m_id(id)
, m_name(name)
, m_isPrivate(isPrivate)
, m_minCharLevel(minCharLevel)
, m_minAccType(minAccType)
, m_isOfficial(isOfficial)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

ChatRoom::~ChatRoom(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

