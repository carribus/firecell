#ifndef _INGAMEAPPWINDOW_H_
#define _INGAMEAPPWINDOW_H_

#include <irrlicht.h>
#include <string>
#include "../../common/fctypes.h"
#include "../../common/protocol/fcprotocol.h"

using namespace irr;
using namespace gui;

class FCController;

class InGameAppWindow
{
public:
	
	InGameAppWindow(FCController* pController, IGUIEnvironment* pEnv);
	virtual ~InGameAppWindow(void);

	virtual bool Create(FCUINT optionID, DesktopOptionType type, std::wstring caption);
	void GetClientRect(core::rect<s32>& rect);

	FCUINT GetOptionID()												{ return m_optionID; }
	DesktopOptionType GetAppType()							{ return m_type; }
	bool IsWaitingForResponse()									{ return m_bWaitingForResponse; }

protected:
	
	FCController*						m_pController;
	IGUIEnvironment*				m_pEnv;
	IGUIWindow*							m_pWindow;
	FCUINT									m_optionID;
	DesktopOptionType				m_type;
	bool										m_bWaitingForResponse;
};

#endif//_INGAMEAPPWINDOW_H_