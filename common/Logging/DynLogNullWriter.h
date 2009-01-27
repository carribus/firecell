#ifndef _DYNLOGNULLWRITER_H_
#define _DYNLOGNULLWRITER_H_

#include "IDynLog.h"

namespace Logging
{

class DynLogNullWriter : public IDynLogWriter
{
public:
  DynLogNullWriter(void);
  ~DynLogNullWriter(void);

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