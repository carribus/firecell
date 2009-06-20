#ifndef _BANK_H_
#define _BANK_H_

#include <map>
#include "../common/fctypes.h"
#include "../common/PThreadRWLock.h"
#include "../common/game_objects/BankAccount.h"

using namespace std;

class Bank
{
public:
  Bank(void);
  ~Bank(void);

  BankAccount* addBankAccount(Player* pPlayer, FCULONG balance, FCULONG debt, FCSHORT interest_rate, bool isSecure, const string& password);
  BankAccount* getBankAccount(FCULONG characterID);
#error you left off here

private:

  /*
   *  Private Data members
   */
  typedef map<FCULONG, BankAccount*> BankAccountsMap;
  BankAccountsMap m_bankAccounts;
  PThreadRWLock m_lock;
};

#endif//_BANK_H_