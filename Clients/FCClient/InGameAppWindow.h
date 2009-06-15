#ifndef _INGAMEAPPWINDOW_H_
#define _INGAMEAPPWINDOW_H_

#include <irrlicht.h>
#include <string>
#include "../../common/fctypes.h"
#include "../../common/protocol/fcprotocol.h"
#include "IDesktop.h"

using namespace irr;
using namespace gui;

class FCController;
class Desktop;

class InGameAppWindow
{
public:
	
	InGameAppWindow(IDesktop* pDesktop, FCController* pController, IGUIEnvironment* pEnv);
	virtual ~InGameAppWindow(void);

	virtual bool Create(s32 AppElemID, FCUINT optionID, FCULONG softwareType, std::wstring caption, bool bModal = false);
	void GetClientRect(core::rect<s32>& rect);
  void CenterWindow();

  /*
   * Pure Abstract Functions
   */
  virtual const wchar_t* getAppName() = 0;

	FCUINT GetOptionID()												        { return m_optionID; }
	FCULONG GetAppType()							                  { return m_type; }
  virtual void SetWaitingForResponse(bool bWait)      { m_bWaitingForResponse = bWait; }
	bool IsWaitingForResponse()									        { return m_bWaitingForResponse; }
	IGUIWindow* GetGUIWindow()													{ return m_pWindow; }

protected:
	
	FCController*						m_pController;
  IDesktop*               m_pDesktop;
	IGUIEnvironment*				m_pEnv;
	IGUIWindow*							m_pWindow;
	FCUINT									m_optionID;
	FCULONG         				m_type;
	bool										m_bWaitingForResponse;
};

#endif//_INGAMEAPPWINDOW_H_