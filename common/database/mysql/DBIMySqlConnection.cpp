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
#include "DBIMySqlResultSet.h"
#include "DBIMySqlResults.h"
#include "DBIMySqlConnection.h"

DBIMySqlConnection::DBIMySqlConnection(void)
: m_conn(NULL)
{
  m_conn = mysql_init(NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIMySqlConnection::~DBIMySqlConnection(void)
{
  Disconnect();
}

//////////////////////////////////////////////////////////////////////////////////////////

bool DBIMySqlConnection::Connect(const std::string& server, short port, const std::string& dbname, const std::string& user, const std::string& pass)
{
  if ( !m_conn )
    return false;

  if ( ! mysql_real_connect(m_conn,
                            server.c_str(),
                            user.c_str(),
                            pass.c_str(),
                            dbname.c_str(),
                            port, 
                            NULL, 
                            0) )
  {
    // failed to connect to the DB
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool DBIMySqlConnection::Execute(const std::string& query)
{
  if ( !m_conn )
    return false;

  bool bResult = true;
  int nResult = mysql_real_query(m_conn, query.c_str(), query.length());
  DBIMySqlResults* pResults = new DBIMySqlResults;
  DBIMySqlResultSet* pResultSet = NULL;

  if ( nResult == 0 )     // success
  {
    do
    {
      MYSQL_RES* result = mysql_store_result(m_conn);

      if ( result )
      {
        pResultSet = new DBIMySqlResultSet;
        // process the results from the query
        pResultSet->Process(result);
        // add the resultset to the results object
        pResults->AddResultSet(pResultSet);
        // free the internal result object
        mysql_free_result(result);
      }
      else
      {
        if ( mysql_field_count(m_conn) == 0 )
        {
          // query does not return data...
          // TODO: Fetch the number of affected rows if needs be
        }
        else
        {
          bResult = false;
        }
      }
    } while ( mysql_next_result(m_conn) == 0 );
  }
  else
  {
    // error handling ..
    bResult = false;
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

void DBIMySqlConnection::Disconnect()
{
  if ( m_conn )
  {
    mysql_close(m_conn);
    m_conn = NULL;
  }
}