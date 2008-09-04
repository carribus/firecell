#include ".\packetextractor.h"

//////////////////////////////////////////////////////////////////////////////////////////

PacketExtractor::PacketExtractor(void)
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

    ------------------------------------------------------------------
    Constants:
    ------------------------------------------------------------------
    #define PKTDEF_START              "[["
    #define PKTDEF_FIELD_START        ":"
    #define PKTDEF_ELEMWIDTH_START    PKTDEF_FIELD_START
    #define PKTDEF_VARNAME_START      "*"
    #define PKTDEF_ELEMCOUNT_START    PKTDEF_FIELD_START
    #define PKTDEF_FIELD_END          "|"
    #define PKTDEF_END                "]]"

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
          pos = strPacketFormat.find_first_of( PKTDEF_FIELD_END, offset );
          string strFieldDef = strPacketFormat.substr( offset, pos-offset+1 );
          EPField field;
          ParseFieldDef(strFieldDef, field);
          offset = pos+1;
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

  return true;
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

