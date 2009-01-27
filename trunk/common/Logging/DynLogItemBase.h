#ifndef _DYNLOGITEMBASE_H_
#define _DYNLOGITEMBASE_H_

#include <ctime>
#include "IDynLog.h"

namespace Logging
{

class DynLogItemBase : public IDynLogItem
{
public:
  DynLogItemBase(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp);
  ~DynLogItemBase(void);

  std::string getData();
  const char* getSourceFile();
  size_t getSourceLineNum();

protected:

  std::string         m_data;
  const char*         m_sourceFile;
  size_t              m_sourceLineNum;
  tm                  m_timestamp;
};

};// end of namespace Logging

#endif//_DYNLOGITEMBASE_H_