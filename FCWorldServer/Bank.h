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