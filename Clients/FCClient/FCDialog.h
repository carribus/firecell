#ifndef _FCDIALOG_H_
#define _FCDIALOG_H_

#include <irrlicht.h>

using namespace irr;
using namespace gui;

class FCDialog : public IGUIElement
{
public:
	FCDialog(IGUIEnvironment* pEnv);
	~FCDialog(void);

	bool Create(core::rect<s32> rect, const wchar_t* caption, bool bModal = false, IGUIElement* pParent = NULL, s32 id = -1);
	bool Create(s32 id, const char* layoutXmlFile, bool bModal = false, IGUIElement* pParent = NULL);
	void GetClientRect(core::rect<s32>& rect);
  void CenterWindow();

	bool OnEvent(const SEvent& event);

private:

	IGUIEnvironment*				m_pEnv;
	IGUIWindow*							m_pWindow;
	bool										m_bModal;
};

#endif//_FCDIALOG_H_