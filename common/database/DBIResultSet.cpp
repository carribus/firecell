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
#include <stdlib.h>
#include "DBIResults.h"
#include "DBIResultSet.h"

string DBIResultSet::DBIColumn::GetValue(size_t index)
{
  string res;

  if ( index < m_values.size() )
    res = m_values.at(index);

  return res;
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIResultSet::DBIResultSet(DBIResults* pParent)
: m_pParent(pParent)
, m_rowCount(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIResultSet::~DBIResultSet(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t DBIResultSet::AddColumn(const string& name, unsigned short type)
{
  DBIColumn col;

  col.SetName(name);
  col.SetType(type);
  m_columns[name] = col;

  return m_columns.size();
}

//////////////////////////////////////////////////////////////////////////////////////////

void DBIResultSet::AddColumnDataForRow(const string& name, const string& value)
{
  map<string, DBIColumn >::iterator it = m_columns.find(name);

  if ( it != m_columns.end() )
  {
    it->second.AddValue(value);
  }
  else
  {
    // column couldn't be found... so there's some kind of logic problem...
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

string DBIResultSet::GetStringValue(const string& name, size_t row)
{
  string val = "";
  map<string, DBIColumn>::iterator it = m_columns.find(name);

  val = "";
  if ( it != m_columns.end() )
  {
    val = it->second.GetValue(row);
  }

  return val;
}

//////////////////////////////////////////////////////////////////////////////////////////

unsigned long DBIResultSet::GetULongValue(const string& name, size_t row)
{
  return atoi(GetStringValue(name, row).c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////

unsigned short DBIResultSet::GetShortValue(const string& name, size_t row)
{
  return (unsigned short)atoi(GetStringValue(name, row).c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////

unsigned char DBIResultSet::GetByteValue(const string& name, size_t row)
{
  return (unsigned char)atoi(GetStringValue(name, row).c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////

bool DBIResultSet::IsLastResultSet()
{
  if ( !m_pParent )
    return true;

  return (m_pParent->GetCount() == 0);
}
