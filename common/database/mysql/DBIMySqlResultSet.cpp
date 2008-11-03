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

DBIMySqlResultSet::DBIMySqlResultSet(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIMySqlResultSet::~DBIMySqlResultSet(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void DBIMySqlResultSet::Process(MYSQL_RES* result)
{
  MYSQL_ROW row;
  unsigned int numFields = mysql_num_fields(result), nField;
  MYSQL_FIELD* fields = mysql_fetch_fields(result);

  // add the fields
  for ( nField = 0; nField < numFields; nField++ )
  {
    AddColumn( fields[nField].name, (unsigned short)fields[nField].type );
  }

  while ( (row = mysql_fetch_row(result)) )
  {
    for ( nField = 0; nField < numFields; nField++ )
    {
      char* pValue = row[nField];
      AddColumnDataForRow(fields[nField].name, row[nField]);
    }
  }

  SetRowCount( (size_t)mysql_num_rows(result) );
}