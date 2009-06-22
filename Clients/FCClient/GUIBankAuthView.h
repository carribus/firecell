#ifndef _GUIBANKAUTHVIEW_H_
#define _GUIBANKAUTHVIEW_H_

#include <irrlicht.h>
#include "BankModel.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class BankingWindow;

class GUIBankAuthView : public IGUIElement
{
public:
  GUIBankAuthView(BankingWindow* pOwner, IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, s32 id = -1);
  ~GUIBankAuthView(void);

  /*
   *  IGUIElement overrides
   */
  void draw();
  bool OnEvent(const SEvent& event);

private:

  BankingWindow*          m_pOwner;
  IGUIEditBox*            m_pEdtPassword;
};

#endif//_GUIBANKAUTHVIEW_H_