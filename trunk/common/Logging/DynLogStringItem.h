#ifndef _DYNLOGSTRINGITEM_H_
#define _DYNLOGSTRINGITEM_H_

#include <string>
#include "DynLogItemBase.h"

namespace Logging
{

class DynLogStringItem : public DynLogItemBase
{
public:
  DynLogStringItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp, const std::string& format = DYNLOG_DEFAULTFORMAT);
  ~DynLogStringItem(void);

  /*
   *  IDynLogItem implementation
   */
  std::string serialise();
  void serialise(char*& buffer, size_t& size);

protected:

  void replacePlaceHolder(std::string& buffer, const std::string placeholder, const std::string value);
  void replacePlaceHolder(std::string& buffer, const std::string placeholder, const size_t value);
};

}; // end of namespace Logging

#endif//_DYNLOGSTRINGITEM_H_