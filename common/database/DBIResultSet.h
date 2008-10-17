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
#ifndef _DBIRESULTSET_H_
#define _DBIRESULTSET_H_

#include <string>
#include <map>
#include <vector>

using namespace std;

class DBIResultSet
{
protected:

  struct DBIColumn
  {
    DBIColumn()
    {
    }

    void SetName(const string& name)                  { m_name = name; }
    string GetName()                                  { return m_name; }
    void SetType(unsigned short type)                 { m_type = type; }
    unsigned short GetType()                          { return m_type; }

  private:

    string m_name;
    unsigned short m_type;
  };

public:
  DBIResultSet(void);
  virtual ~DBIResultSet(void);

  size_t AddColumn(const string& name, unsigned short type);
  void AddColumnDataForRow(const string& name, const string& value);

private:

  map< string, DBIColumn >       m_columns;
};

#endif//_DBIRESULTSET_H_