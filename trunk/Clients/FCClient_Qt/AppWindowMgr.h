#ifndef APPWINDOWMGR_H
#define APPWINDOWMGR_H

#include <QObject>
#include <QMap>
#include "../../common/fctypes.h"
#include "AppWindow.h"
#include "AppLogicFactory.h"

class AppWindowMgr : public QObject
{
  Q_OBJECT

  typedef QMap<FCULONG, AppWindow*>     WindowMap;

public:
  AppWindowMgr(QObject *parent);
  ~AppWindowMgr();

  bool openApplicationWindow(FCULONG itemID, QWidget* parent);
  AppWindow* getWindowForApp(FCULONG itemID);

protected:

  virtual AppWindow* createApplicationWindow(FCULONG itemID, QWidget* parent);

private:

  AppLogicFactory         m_logicFactory;
  WindowMap               m_mapWindows;
};

#endif // APPWINDOWMGR_H
