#ifndef _GUIFORUMTHREADBROWSER_H_
#define _GUIFORUMTHREADBROWSER_H_

#include <string>
#include <vector>
#include <irrlicht.h>
#include "ForumCategory.h"
#include "ForumModel.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

struct IForumThreadBrowserSink
{
	virtual void OnThreadSelected(FCULONG category_id, FCULONG thread_id) = 0;
};

class GUIForumThreadBrowser : public IGUIElement
{
public:
  GUIForumThreadBrowser(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 255, 255, 255), s32 id = -1);
  ~GUIForumThreadBrowser(void);

  void updateCategoryThreads(FCULONG category_id);
  void registerSink(IForumThreadBrowserSink* pSink)                     { m_pSink = pSink; }
  void setModel(ForumModel* pModel)                                     { m_pModel = pModel; }
  void setCategory(ForumCategory* pCat)                                 { m_currentCategory = pCat; }

  void draw();

private:

	struct ForumThreadStruct
	{
		ForumThread* pThread;
		bool bHighlighted;
		rect<s32> rect;
	};

	void drawHeader(IVideoDriver* pVideo);
	void drawThread(const ForumThreadStruct& thread);

  void addThread(ForumThread* pThread);

  ForumModel*                         m_pModel;
  IForumThreadBrowserSink*            m_pSink;
  SColor                              m_backColor;
  SColor                              m_textColor;
	ForumCategory*		                  m_currentCategory;

	typedef std::vector<ForumThreadStruct> ThreadVector;
	ThreadVector                        m_threads;

};

#endif//_GUIFORUMTHREADBROWSER_H_