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

  /**
   *  @brief Used to create an in-game window for a specific application type
   *
   *  @param itemID The ID of the [software] item to create the window for
   *  @param parent The parent (if any) of the new window
   */
  bool openApplicationWindow(FCULONG itemID, QWidget* parent);

  /**
   *  @brief Searches the map of open in-game windows for an existing window for the specific [software] item
   *
   *  @param itemID The ID of the [software] item to create the window for
   */
  AppWindow* getWindowForApp(FCULONG itemID);

protected:

  /**
   *  @brief The function handles the details of creating in-game windows for [software] items.
   *
   *  @param itemID The ID of the [software] item to create the window for
   *  @param parent The parent (if any) of the new window
   */
  virtual AppWindow* createApplicationWindow(FCULONG itemID, QWidget* parent);

private:

  AppLogicFactory         m_logicFactory;
  WindowMap               m_mapWindows;
};

#endif // APPWINDOWMGR_H
