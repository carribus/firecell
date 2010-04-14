#include "StdAfx.h"
#include <deque>
#include "irrDiag.h"
#include "irrDesignerView.h"

irrDesignerView::irrDesignerView(void)
: m_pDev(NULL)
, m_pDriver(NULL)
, m_pEnv(NULL)
, m_bDrawGrid(true)
, m_bSnapToGrid(true)
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

    if ( m_bDrawGrid )
      renderGrid();
    m_pEnv->drawAll();

    m_pDriver->endScene();
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////

void irrDesignerView::renderGrid(u32 gapX, u32 gapY)
{
  dimension2d<s32> screenDim = m_pDriver->getScreenSize();

  for ( s32 i = gapX; i < screenDim.Width; i += gapX )
    m_pDriver->draw2DLine( position2d<s32>(i, 0), position2d<s32>(i, screenDim.Height), SColor(32, 255, 255, 255) );

  for ( s32 i = gapY; i < screenDim.Height; i += gapY )
    m_pDriver->draw2DLine( position2d<s32>(0, i), position2d<s32>(screenDim.Width, i), SColor(32, 255, 255, 255) );
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

  case  EET_GUI_EVENT:
    bResult = OnGUIEvent(event.GUIEvent);
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

  m_lastClickPos = position2d<s32>(X, Y);

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

//////////////////////////////////////////////////////////////////////////////

bool irrDesignerView::OnGUIEvent(const SEvent::SGUIEvent event)
{
  bool bResult = false;

  switch ( event.EventType )
  {
  case  EGET_MENU_ITEM_SELECTED:
    {
      IGUIContextMenu* pMenu = (IGUIContextMenu*)event.Caller;
      s32 selItem = pMenu->getSelectedItem();
      bResult = OnMenuItemSelected(selItem, pMenu);
    }
    break;

  default:
    break;
  }

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////

bool irrDesignerView::OnMenuItemSelected(s32 selectedItem, IGUIContextMenu* pMenu)
{
  // at the moment, we assume that the only option available to create objects...
  s32 factoryIx = pMenu->getID();
  IGUIElementFactory* pFactory = m_pEnv->getGUIElementFactory(factoryIx);
  std::wstringstream ss;

  if ( pFactory )
  {
    IGUIElement* pNewElem = pFactory->addGUIElement( pFactory->getCreateableGUIElementType(selectedItem), m_pEnv->getRootGUIElement() );
    
    if ( pNewElem )
    {
      // TODO: Instead of creating the actual selected element, we need to create an instance of ElementContainer. It must then contain an instance of the object we selected to create
      pNewElem->setRelativePosition( rect<s32>( m_lastClickPos.X, m_lastClickPos.Y, m_lastClickPos.X + 100, m_lastClickPos.Y + 50 ) );
      ss << pFactory->getCreateableGUIElementTypeName( selectedItem );
      pNewElem->setText( ss.str().c_str() );
    }
  }

  return true;
}