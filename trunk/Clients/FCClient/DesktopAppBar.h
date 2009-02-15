#ifndef _DESKTOPAPPBAR_H_
#define _DESKTOPAPPBAR_H_

#include <vector>
#include <irrlicht.h>
#include "InGameAppWindow.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class DesktopAppBar : public IGUIElement
{
public:
  DesktopAppBar(IGUIEnvironment* env, IGUIElement* pParent = 0, s32 id = -1);
  ~DesktopAppBar(void);

  void setActiveApp(InGameAppWindow* pApp);

  void draw();
	bool OnEvent(const SEvent& event);

  void setHeight(u32 height)                                { m_height = height; }
  u32 getHeight()                                           { return m_height; }

private:

	void drawSystemContext(IVideoDriver* pVideo);
	void drawActiveAppContext();
	void drawClock(IVideoDriver* pVideo);

	void addApplicationOptions();
	void removeApplicationOptions();

	void showSystemMenu();

  u32                 m_height;

  IGUIFont*           m_pClockFont;
  InGameAppWindow*    m_pActiveApp;
	rect<s32>						m_rectSystemCtx;

	SColor							m_colShaderNormal;
	SColor							m_colShaderHilite;

	struct AppBarOption
	{
		FCULONG id;
		bool bAppOption;
		bool bHighlight;
		rect<s32> rect;
		std::wstring str;
	};
	typedef vector<AppBarOption> AppBarOptionVector;
	AppBarOptionVector	m_appBarOptions;
};

#endif//_DESKTOPAPPBAR_H_