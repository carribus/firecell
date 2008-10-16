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
#ifndef _DBIMYSQLCONNECTION_H_
#define _DBIMYSQLCONNECTION_H_

#ifdef _WIN32
#include "my_global.h"
#endif//_WIN32
#include "mysql.h"
#include "../IDBInterface.h"

class DBIMySqlConnection : public IDBConnection
{
public:
  DBIMySqlConnection(void);
  ~DBIMySqlConnection(void);

  bool Connect(const std::string& server, short port, const std::string& dbname, const std::string& user, const std::string& pass);

  /*
   *  IDBConnection implementation
   */
  bool Execute(const std::string& query);
  void Disconnect();

private:

  MYSQL*       m_conn;
};

#endif//_DBIMYSQLCONNECTION_H_