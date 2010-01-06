#ifndef _CHATCHANNEL_H_
#define _CHATCHANNEL_H_

#include <string>
#include "../../common/fctypes.h"

class ChatChannel
{
public:
  ChatChannel(FCULONG id, const char* name, const char* topic, bool hasPassword, bool is_private, FCULONG min_char_level, bool is_official);
  ~ChatChannel(void);

  FCULONG getID()                           { return m_id; }
  void setID(FCULONG id)                    { m_id = id; }
  std::string getName()                     { return m_name; }
  void setName(const std::string name)      { m_name = name; }
  std::string getTopic()                    { return m_topic; }
  void setTopic(const std::string topic)    { m_topic = topic; }
  bool hasPassword()                        { return m_hasPassword; }
  void hasPassword(bool bFlag)              { m_hasPassword = bFlag; }
  bool isPrivate()                          { return m_isPrivate; }
  void isPrivate(bool bPrivate)             { m_isPrivate = bPrivate; }
  FCULONG getMinCharLevel()                 { return m_minCharLevel; }
  void setMinCharLevel(FCULONG minLvl)      { m_minCharLevel = minLvl; }
  bool isOfficial()                         { return m_isOfficial; }
  void isOfficial(bool bOfficial)           { m_isOfficial = bOfficial; }

private:

  FCULONG m_id;
  std::string m_name;
  std::string m_topic;
  bool m_hasPassword;
  bool m_isPrivate;
  FCULONG m_minCharLevel;
  bool m_isOfficial;
};

#endif//_CHATCHANNEL_H_