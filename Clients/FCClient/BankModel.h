#ifndef _BANKMODEL_H_
#define _BANKMODEL_H_

#include "../../common/fctypes.h"

/**
 *	@class BankModel
 *	@brief Data model for the Banking portion of FireCell
 */
class BankModel
{
  BankModel(void);
  ~BankModel(void);

public:

	/**
	 *	@brief Get access to the singleton instance of the BankModel
	 */
  static BankModel* instance();

	/**
	 *	@brief Destroy the singleton instance of the BankModel
	 */
  static void destroy();

  bool isLoggedIn()                                             { return m_bLoggedIn; }
  void setLoggedIn(bool bFlag)                                  { m_bLoggedIn = bFlag; }

private:

  static BankModel*	m_pThis;
  bool m_bLoggedIn;

};

#endif//_BANKMODEL_H_