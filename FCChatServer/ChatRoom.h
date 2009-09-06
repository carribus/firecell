#ifndef _CHATROOM_H_
#define _CHATROOM_H_

#include <string>
#include "../common/fctypes.h"

class ChatRoom
{
public:
  ChatRoom(FCULONG id, std::string name, bool isPrivate, FCULONG m_minCharLevel, FCULONG m_minAccType, bool isOfficial);
  ~ChatRoom(void);

  void setTopic(std::string topic)                              { m_topic = topic; }
  void setPassword(std::string password)                        { m_password = password; }

  FCULONG getID()                                               { return m_id; }
  std::string getName()                                         { return m_name; }
  std::string getTopic()                                        { return m_topic; }
  bool isPrivate()                                              { return m_isPrivate; }
  std::string getPassword()                                     { return m_password; }
  FCULONG getMinCharLevel()                                     { return m_minCharLevel; }
  FCULONG getMinAccType()                                       { return m_minAccType; }
  bool isOfficial()                                             { return m_isOfficial; }

private:

  FCULONG m_id;
  std::string m_name;
  std::string m_topic;
  bool m_isPrivate;
  std::string m_password;
  FCULONG m_minCharLevel;
  FCULONG m_minAccType;
  bool m_isOfficial;
};

#endif//_CHATROOM_H_