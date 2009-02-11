#ifndef _GUIFORUMCATBROWSER_H_
#define _GUIFORUMCATBROWSER_H_

#include <string>
#include <vector>
#include <irrlicht.h>
#include "ForumCategory.h"
#include "ForumModel.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

struct IForumCatBrowserSink
{
	virtual void OnCategorySelected(FCULONG category_id) = 0;
};

class GUIForumCatBrowser : public IGUIElement
{
public:
  GUIForumCatBrowser(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 255, 255, 255), s32 id = -1);
  ~GUIForumCatBrowser(void);

	void registerSink(IForumCatBrowserSink* pSink)									{ m_pSink = pSink; }
	void updateCategories();

  /*
   *  IGUIElement overrides
   */
  void draw();
  bool OnEvent(const SEvent& event);
		bool OnLButtonDown(const SEvent::SMouseInput& event);
		bool OnLButtonDblClick(const SEvent::SMouseInput& event);

	void setModel(ForumModel* pModel)																{ m_pModel = pModel; }

private:

	struct ForumCatStruct
	{
		ForumCategory* pCat;
		bool bExpanded;
		bool bHighlighted;
		rect<s32> rect;
	};

	void addCategory(ForumCategory* pCat);

	void drawHeader(IVideoDriver* pVideo);
	void drawForumCategory(ForumCatStruct& fcs, s32 level = 0);
	void drawExpanderIcon(ForumCatStruct& fcs, s32 level, IVideoDriver* pVideo, IGUIFont* pFont);

  /**
   *  @brief Checks if drawing to the specified rectangle will cause an overflow.
   *
   *  @param rect Target rectangle that drawing will take place in
   *  @return number of pixels of overflop
   */
  u32 checkForOverflow(core::rect<s32> rect);

  /**
   *  @brief Loops through all ForumCatStruct objects in the m_categories vector and updates the highlight flag based on the mouse position
   */
  void checkHighlights();

  /**
   *  @brief Utility function to quickly offset a rectangle's position by a specific XY offset
   */
  void offsetRect(core::rect<s32>& rect, s32 xOffs, s32 yOffs);

	IForumCatBrowserSink*	m_pSink;

	ForumModel*				m_pModel;
  SColor            m_backColor;
  SColor            m_textColor;
  IGUIFont*         m_pFontDesc;
	IGUIScrollBar*		m_pSB;
	ForumCatStruct*		m_currentCategory;
	position2d<s32>		m_offset;
  bool              m_bOverflow;
  u32               m_pixelOverflow;
  u32               m_pixelUnderflow;
  position2d<s32>   m_mousePos;

	typedef std::vector<ForumCatStruct> CategoryVector;
	CategoryVector		m_categories;

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

#endif//_GUIFORUMCATBROWSER_H_