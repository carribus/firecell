#ifndef _DYNLOGGER_H_
#define _DYNLOGGER_H_

#include <ctime>
#include <deque>
#include <vector>
#include "IDynLog.h"
#include "RefCounter.h"

namespace Logging
{

/**
 *  \class DynLogger
 *  \brief This is the concrete implementation of the singleton IDynLogger object.
 *
 *  This class represents the object that sits behind the singleton IDynLogger interface that is returned from 
 *  getLogger() and provides the front line interface to the logging framework.
 */
class DynLogger : public IDynLogger
{
  DynLogger(void);
  ~DynLogger(void);

public:

  static DynLogger* instance();
  static void destroy();

  /*
   *  IDynLogger implementation
   */
  size_t addWriter(IDynLogWriter* writer);
  IDynLogWriter* getWriter(size_t index);
  size_t getWriterCount();
  void flush();
  size_t getBackLog();
  size_t addLog(const std::string& data, const char* sourceFile, size_t sourceLineNum, size_t logLevel = 1);
  std::string formatLog(const char* data, ...);
  void setLogLevelFilter(size_t maxLevelToLog);

private:

  /*
   *  Private Methods
   */
  void FreeWriters();

  /*
   *  Private Data Members
   */

  /**
   *  The singleton instance of the DynLogger object
   */
  static DynLogger* m_pThis;

  typedef std::vector<IDynLogWriter*> WritersVector;
  WritersVector           m_writers;

  /**
   *  m_items is the deque object that stores the backlog of items to be written out by the Log Writer
   */
  struct stLogItem
  {
    std::string data;           //!< the log item's data block
    const char* sourceFile;     //!< the source file that generated the log item
    size_t sourceLineNum;       //!< the line number in the source file that generated the log item
    tm timestamp;               //!< the timestamp at which the logitem was generated
  };
  typedef std::deque<stLogItem> LogItemQueue;
  LogItemQueue            m_items;

  /** 
   *  m_logLevelFilter indicates which entries should be logged. Always log on levels equal or less than m_logLevelFilter
   */ 
  size_t m_logLevelFilter;

};

}; // end of namespace Logging

#endif//_DYNLOGGER_H_