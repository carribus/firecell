#ifndef _BANKINGWINDOW_H_
#define _BANKINGWINDOW_H_

#include <string>
#include "FCModel.h"
#include "InGameAppWindow.h"
#include "GUIBankAuthView.h"
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

  /**
   *  @brief Event handler called when a successful connection has been established to the bank account
   */
  bool OnBankConnected();

  /**
   *  @brief Event handler called when authentication is needed for a bank account
   */
  bool OnBankAuthNeeded();

  /**
   *  @brief Event handler called when the player does not have a bank account
   */
  bool OnNoAccountExists();

  /**
   *  @brief Event handler called when a player's bank account details have been updated
   */
  bool OnAccountDetailsUpdated(BankAccount* pAccount);

  void SendBankAuthentication(std::wstring& password);

private:

  /*
   *  Private Members
   */
  IrrlichtDevice*         m_pDevice;
  GUIBankAuthView*        m_pAuthView;
  GUIBankView*            m_pBankView;
};

#endif//_BANKINGWINDOW_H_