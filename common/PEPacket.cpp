#include "PEPacket.h"

PEPacket::PEPacket(void)
: m_pDataBlock(NULL)
, m_dataLen(0)
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

void PEPacket::SetFieldValue(const string& name, void* pValue)
{
  if ( !m_pDataBlock || !pValue )
    return;

  if ( !FieldExists(name) )
    return;

  Field f = m_fields[name];

  memcpy( m_pDataBlock + f.offset, pValue, f.elem_count * f.elem_width );
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

bool PEPacket::AddField(const string& key, size_t elem_width, size_t elem_count, void* pDefaultValue)
{
  if ( FieldExists(key) )
    return false;

  Field f = { key, elem_width, elem_count, m_dataLen };
  size_t prevSize = m_dataLen, fieldSize = elem_width * elem_count;

  m_dataLen += elem_width * elem_count;
  if ( fieldSize )
  {
    if ( m_pDataBlock )
    {
      if ( !(m_pDataBlock = (char*)realloc( m_pDataBlock, m_dataLen )) )
        return false;
    }
    else
    {
      if ( !(m_pDataBlock = (char*)malloc(m_dataLen)) )
        return false;
    }
  }

  m_fields[key] = f;

  if ( pDefaultValue )
  {
    memcpy( m_pDataBlock + prevSize, pDefaultValue, fieldSize );
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PEPacket::SetDataBlock(const char* pData, size_t blockLen)
{
  if ( !pData || !blockLen )
    return;

  m_pDataBlock = (char*)malloc( blockLen );
  memcpy(m_pDataBlock, pData, blockLen);
  m_dataLen = blockLen;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PEPacket::GetDataBlock(char*& pData, size_t& blockLen)
{
  pData = m_pDataBlock;
  blockLen = m_dataLen;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PEPacket::DebugDump()
{
  FieldMap::iterator it;

  for ( it = m_fields.begin(); it != m_fields.end(); it++ )
  {
    size_t offset = it->second.offset;
    printf("%s (%ldx%ld): ", it->second.name.c_str(), it->second.elem_width, it->second.elem_count);
    for ( size_t i = 0; i < it->second.elem_count * it->second.elem_width; i++ )
    {
      printf("%0x", (char)*(m_pDataBlock+offset+i));
    }
    printf("\n");
  }
}