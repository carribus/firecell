#ifndef _CHATMODEL_H_
#define _CHATMODEL_H_

#include "../../common/fctypes.h"

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

};

#endif//_CHATMODEL_H_