#include "StdAfx.h"
#include <deque>
#include "irrDiag.h"
#include "irrDesignerView.h"

irrDesignerView::irrDesignerView(void)
: m_pDev(NULL)
, m_pDriver(NULL)
, m_pEnv(NULL)
{
}

//////////////////////////////////////////////////////////////////////////////

irrDesignerView::~irrDesignerView(void)
{
  if ( m_pDev )
    m_pDev->drop();
}

//////////////////////////////////////////////////////////////////////////////

bool irrDesignerView::create(s32 width, s32 height, SColor background)
{
  bool bResult = false;
  if ( (m_pDev = createDevice(EDT_OPENGL, dimension2d<s32>(width, height), 32, false, false, false, this)) )
  {
    m_pDriver = m_pDev->getVideoDriver();
    m_pEnv = m_pDev->getGUIEnvironment();
    m_pDev->setResizeAble(true);
    m_pEnv->setUserEventReceiver(this);

    // configure the skin
    IGUISkin* pSkin = m_pEnv->createSkin(EGST_BURNING_SKIN);
    IGUIFont* pFontCourier = m_pEnv->getFont("fontcourier.png");
    if ( pSkin )
    {
      pSkin->setFont(pFontCourier);
      m_pEnv->setSkin(pSkin);
      pSkin->drop();
    }

    m_colBackground = background;
    bResult = true;
  }

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////

u32 irrDesignerView::exec()
{
  if ( !m_pDev || !m_pDriver || !m_pEnv )
    return 1;

  while ( m_pDev->run() )
  {
    m_pDriver->beginScene(true, true, m_colBackground);

    m_pEnv->drawAll();

    m_pDriver->endScene();
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
// if we handle an event specifically, set bResult = true;
//////////////////////////////////////////////////////////////////////////////
bool irrDesignerView::OnEvent(const SEvent& event)
{
  bool bResult = false;

  switch ( event.EventType )
  {
  case  EET_MOUSE_INPUT_EVENT:
    bResult = OnMouseEvent(event.MouseInput);
    break;

  default:
    break;
  }

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////

bool irrDesignerView::OnMouseEvent(const SEvent::SMouseInput event)
{
  bool bResult = false;

  switch ( event.Event )
  {
  case  EMIE_RMOUSE_LEFT_UP:
    bResult = OnRButtonDown(event.X, event.Y);
    break;

  case  EMIE_LMOUSE_PRESSED_DOWN:
    OutputDebugString(L"LMOUSE DOWN\n");
    break;

  default:
    break;
  }

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////

bool irrDesignerView::OnRButtonDown(s32 X, s32 Y)
{
  IGUIElement* pRootElem = m_pEnv->getRootGUIElement();
  IGUIContextMenu* pMenu = m_pEnv->addContextMenu( rect<s32>(X, Y, X+300, Y+300), pRootElem );
  std::deque< IGUIContextMenu* > menuStack;
  IGUIElementFactory* pFactory = NULL;
  u32 itemIx = 0, factoryCount = m_pEnv->getRegisteredGUIElementFactoryCount();
  s32 elemCount = 0;
  std::wstringstream str;

  itemIx = pMenu->addItem(L"Create Object", -1, true, true);
  menuStack.push_back(pMenu);
  if ( (pMenu = pMenu->getSubMenu(itemIx)) )
  {
    // loop through the registered factories
    for ( u32 nFactory = 0; nFactory < factoryCount; nFactory++ )
    {
      // create the menu item
      str << L"Factory " << nFactory;
      itemIx = pMenu->addItem(str.str().c_str(), nFactory, true, true);
      menuStack.push_back(pMenu);
      if ( ( pMenu = pMenu->getSubMenu(itemIx) ) )
      {
        if ( (pFactory = m_pEnv->getGUIElementFactory(nFactory)) )
        {
          // loop through the creatable elements of the factory
          elemCount = pFactory->getCreatableGUIElementTypeCount();
          for ( s32 nElem = 0; nElem < elemCount; nElem++ )
          {
            str.str(L"");
            str << pFactory->getCreateableGUIElementTypeName(nElem);
            pMenu->addItem( str.str().c_str(), nElem );
          }
        }
      }
      pMenu = menuStack.back();
      menuStack.pop_back();
    }
  }

  pMenu = menuStack.back();
  menuStack.pop_back();

  m_pEnv->setFocus(pMenu);

#ifdef _DEBUG
  irrDiag diag;

  std::string s = diag.dumpElementTree(pRootElem);
  OutputDebugStringA(s.c_str());
#endif//_DEBUG

  return true;
}