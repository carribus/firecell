#ifndef _GUIFORUMTHREADREADER_H_
#define _GUIFORUMTHREADREADER_H_

#include <irrlicht.h>
#include "FCDialog.h"
#include "ForumModel.h"
#include "ForumThread.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

struct IForumThreadReaderSink
{
  virtual void OnThreadReaderViewClosed() = 0;
  virtual void OnThreadReply(FCULONG category_id, FCULONG thread_id, std::wstring& subject, std::wstring& message) = 0;
	virtual void OnThreadMissionAccept(FCULONG mission_id) = 0;
};

class GUIForumThreadReader : public IGUIElement
{
public:
  GUIForumThreadReader(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 255, 255, 255), s32 id = -1);
  ~GUIForumThreadReader(void);

	void clearPosts();
  void updateThreadContent(ForumThread* pThread);
  void registerSink(IForumThreadReaderSink* pSink)               { m_pSink = pSink; }

  bool OnEvent(const SEvent& event);
		bool OnLButtonDown(const SEvent::SMouseInput& event);
    bool OnLButtonUp(const SEvent::SMouseInput& event);
  void draw();

  void setModel(ForumModel* pModel)                               { m_pModel = pModel; }
  void setCategoryID(FCULONG category_id)                         { m_category_id = category_id; }
  void setThreadID(FCULONG thread_id)                             { m_thread_id = thread_id; }
  FCULONG getCategoryID()                                         { return m_category_id; }
  FCULONG getThreadID()                                           { return m_thread_id; }

private:

	void drawOptionBar(IVideoDriver* pVideo);
	void drawHeader(IVideoDriver* pVideo);

	void addThreadPost(ForumThread* pPost, core::rect<s32>& rect, SColor colBkg, SColor colText);
  void OnOptionButtonPressed(int index);

	void OpenNewReplyWindow();
		static void onReplyDlgComplete(void* pParam);
		static void OnReplyDlgCancel(void* pParam);

  ForumModel*                         m_pModel;
  IForumThreadReaderSink*							m_pSink;
  SColor                              m_backColor;
  SColor                              m_textColor;
	IGUIScrollBar*		                  m_pSB;
  u32                                 m_pixelUnderflow;
	FCDialog*														m_pActiveDialog;

  FCULONG                             m_category_id,
                                      m_thread_id;
/*
  struct _ThreadStruct
  {
    ForumThread* pThread;
    IGUIStaticText* pTxtContent;
    rect<s32> rect;
  };
*/
  vector<IGUIElement*>               m_posts;

	struct ThreadOption
	{
		bool bVisible;
    bool bPressed;
		std::wstring label;
		core::rect<s32> rectBtn;
	};
	ThreadOption											m_optionBtns[4];
};

#endif//_GUIFORUMTHREADREADER_H_