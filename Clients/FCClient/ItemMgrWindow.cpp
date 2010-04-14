#include <sstream>
#include "../common/ResourceManager.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "clientstrings.h"
#include "ItemMgr.h"
#include "ItemMgrWindow.h"

#ifdef _DEBUG
#include "../common/irrlichtUtil/irrDiag.h"
#endif//_DEBUG

DECLARE_FORM_ELEMENTS(ItemMgrWindow)
  FORM_ELEMENT("listBox"			, 1							, 10			, 40			, 300			, 200			, L"")
/*
  FORM_ELEMENT("staticText"			, 1							, 10			, 40			, 70			, 25			, L"")
  FORM_ELEMENT("hyperlink"			, 3             , 85			, 44			, 250			, 20			, L"")
  FORM_ELEMENT("checkBox"       , 50            , 340     , 40      , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 51            , 380     , 40      , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 52            , 380     , 53      , 125     , 12      , L"")
*/
  FORM_ELEMENT("button"					, FCBTN_OK			, 380			, 290			, 125     , 25			, L"")
END_FORM_ELEMENTS();

IMPLEMENT_IRRLICHT_FORM(ItemMgrWindow);

///////////////////////////////////////////////////////////////////////

ItemMgrWindow::ItemMgrWindow(IDesktop* pDesktop, FCModel& model, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, false, id, rect<s32>(0, 0, 530, 340))
, m_model(model)
, m_pDesktop(pDesktop)
, m_pController(NULL)
{
//  createFormElements();

  pEnv->loadGUI("./clientdata/ui/guiTest.xml", this);
#ifdef _DEBUG
  irrDiag diag;
  std::string dump = diag.dumpElementTree(m_pEnv->getRootGUIElement());
  OutputDebugString( dump.c_str() );
#endif//_DEBUG

  FCDialog::setSuccessCallback(OnOK, this);

  UpdateUIFromModel();

  IGUIElement* pElem = NULL;
  if ((pElem = getElementFromId(FCBTN_OK)))
    pElem->setText(ResourceManager::instance().GetClientString(STR_DLG_CLOSE).c_str());

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

///////////////////////////////////////////////////////////////////////

void ItemMgrWindow::UpdateUIFromModel()
{
  FCModel& model = FCModel::instance();
  ItemMgr& mgr = model.GetItemMgr();
  IGUIListBox* pList = (IGUIListBox*)getElementFromId(1);
  std::vector<ItemMgr::GameItem> items;
  std::wstringstream ss;

  mgr.getItems(items);
  if ( pList )
  {
    std::vector<ItemMgr::GameItem>::iterator it = items.begin();
    std::vector<ItemMgr::GameItem>::iterator limit = items.end();
    Item* pItem = NULL;
    irr::u32 itemNdx = 0;

    for ( ; it != limit; ++it )
    {
      pItem = it->getItem();
      if ( pItem && it->getCount() > 0 )
      {
        ss << pItem->GetName().c_str();
        itemNdx = pList->addItem( ss.str().c_str() );
        ss.str(L"");
      }
    }
  }
}
