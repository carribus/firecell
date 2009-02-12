#ifndef _GUIFORUMTHREADBROWSER_H_
#define _GUIFORUMTHREADBROWSER_H_

#include <string>
#include <vector>
#include <irrlicht.h>
#include "FCDialog.h"
#include "ForumCategory.h"
#include "ForumModel.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

struct IForumThreadBrowserSink
{
	virtual void OnThreadSelected(FCULONG category_id, FCULONG thread_id) = 0;
  virtual void OnThreadViewClosed() = 0;
};

class GUIForumThreadBrowser : public IGUIElement
{
public:
  GUIForumThreadBrowser(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 255, 255, 255), s32 id = -1);
  ~GUIForumThreadBrowser(void);

  void clearThreads();
  void updateCategoryThreads(FCULONG category_id);
  void registerSink(IForumThreadBrowserSink* pSink)                     { m_pSink = pSink; }
  void setModel(ForumModel* pModel)                                     { m_pModel = pModel; }
  void setCategory(ForumCategory* pCat)                                 { m_currentCategory = pCat; }

  bool OnEvent(const SEvent& event);
		bool OnLButtonDown(const SEvent::SMouseInput& event);
    bool OnLButtonUp(const SEvent::SMouseInput& event);
		bool OnLButtonDblClick(const SEvent::SMouseInput& event);
  void draw();

private:

	struct ForumThreadStruct
	{
		ForumThread* pThread;
		bool bHighlighted;
		rect<s32> rect;
	};

	void drawOptionBar(IVideoDriver* pVideo);
	void drawHeader(IVideoDriver* pVideo);
	void drawThread(ForumThreadStruct& thread);

  void addThread(ForumThread* pThread);

  void OnOptionButtonPressed(int index);

	void OpenNewThreadWindow();
		static void onNewThreadDlgComplete(void* pParam);
		static void onNewThreadDlgCancel(void* pParam);

  /**
   *  @brief Loops through all ForumCatStruct objects in the m_categories vector and updates the highlight flag based on the mouse position
   */
  void checkHighlights();

  ForumModel*                         m_pModel;
  IForumThreadBrowserSink*            m_pSink;
  SColor                              m_backColor;
  SColor                              m_textColor;
	IGUIScrollBar*		                  m_pSB;
	ForumCategory*		                  m_currentCategory;
  bool                                m_bOverflow;
  u32                                 m_pixelOverflow;
  u32                                 m_pixelUnderflow;
  position2d<s32>                     m_mousePos;
	FCDialog*														m_pActiveDialog;

	typedef std::vector<ForumThreadStruct> ThreadVector;
	ThreadVector                        m_threads;

	struct ThreadOption
	{
		bool bVisible;
    bool bPressed;
		std::wstring label;
		core::rect<s32> rectBtn;
	};
	ThreadOption											m_optionBtns[4];

  /*
   *  Double click tracking
   */
	struct LastClick
	{
		u32 last_tick;
		s32 lastX;
		s32	lastY;
	};
	LastClick										m_LButtonLastClick;
};

#endif//_GUIFORUMTHREADBROWSER_H_