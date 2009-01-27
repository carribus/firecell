#include "DynLogger.h"

namespace Logging
{

DynLogger::DynLogger(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

DynLogger::~DynLogger(void)
{
  FreeWriters();
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t DynLogger::addWriter(IDynLogWriter* writer)
{
  if ( !writer )
    return 0;

  writer->addRef();
  writer->startWriter();
  m_writers.push_back(writer);

  return m_writers.size();
}

//////////////////////////////////////////////////////////////////////////////////////////

IDynLogWriter* DynLogger::getWriter(size_t index)
{
  if ( index > m_writers.size()-1 )
    return NULL;

  return m_writers[index];
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t DynLogger::getWriterCount()
{
  return m_writers.size();
}

//////////////////////////////////////////////////////////////////////////////////////////

void DynLogger::flush()
{
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t DynLogger::getBackLog()
{
  return m_items.size();
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t DynLogger::addLog(const std::string& data, const char* sourceFile, size_t sourceLineNum)
{
  WritersVector::iterator it = m_writers.begin();
  WritersVector::iterator limit = m_writers.end();
  IDynLogItem* pItem = NULL;
  time_t t = time(NULL);
  tm timestamp = *gmtime(&t);

  for ( ; it != limit; it++ )
  {
    pItem = (*it)->createLogItem( data, sourceFile, sourceLineNum, timestamp );
    if ( pItem )
    {
      (*it)->writeItem(pItem);
    }
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

void DynLogger::FreeWriters()
{
  WritersVector::iterator it = m_writers.begin();
  WritersVector::iterator limit = m_writers.end();

  for ( ; it != limit; it++ )
  {
    (*it)->stopWriter();
    if ( !(*it)->release() )
    {
      // something went wrong here and we can't release our writer
    }
  }

  m_writers.erase( m_writers.begin(), limit );
}

};// end of namespace Logging