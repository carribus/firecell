#include "../common/Logging/DynLog.h"
#include "Player.h"
#include "Bank.h"

Bank::Bank(void)
{
}

///////////////////////////////////////////////////////////////////////

Bank::~Bank(void)
{
}

///////////////////////////////////////////////////////////////////////

BankAccount* Bank::addBankAccount(Player* pPlayer, FCULONG balance, FCULONG debt, FCSHORT interest_rate, bool isSecure, const string& password)
{
  m_lock.LockForWrite();

  BankAccount* pAccount = NULL;
  BankAccountsMap::iterator it = m_bankAccounts.find(pPlayer->GetID());

  // check if this account exists already
  if ( it == m_bankAccounts.end() )
  {
    // it doesn't so we can create it
    if ( (pAccount = new BankAccount) )
    {
      pAccount->setCharacterID(pPlayer->GetID());
      pAccount->setBalance(balance);
      pAccount->setDebt(debt);
      pAccount->setInterestRate(interest_rate);
      pAccount->setSecure(isSecure);
      pAccount->setPassword(password);
      m_bankAccounts[pPlayer->GetID()] = pAccount;
    }
  }
  else
  {
    // the account already exists for some reason...
    // log the situation and update the account to reflect the information passed in
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Bank::addBankAccount(): Account for character %ld already exists in memory.", pPlayer->GetID()) );
    if ( (pAccount = it->second) )
    {
      pAccount->setCharacterID(pPlayer->GetID());
      pAccount->setBalance(balance);
      pAccount->setDebt(debt);
      pAccount->setInterestRate(interest_rate);
      pAccount->setSecure(isSecure);
      pAccount->setPassword(password);
    }
  }

  m_lock.Unlock();

  return pAccount;
}

///////////////////////////////////////////////////////////////////////

BankAccount* Bank::getBankAccount(FCULONG characterID)
{
  BankAccount* pAcc = NULL;
  BankAccountsMap::iterator it;
  
  m_lock.LockForRead();
  
  it = m_bankAccounts.find(characterID);

  if ( it != m_bankAccounts.end() )
    pAcc = it->second;

  m_lock.Unlock();

  return pAcc;
}