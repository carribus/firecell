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