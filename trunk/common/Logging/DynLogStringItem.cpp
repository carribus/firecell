#include <sstream>
#include "DynLogStringItem.h"

namespace Logging
{

DynLogStringItem::DynLogStringItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp, const std::string& format)
: DynLogItemBase(data, sourceFile, sourceLineNum, timestamp, format)
{
}

///////////////////////////////////////////////////////////////////////////////////////////

DynLogStringItem::~DynLogStringItem()
{
}

///////////////////////////////////////////////////////////////////////////////////////////

std::string DynLogStringItem::serialise()
{
  std::stringstream ss;
  std::string result, ts = asctime(&m_timestamp);

  if ( ts[ts.size()-1] == '\n' )
    ts.erase( ts.size()-1 );

  result = m_format;
  replacePlaceHolder(result, DYNLOG_FORMAT_TIME, ts);
  replacePlaceHolder(result, DYNLOG_FORMAT_DATA, m_data);
  replacePlaceHolder(result, DYNLOG_FORMAT_FILE, m_sourceFile);
  replacePlaceHolder(result, DYNLOG_FORMAT_LINE, m_sourceLineNum);
  result += "\n";

  return result;
}

///////////////////////////////////////////////////////////////////////////////////////////

void DynLogStringItem::serialise(char*& buffer, size_t& size)
{
  std::string result = serialise();
  size = result.size();

  buffer = new char[size+1];
  memset(buffer, 0, size+1);
  memcpy(buffer, result.c_str(), size);
}

///////////////////////////////////////////////////////////////////////////////////////////

void DynLogStringItem::replacePlaceHolder(std::string& buffer, const std::string placeholder, const std::string value)
{
  if ( !placeholder.compare(value) )    // ensure that the replacement value is not the same as the placeholder
    return;

  size_t pos = buffer.find(placeholder);

  while ( pos != std::string::npos )
  {
    buffer.replace(pos, placeholder.size(), value.c_str(), value.size() );
    pos = buffer.find(placeholder);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////

void DynLogStringItem::replacePlaceHolder(std::string& buffer, const std::string placeholder, const size_t value)
{
  std::stringstream ss;

  ss << (unsigned int)value;
  replacePlaceHolder(buffer, placeholder, ss.str());
}

}