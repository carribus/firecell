#ifndef _GUIBANKVIEW_H_
#define _GUIBANKVIEW_H_

#include <irrlicht.h>
#include "BankModel.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class GUIBankView : public IGUIElement
{
public:
  GUIBankView(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, s32 id = -1);
  ~GUIBankView(void);

  /*
   *  IGUIElement overrides
   */
  void draw();
  bool OnEvent(const SEvent& event);

private:


  BankModel*        m_pModel;
};

#endif//_GUIBANKVIEW_H_