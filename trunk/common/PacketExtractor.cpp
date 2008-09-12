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
#include "PacketExtractor.h"

//////////////////////////////////////////////////////////////////////////////////////////

PacketExtractor::PacketExtractor(void)
: m_minPktSize(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

PacketExtractor::~PacketExtractor(void)
{
}

/*////////////////////////////////////////////////////////////////////////////////////////

    Purpose of this function is to prepare an EP (Extraction Plan) for the submitted
    packet format.

    ------------------------------------------------------------------
    Definition Format:
    ------------------------------------------------------------------
    start of definition = [[
      start of field      = :
        field name          = alphanumeric sequence up to next ':'
      element width start = :
        element width (size)= value
      number of elements  = [0..inf] OR
                            (reference to previous member definition)
      end of field        = |
    end of definition   = ]]

////////////////////////////////////////////////////////////////////////////////////////*/
bool PacketExtractor::Prepare(string strPacketFormat)
{
  if ( strPacketFormat.empty() )
    return false;

  size_t len = strPacketFormat.length();
  size_t offset = 0, pos;
  if ( (offset = strPacketFormat.find_first_not_of("[")) == 1 )
  {
    while ( offset < len )
    {
      switch ( strPacketFormat[offset] )
      {
      case  PKTDEF_FIELD_START:
        {
          EPField field;

          pos = strPacketFormat.find_first_of( PKTDEF_FIELD_END, offset );
          string strFieldDef = strPacketFormat.substr( offset, pos-offset+1 );

          ParseFieldDef(strFieldDef, field);
          offset = pos+1;

          AddToExtractionPlan(field);
        }
        break;

      case  PKTDEF_END:
        offset++;
        break;

      default:
        return false;
      }
    }
  }
  else
    return false;

  CalculateMinPacketSize();

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

PEPacket* PacketExtractor::Extract(const char* pSrc, size_t& streamOffset, const size_t streamLen)
{
  if ( !pSrc || streamOffset > streamLen || streamLen <= 0 )
    return NULL;
  if ( m_ep.empty() )
    return NULL;

  FieldVector::iterator it;
  PEPacket* pNewPkt = new PEPacket;
  EPField field;
  size_t width, count, refSize, refOffset, origOffset = streamOffset;

  for ( it = m_ep.begin(); it != m_ep.end() && streamOffset < streamLen; it++ )
  {
    // check if we have a fixed element width...
    if ( !it->elem_width )
    {
      // if not, its a width based on a previous field...
      if ( pNewPkt->FieldExists( it->varRefWidth ) )
      {
        refSize = pNewPkt->GetFieldSize( it->varRefWidth );
        refOffset = pNewPkt->GetFieldOffset( it->varRefWidth );
        memcpy( (void*)&width, pSrc + refOffset, refSize );
      }
    }
    else 
      width = it->elem_width;

    // check if we have a fixed element count...
    if ( !it->elem_count )
    {
      // if not, its a count based on a previous field...
      if ( pNewPkt->FieldExists( it->varRefCount ) )
      {
        refSize = pNewPkt->GetFieldSize( it->varRefCount );
        refOffset = pNewPkt->GetFieldOffset( it->varRefCount );
        memcpy( (void*)&count, pSrc + refOffset, refSize );
      }
    }
    else
      count = it->elem_count;

    // check if we're still within our stream data range
    if ( streamOffset + (count*width) <= streamLen )
    {
      // yip, add the field
      pNewPkt->SetField( it->name, streamOffset, width, count );
      streamOffset += count*width;
    }
    else
    {
      // nope, but lets move the offset accordingly anyway
      streamOffset += count*width;
      break;
    }
  }

  if ( it != m_ep.end() )
  {
    delete pNewPkt;
    pNewPkt = NULL;
    // reset the offset
    streamOffset = origOffset;
  }
  else
  {
    pNewPkt->SetDataBlock( pSrc, streamOffset );
  }

  return pNewPkt;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool PacketExtractor::ParseFieldDef(string strDef, PacketExtractor::EPField& field)
{
  if ( strDef.empty() )
    return false;

  size_t offset = 0, temp_ofs = 0;

  if ( strDef[offset] != PKTDEF_FIELD_START )
    return false;

  ClearField(field);
  offset++;

  if ( (temp_ofs = strDef.find_first_of( PKTDEF_ELEMWIDTH_START, offset )) != -1 )
  {
    // get the name
    field.name = strDef.substr( offset, temp_ofs - offset );
    offset = temp_ofs+1;

    // get the elem width
    if ( (temp_ofs = strDef.find_first_of( PKTDEF_ELEMCOUNT_START, offset )) != -1 )
    {
      // check for a variable derefence
      if ( strDef[offset] != PKTDEF_VARNAME_START )
      {
        // static element size expected
        field.elem_width = (size_t)atoi( strDef.substr( offset, temp_ofs - offset ).c_str() );
      }
      else
      {
        // variable dereference
        offset++;
        field.varRefWidth = strDef.substr(offset, temp_ofs - offset);
      }
      offset = temp_ofs+1;

      // get the number of [width] size elements to read
      if ( (temp_ofs = strDef.find_first_of( PKTDEF_FIELD_END, offset )) != -1 )
      {
        // check for a variable derefence
        if ( strDef[offset] != PKTDEF_VARNAME_START )
        {
          // static element size expected
          field.elem_count = (size_t)atoi( strDef.substr( offset, temp_ofs - offset ).c_str() );
        }
        else
        {
          // variable dereference
          offset++;
          field.varRefCount = strDef.substr(offset, temp_ofs - offset);
        }
      }
    }
    else
      return false;
  }
  else
    return false;

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PacketExtractor::ClearField(PacketExtractor::EPField& field)
{
  field.elem_count = field.elem_width = 0;
  field.name = field.varRefCount = field.varRefWidth = "";
}

//////////////////////////////////////////////////////////////////////////////////////////

bool PacketExtractor::ReadElement(const char* pSrc, size_t& streamOffset, size_t streamLen, void* pDest, size_t elemsize)
{
  if ( !pDest || !elemsize || (streamOffset > streamLen) )
    return false;

  memcpy(pDest, pSrc + streamOffset, elemsize);
  streamOffset += elemsize;

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

void PacketExtractor::AddToExtractionPlan(PacketExtractor::EPField& field)
{
  m_ep.push_back(field);
}

//////////////////////////////////////////////////////////////////////////////////////////

void PacketExtractor::CalculateMinPacketSize()
{
  m_minPktSize = 0;
  for ( FieldVector::iterator it = m_ep.begin(); it != m_ep.end(); it++ )
  {
    m_minPktSize += it->elem_width * it->elem_count;
  }
}