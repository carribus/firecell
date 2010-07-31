#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif
#include <QLayout>
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
    if ( (pWnd = createApplicationWindow(itemID, parent)) )
    {
      pWnd->show();
    }
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
  ItemSoftware* pSoftware = ItemHelper::getSoftwareItem(itemID);
  AppWindow* pWnd = NULL;

  if ( pSoftware )
  {
    pWnd = new AppWindow(parent);

    if ( pWnd )
    {
      IGameAppLogic* pLogic = m_logicFactory.createLogicForApp(pSoftware);

      if ( pLogic )
      {
        pLogic->create(NULL);
        QWidget* pChildWnd = pLogic->getWidget();
        pWnd->layout()->addWidget(pChildWnd);
        // finally, assign the new window to our map
        m_mapWindows[itemID] = pWnd;
      }

      pWnd->setWindowFlags( Qt::Window );
    }
  }

  return pWnd;
}