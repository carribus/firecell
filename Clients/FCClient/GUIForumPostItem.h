#ifndef _GUIFORUMPOSTITEM_H_
#define _GUIFORUMPOSTITEM_H_

#include <irrlicht.h>
#include "ForumThread.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class GUIForumPostItem : public IGUIElement
{
public:
	GUIForumPostItem(IGUIEnvironment* environment, IGUIElement* pParent, core::rect<s32> rect, SColor textCol, SColor bkgCol, s32 id = -1);
	~GUIForumPostItem(void);

	void setItemData(ForumThread* pThread);

	void draw();
	bool OnEvent(const SEvent& event);

private:

	SColor									m_colText;
	SColor									m_colBkg;
	ForumThread*						m_pThread;

	IGUIStaticText*					m_pTxtAuthor;
	IGUIStaticText*					m_pTxtSubject;
	IGUIStaticText*					m_pTxtContent;
	IGUIButton*							m_pBtnAccept;
};

#endif//_GUIFORUMPOSTITEM_H_