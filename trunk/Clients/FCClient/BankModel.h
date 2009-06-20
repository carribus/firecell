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