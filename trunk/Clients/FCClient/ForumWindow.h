#ifndef _FORUMWINDOW_H_
#define _FORUMWINDOW_H_

#include <string>
#include "FCModel.h"
#include "InGameAppWindow.h"
#include "GUIForumCatBrowser.h"

class ForumWindow : public InGameAppWindow
{
public:
  ForumWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice);
  ~ForumWindow(void);

  bool Create(s32 AppElemID, FCUINT optionID, std::wstring caption);

  const wchar_t* getAppName()                             { return L"Forum Browser"; }

private:

  /*
   *  Private Methods
   */
  void RequestForumCategoryRefresh();

  /*
   *  Private Members
   */
  IrrlichtDevice*         m_pDevice;
  GUIForumCatBrowser*     m_pForumCatBrowser;
};

#endif//_FORUMWINDOW_H_