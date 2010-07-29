#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif
#include "ItemHelper.h"
#include "AppWindowMgr.h"

AppWindowMgr::AppWindowMgr(QObject *parent)
: QObject(parent)
{
}

///////////////////////////////////////////////////////////////////////

AppWindowMgr::~AppWindowMgr()
{
}

///////////////////////////////////////////////////////////////////////

bool AppWindowMgr::openApplicationWindow(FCULONG itemID, QWidget* parent)
{
  AppWindow* pWnd = getWindowForApp(itemID);

  if ( !pWnd )
  {
    pWnd = createApplicationWindow(itemID, parent);
  }
  else
  {
    pWnd->isVisible() ? pWnd->hide() : pWnd->show();
  }

  return false;
}

///////////////////////////////////////////////////////////////////////

AppWindow* AppWindowMgr::getWindowForApp(FCULONG itemID)
{
  AppWindow* pWnd = NULL;
  WindowMap::iterator it = m_mapWindows.find(itemID);

  if ( it != m_mapWindows.end() )
    pWnd = *it;

  return pWnd;

}

///////////////////////////////////////////////////////////////////////

AppWindow* AppWindowMgr::createApplicationWindow(FCULONG itemID, QWidget* parent)
{
  AppWindow* pWnd = new AppWindow(parent);

  if ( pWnd )
  {
    ItemSoftware* pSoftware = ItemHelper::getSoftwareItem(itemID);

    if ( pSoftware )
    {
      IGameAppLogic* pLogic = m_logicFactory.createLogicForApp(pSoftware);

      if ( pLogic )
      {
        pLogic->create(pWnd);
        // finally, assign the new window to our map
        m_mapWindows[itemID] = pWnd;
      }
    }
  }

  return pWnd;
}