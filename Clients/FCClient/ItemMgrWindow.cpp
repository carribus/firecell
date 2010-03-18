#include "../common/ResourceManager.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "clientstrings.h"
#include "ItemMgr.h"
#include "ItemMgrWindow.h"

DECLARE_FORM_ELEMENTS(ItemMgrWindow)
  FORM_ELEMENT("staticText"			, 1							, 10			, 40			, 70			, 25			, L"")
  FORM_ELEMENT("hyperlink"			, 3             , 85			, 44			, 250			, 20			, L"")
  FORM_ELEMENT("checkBox"       , 50            , 340     , 40      , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 51            , 380     , 40      , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 52            , 380     , 53      , 125     , 12      , L"")
END_FORM_ELEMENTS();

IMPLEMENT_IRRLICHT_FORM(ItemMgrWindow);

///////////////////////////////////////////////////////////////////////

ItemMgrWindow::ItemMgrWindow(IDesktop* pDesktop, FCModel& model, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, false, id, rect<s32>(0, 0, 530, 340))
, m_model(model)
, m_pDesktop(pDesktop)
, m_pController(NULL)
{
  createFormElements();

  FCDialog::setSuccessCallback(OnOK, this);

  CenterWindow();
}

///////////////////////////////////////////////////////////////////////

ItemMgrWindow::~ItemMgrWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

void ItemMgrWindow::OnOK(void* pCtx)
{
}

///////////////////////////////////////////////////////////////////////

void ItemMgrWindow::OnCancel(void* pCtx)
{
}
