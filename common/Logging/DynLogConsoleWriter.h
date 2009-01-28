#ifndef _DYNLOGCONSOLEWRITER_H_
#define _DYNLOGCONSOLEWRITER_H_

#include "IDynLog.h"

namespace Logging
{

/**
 *  \class DynLogConsoleWriter
 *  \brief This writer is used for log output to a console
 */
class DynLogConsoleWriter : public IDynLogWriter
{
public:
  DynLogConsoleWriter(void);
  ~DynLogConsoleWriter(void);

  /*
   *  IDynLogWriter implementation
   */
  void setOutputFormat(const std::string& format);
  bool startWriter();
  void stopWriter();
  E_LOGWRITER_TYPE getType();
  std::string getName();
  IDynLogItem* createLogItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp);
  bool writeItem(IDynLogItem* item);

private:

  std::string       m_format;
};

} // end of namespace Logging

#endif//_DYNLOGCONSOLEWRITER_H_