#include "ChatChannel.h"

ChatChannel::ChatChannel(FCULONG id, const char* name, const char* topic, bool hasPassword, bool is_private, FCULONG min_char_level, bool is_official)
: m_id(id)
, m_name(name)
, m_topic(topic)
, m_hasPassword(hasPassword)
, m_isPrivate(is_private)
, m_minCharLevel(min_char_level)
, m_isOfficial(is_official)
{
}

ChatChannel::~ChatChannel(void)
{
}
