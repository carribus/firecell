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
#ifndef _PEPACKET_H_
#define _PEPACKET_H_

#include <map>
#include <string>

using namespace std;

class PEPacket
{
  struct Field
  {
    string name;
    size_t elem_width;
    size_t elem_count;
    size_t offset;
  };

public:
  PEPacket(void);
  virtual ~PEPacket(void);

  void SetField(const string& name, size_t offset, size_t elem_width, size_t elem_count);
  void SetFieldValue(const string& name, void* pValue);
  bool FieldExists(const string& name);
  size_t GetFieldSize(const string& name);
  size_t GetFieldOffset(const string& name);
  bool GetField(const string& key, void* pDest, size_t size);
  bool AddField(const string& key, size_t elem_width, size_t elem_count, void* pDefaultValue = NULL);
  void SetDataBlock(const char* pData, size_t blockLen);
  void GetDataBlock(char*& pData, size_t& blockLen);

  PEPacket* Clone();

  void DebugDump();

private:

  typedef map< string, Field > FieldMap;
  FieldMap                      m_fields;
  char*                         m_pDataBlock;
  size_t                        m_dataLen;
};

#endif//_PEPACKET_H_
