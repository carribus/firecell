#include <iostream>
#include "DynLog.h"
#include "DynLogStringItem.h"
#include "DynLogConsoleWriter.h"

namespace Logging
{

DynLogConsoleWriter::DynLogConsoleWriter(void)
: m_format(DYNLOG_DEFAULTFORMAT)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

DynLogConsoleWriter::~DynLogConsoleWriter(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void DynLogConsoleWriter::setOutputFormat(const std::string& format)
{
  if ( format.empty() )
    m_format = DYNLOG_DEFAULTFORMAT;
  else
    m_format = format;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool DynLogConsoleWriter::startWriter()
{
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

void DynLogConsoleWriter::stopWriter()
{
}

//////////////////////////////////////////////////////////////////////////////////////////

E_LOGWRITER_TYPE DynLogConsoleWriter::getType()
{
  return LOGWRITER_CONSOLE;
}

//////////////////////////////////////////////////////////////////////////////////////////

std::string DynLogConsoleWriter::getName()
{
  return "DynLogConsoleWriter";
}

//////////////////////////////////////////////////////////////////////////////////////////

IDynLogItem* DynLogConsoleWriter::createLogItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp)
{
  return new DynLogStringItem(data, sourceFile, sourceLineNum, timestamp, m_format);
}

//////////////////////////////////////////////////////////////////////////////////////////

bool DynLogConsoleWriter::writeItem(IDynLogItem* item)
{
  std::cout << item->serialise();

  return true;
}

}; // end of namespace Logging