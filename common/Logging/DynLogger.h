/**
 *  \ingroup DynLog
 */
#ifndef _DYNLOGGER_H_
#define _DYNLOGGER_H_

#include <ctime>
#include <deque>
#include <vector>
#include "IDynLog.h"
#include "RefCounter.h"

namespace Logging
{

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
  size_t addLog(const std::string& data, const char* sourceFile, size_t sourceLineNum);
  std::string formatLog(const char* data, ...);

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
    std::string data;
    const char* sourceFile;
    size_t sourceLineNum;
    tm timestamp;
  };
  typedef std::deque<stLogItem> LogItemQueue;
  LogItemQueue            m_items;

};

}; // end of namespace Logging

#endif//_DYNLOGGER_H_