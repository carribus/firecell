#ifndef _DYNLOGCONSOLEWRITER_H_
#define _DYNLOGCONSOLEWRITER_H_

#include "IDynLog.h"

namespace Logging
{

class DynLogConsoleWriter : public IDynLogWriter
{
public:
  DynLogConsoleWriter(void);
  ~DynLogConsoleWriter(void);

  /*
   *  IDynLogWriter implementation
   */
  bool startWriter();
  void stopWriter();
  E_LOGWRITER_TYPE getType();
  std::string getName();
  IDynLogItem* createLogItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp);
  bool writeItem(IDynLogItem* item);
};

} // end of namespace Logging

#endif//_DYNLOGCONSOLEWRITER_H_