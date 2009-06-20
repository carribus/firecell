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
#include "BankAccount.h"

BankAccount::BankAccount()
: m_characterID(0)
, m_balance(0)
, m_debt(0)
, m_interest_rate(0)
, m_isSecure(false)
{
}

///////////////////////////////////////////////////////////////////////

BankAccount::~BankAccount(void)
{
}

///////////////////////////////////////////////////////////////////////

bool BankAccount::verifyPassword(const char* password)
{
  if ( !password )
    return false;

  return (m_password.compare(password) == 0);
}