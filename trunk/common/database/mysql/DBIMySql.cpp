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
#ifndef HAVE_STRTOK_R
  #define HAVE_STRTOK_R
#endif//HAVE_STRTOK_R
#ifdef _WIN32
#include "my_global.h"
#endif//_WIN32
#include "mysql.h"
#include "DBIMySqlConnection.h"
#include "DBIMySql.h"

DBIMySql::DBIMySql(void)
{
  pthread_mutex_init(&m_mutexConns, NULL);
  // initialise the mysql library
  mysql_library_init(0, NULL, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIMySql::~DBIMySql(void)
{
  // delete the connections
  pthread_mutex_lock(&m_mutexConns);
  vector<DBIMySqlConnection*>::iterator it;

  for ( it = m_connections.begin(); it != m_connections.end(); it++ )
  {
    delete *it;
  }
  m_connections.clear();
  pthread_mutex_unlock(&m_mutexConns);

  pthread_mutex_destroy(&m_mutexConns);

  // close the mysql library
  mysql_library_end();
}

//////////////////////////////////////////////////////////////////////////////////////////

IDBConnection* DBIMySql::Connect(std::string server, short port, std::string dbname, std::string user, std::string pass)
{
  DBIMySqlConnection* pConn = new DBIMySqlConnection;

  if ( !pConn->Connect(server,
                       port,
                       dbname,
                       user,
                       pass) )
  {
    // failed to connect to the db
    delete pConn;
    pConn = NULL;
  }
  else
  {
    pthread_mutex_lock(&m_mutexConns);
    m_connections.push_back(pConn);
    pthread_mutex_unlock(&m_mutexConns);
  }

  return pConn;
}

//////////////////////////////////////////////////////////////////////////////////////////

void DBIMySql::Disconnect(IDBConnection* pConn)
{
	if ( !pConn )
		return;

  pthread_mutex_lock(&m_mutexConns);

	vector<DBIMySqlConnection*>::iterator it = m_connections.begin();
	vector<DBIMySqlConnection*>::iterator limit = m_connections.end();

	for ( ; it != limit; it++ )
	{
		if ( *it == pConn )
		{
			m_connections.erase(it);
			delete (DBIMySqlConnection*) pConn;
			break;
		}
	}

  pthread_mutex_unlock(&m_mutexConns);
}

//////////////////////////////////////////////////////////////////////////////////////////

void DBIMySql::Release()
{
  delete this;
}