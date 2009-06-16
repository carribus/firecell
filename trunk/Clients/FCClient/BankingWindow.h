#ifndef _BANKINGWINDOW_H_
#define _BANKINGWINDOW_H_

#include <string>
#include "FCModel.h"
#include "InGameAppWindow.h"

class BankingWindow : public InGameAppWindow
{
public:
  BankingWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice);
  ~BankingWindow(void);

  bool Create(s32 AppElemID, FCUINT optionID, std::wstring caption);

  const wchar_t* getAppName()                             { return L"Banking"; }

private:

  /*
   *  Private Members
   */
  IrrlichtDevice*         m_pDevice;
};

#endif//_BANKINGWINDOW_H_