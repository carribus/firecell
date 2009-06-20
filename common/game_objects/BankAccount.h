#ifndef _BANKACCOUNT_H_
#define _BANKACCOUNT_H_

#include <string>
#include "../fctypes.h"

class BankAccount
{
public:
  BankAccount();
  ~BankAccount(void);

  bool verifyPassword(const char* password);

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
  void                setPassword(const std::string& pw)      { m_password = pw; }

private:

  FCULONG             m_characterID,
                      m_balance,
                      m_debt;
  FCSHORT             m_interest_rate;
  bool                m_isSecure;
  std::string         m_password;
};

#endif//_BANKACCOUNT_H_