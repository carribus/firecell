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
#ifndef _DBIMYSQL_H_
#define _DBIMYSQL_H_

#include <vector>
#include "DBIMySqlConnection.h"
#include "../IDBInterface.h"

using namespace std;

class DBIMySql : public IDBInterface
{
public:
  DBIMySql(void);
  virtual ~DBIMySql(void);

  /*
   *  IDBInterface implementation
   */
  IDBConnection* Connect(std::string server, short port, std::string dbname, std::string user, std::string pass);
  void Release();

private:

  vector<DBIMySqlConnection*>     m_connections;
};

#endif//_DBIMYSQL_H_