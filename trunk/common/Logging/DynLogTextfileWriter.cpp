#include "DynLog.h"
#include "DynLogTextfileWriter.h"

namespace Logging
{

//////////////////////////////////////////////////////////////////////////////////////////
// class DynLogTextfileWriter
//////////////////////////////////////////////////////////////////////////////////////////
DynLogTextfileWriter::DynLogTextfileWriter(void)
: m_format(DYNLOG_DEFAULTFORMAT)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

DynLogTextfileWriter::~DynLogTextfileWriter(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void DynLogTextfileWriter::setOutputFormat(const std::string& format)
{
  if ( format.empty() )
    m_format = DYNLOG_DEFAULTFORMAT;
  else
    m_format = format;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool DynLogTextfileWriter::startWriter()
{
  if ( m_filename.empty() )
    return false;

  bool bResult = false;

  m_file.open( m_filename.c_str(), std::ios_base::app | std::ios_base::out );
  if ( (bResult = m_file.is_open()) )
  {
  }

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////////////////

void DynLogTextfileWriter::stopWriter()
{
  if ( m_file.is_open() )
  {
    m_file.flush();
    m_file.close();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

E_LOGWRITER_TYPE DynLogTextfileWriter::getType()
{
  return LOGWRITER_TEXTFILE;
}

//////////////////////////////////////////////////////////////////////////////////////////

std::string DynLogTextfileWriter::getName()
{
  return "DynLogTextfileWriter";
}

//////////////////////////////////////////////////////////////////////////////////////////

IDynLogItem* DynLogTextfileWriter::createLogItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp)
{
  return (new DynLogStringItem(data, sourceFile, sourceLineNum, timestamp, m_format));
}

//////////////////////////////////////////////////////////////////////////////////////////

bool DynLogTextfileWriter::writeItem(IDynLogItem* item)
{
  if ( !item )
    return false;

  std::string data = item->serialise();

  m_file << data;
  m_file.flush();

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

void DynLogTextfileWriter::setFilename(const std::string& filename)
{
  m_filename = filename;
}

//////////////////////////////////////////////////////////////////////////////////////////

std::string DynLogTextfileWriter::getFilename()
{
  return m_filename;
}

}