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
#ifndef _BANKACCOUNT_H_
#define _BANKACCOUNT_H_

#include <string>
#include "../fctypes.h"

class BankAccount
{
public:
  BankAccount();
  ~BankAccount(void);

#ifndef _FCCLIENT_COMPILE_
  bool verifyPassword(const char* password);
  bool verifyTicket(FCULONG ticket);

  FCULONG             generateTicket();
#endif//_FCCLIENT_COMPILE_

  FCULONG             getCharacterID()                        { return m_characterID; }
  void                setCharacterID(FCULONG charID)          { m_characterID = charID; }
  FCULONG             getBalance()                            { return m_balance; }
  void                setBalance(FCULONG bal)                 { m_balance = bal; }
  FCULONG             getDebt()                               { return m_debt; }
  void                setDebt(FCULONG debt)                   { m_debt = debt; }
  FCSHORT             getInterestRate()                       { return m_interest_rate; }
  void                setInterestRate(FCSHORT rate)           { m_interest_rate = rate; }
  bool                isSecure()                              { return m_isSecure; }
  void                setSecure(bool bSecure)                 { m_isSecure = bSecure; }
  std::string         getPassword()                           { return m_password; }
  void                setPassword(const std::string& pw)      { m_password = pw; }
#ifndef _FCCLIENT_COMPILE_
  bool                isConnected()                           { return m_bConnected; }
  void                setConnected(bool bFlag)                { m_bConnected = bFlag; }
#else
  FCULONG             getTicket()                             { return m_dwTicket; }
  void                setTicket(FCULONG ticket)               { m_dwTicket = ticket; }
#endif//_FCCLIENT_COMPILE_

private:

  FCULONG             m_characterID,
                      m_balance,
                      m_debt;
  FCSHORT             m_interest_rate;
  bool                m_isSecure;
  std::string         m_password;

#ifndef _FCCLIENT_COMPILE_
  bool                m_bConnected;
#endif//_FCCLIENT_COMPILE_
  FCULONG             m_dwTicket;
};

#endif//_BANKACCOUNT_H_