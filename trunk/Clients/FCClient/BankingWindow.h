#ifndef _BANKINGWINDOW_H_
#define _BANKINGWINDOW_H_

#include <string>
#include "FCModel.h"
#include "InGameAppWindow.h"
#include "GUIBankView.h"

class BankingWindow : public InGameAppWindow
{
public:
  BankingWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice);
  ~BankingWindow(void);

  bool Create(s32 AppElemID, FCUINT optionID, std::wstring caption);

  /**
   *  @brief Connects to the banking server and determines authentication requirements for the player
   */
  void ConnectToBank();

private:

  /*
   *  Private Members
   */
  IrrlichtDevice*         m_pDevice;
  GUIBankView*            m_pBankView;

  BankModel*              m_pModel;
};

#endif//_BANKINGWINDOW_H_