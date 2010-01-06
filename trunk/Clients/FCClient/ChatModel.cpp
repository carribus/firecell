#include "ChatModel.h"

ChatModel::ChatModel(void)
{
}

///////////////////////////////////////////////////////////////////////

ChatModel::~ChatModel(void)
{
  deleteAllChannels();
}

///////////////////////////////////////////////////////////////////////

ChatModel& ChatModel::instance()
{
  static ChatModel cm;

  return cm;
}

///////////////////////////////////////////////////////////////////////

void ChatModel::initialise()
{
  // TODO: Initialise the model (if needed)
}

///////////////////////////////////////////////////////////////////////

bool ChatModel::updateChannel(FCULONG id, const char* name, const char* topic, bool hasPassword, bool is_private, FCULONG min_char_level, bool is_official)
{
  bool bResult = false;

  m_mutexChannels.Lock();
  {
    ChannelMap::iterator it = m_mapChannels.find(id);
    ChatChannel* pChannel = NULL;

    if ( it == m_mapChannels.end() )
    {
      if ( (pChannel = new ChatChannel(id, name, topic, hasPassword, is_private, min_char_level, is_official)) )
      {
        m_mapChannels[id] = pChannel;
        bResult = true;
      }
    }
    else
    {
      pChannel = it->second;
      if ( pChannel )
      {
        pChannel->setName(name);
        pChannel->setTopic(topic);
        pChannel->hasPassword(hasPassword);
        pChannel->isPrivate(is_private);
        pChannel->setMinCharLevel(min_char_level);
        pChannel->isOfficial(is_official);
        bResult = true;
      }
    }
  }
  m_mutexChannels.Unlock();

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void ChatModel::deleteAllChannels()
{
  ChannelMap::iterator it = m_mapChannels.begin();
  ChannelMap::iterator limit = m_mapChannels.end();

  for ( ; it != limit; ++it )
  {
    if ( it->second )
      delete it->second;
  }

  m_mapChannels.erase( m_mapChannels.begin(), limit );
}