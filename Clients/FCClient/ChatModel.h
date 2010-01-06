#ifndef _CHATMODEL_H_
#define _CHATMODEL_H_

#include <map>
#include "../../common/fctypes.h"
#include "../../common/PThreadMutex.h"
#include "ChatChannel.h"

/**
 *	@class ChatModel
 *	@brief Data model for the Online Chat portion of FireCell
 */
class ChatModel
{
  ChatModel(void);
  ~ChatModel(void);

public:

	/**
	 *	@brief Get access to the singleton instance of the ChatModel
	 */
  static ChatModel& instance();

  void initialise();
  bool updateChannel(FCULONG id, const char* name, const char* topic, bool hasPassword, bool is_private, FCULONG min_char_level, bool is_official);

private:

  void deleteAllChannels();

  typedef std::map< FCULONG, ChatChannel* > ChannelMap;
  ChannelMap m_mapChannels;
  PThreadMutex m_mutexChannels;
};

#endif//_CHATMODEL_H_