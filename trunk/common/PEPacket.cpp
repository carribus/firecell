#include "PEPacket.h"

PEPacket::PEPacket(void)
: m_pDataBlock(NULL)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

PEPacket::~PEPacket(void)
{
  if ( m_pDataBlock )
    free(m_pDataBlock);
}

//////////////////////////////////////////////////////////////////////////////////////////

void PEPacket::SetField(const string& name, size_t offset, size_t elem_width, size_t elem_count)
{
  Field f;

  f.name = name;
  f.offset = offset;
  f.elem_width = elem_width;
  f.elem_count = elem_count;

  m_fields[name] = f;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool PEPacket::FieldExists(const string& name)
{
  FieldMap::iterator it = m_fields.find(name);

  return ( it != m_fields.end() );
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t PEPacket::GetFieldSize(const string& name)
{
  FieldMap::iterator it = m_fields.find(name);

  if ( it != m_fields.end() )
    return it->second.elem_count * it->second.elem_width;

  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t PEPacket::GetFieldOffset(const string& name)
{
  FieldMap::iterator it = m_fields.find(name);

  if ( it != m_fields.end() )
    return it->second.offset;

  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool PEPacket::GetField(const string& key, void* pDest, size_t size)
{
  if ( !pDest || !size )
    return false;

  bool bResult = false;
  FieldMap::iterator it = m_fields.find(key);
  size_t field_size = 0;

  if ( it != m_fields.end() )
  {
    field_size = it->second.elem_count * it->second.elem_width;
    if ( field_size <= size )
    {
      memcpy(pDest, m_pDataBlock + it->second.offset, field_size);
      bResult = true;
    }
  }

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PEPacket::SetDataBlock(const char* pData, size_t blockLen)
{
  if ( !pData || !blockLen )
    return;

  m_pDataBlock = (char*)malloc( blockLen );
  memcpy(m_pDataBlock, pData, blockLen);
}

