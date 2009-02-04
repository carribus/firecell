#ifndef _FORUMWINDOW_H_
#define _FORUMWINDOW_H_

#include <string>
#include "FCModel.h"
#include "InGameAppWindow.h"

class ForumWindow : public InGameAppWindow
{
public:
  ForumWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice);
  ~ForumWindow(void);

  bool Create(s32 AppElemID, FCUINT optionID, std::wstring caption);

  const wchar_t* getAppName()                             { return L"Forum Browser"; }

private:

  IrrlichtDevice*         m_pDevice;
};

#endif//_FORUMWINDOW_H_