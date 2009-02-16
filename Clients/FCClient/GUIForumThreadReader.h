#ifndef _GUIFORUMTHREADREADER_H_
#define _GUIFORUMTHREADREADER_H_

#include <irrlicht.h>
#include "ForumModel.h"
#include "ForumThread.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class GUIForumThreadReader : public IGUIElement
{
public:
  GUIForumThreadReader(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 255, 255, 255), s32 id = -1);
  ~GUIForumThreadReader(void);

  void updateThreadContent(ForumThread* pThread);

  bool OnEvent(const SEvent& event);
  void draw();

  void setModel(ForumModel* pModel)                               { m_pModel = pModel; }
  void setCategoryID(FCULONG category_id)                         { m_category_id = category_id; }
  void setThreadID(FCULONG thread_id)                             { m_thread_id = thread_id; }
  FCULONG getCategoryID()                                         { return m_category_id; }
  FCULONG getThreadID()                                           { return m_thread_id; }

private:

  ForumModel*                         m_pModel;
  SColor                              m_backColor;
  SColor                              m_textColor;
	IGUIScrollBar*		                  m_pSB;
  u32                                 m_pixelUnderflow;

  FCULONG                             m_category_id,
                                      m_thread_id;

  struct _ThreadStruct
  {
    ForumThread* pThread;
    IGUIStaticText* pTxtContent;
    rect<s32> rect;
  };
  vector<_ThreadStruct>               m_posts;
};

#endif//_GUIFORUMTHREADREADER_H_