/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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

BankAccount* Bank::createBankAccount(Player* pPlayer, FCULONG balance, FCULONG debt, FCSHORT interest_rate, bool isSecure, const string& password)
{
  if ( !pPlayer )
    return NULL;

  BankAccount* pAccount = NULL;
  BankAccountsMap::iterator it;

  m_lock.LockForWrite();

  it = m_bankAccounts.find(pPlayer->GetID());
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

      PersistNewBankAccount(pPlayer, pAccount);
    }
  }
  else
  {
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Bank::createBankAccount(): Account creationg failed for character %ld because account already exists in memory.", pPlayer->GetID()) );
  }

  m_lock.Unlock();

  return pAccount;
}

///////////////////////////////////////////////////////////////////////

BankAccount* Bank::addBankAccount(Player* pPlayer, FCULONG balance, FCULONG debt, FCSHORT interest_rate, bool isSecure, const string& password)
{
  if ( !pPlayer )
    return NULL;

  BankAccount* pAccount = NULL;
  BankAccountsMap::iterator it;

  m_lock.LockForWrite();

  it = m_bankAccounts.find(pPlayer->GetID());
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

///////////////////////////////////////////////////////////////////////

bool Bank::removeBankAccount(Player* pPlayer)
{
  BankAccountsMap::iterator it;
  bool bResult = false;

  m_lock.LockForWrite();
  
  it = m_bankAccounts.find(pPlayer->GetID());
  if ( it != m_bankAccounts.end() )
  {
    PersistBankAccount(pPlayer, it->second);
    delete it->second;
    m_bankAccounts.erase(it);
  }

  m_lock.Unlock();

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void Bank::PersistNewBankAccount(Player* pPlayer, BankAccount* pAccount)
{
  FCDatabase& db = FCDatabase::instance();

  db.ExecuteJob(DBQ_INSERT_CHARACTER_BANK_ACCOUNT, NULL, 
                pPlayer->GetID(),
                pAccount->getBalance(),
                pAccount->getDebt(),
                pAccount->getInterestRate(),
                pAccount->isSecure(),
                pAccount->getPassword().c_str());
}

///////////////////////////////////////////////////////////////////////

void Bank::PersistBankAccount(Player* pPlayer, BankAccount* pAccount)
{
  FCDatabase& db = FCDatabase::instance();

  db.ExecuteJob(DBQ_UPDATE_CHARACTER_BANK_ACCOUNT, NULL, 
                pAccount->getBalance(),
                pAccount->getDebt(),
                pAccount->getInterestRate(),
                pAccount->isSecure(),
                pAccount->getPassword().c_str(),
                pPlayer->GetID());
}