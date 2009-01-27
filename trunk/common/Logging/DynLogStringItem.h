#ifndef _DYNLOGSTRINGITEM_H_
#define _DYNLOGSTRINGITEM_H_

#include <string>
#include <sstream>
#include "DynLogItemBase.h"

namespace Logging
{

class DynLogStringItem : public DynLogItemBase
{
public:
  DynLogStringItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp)
    : DynLogItemBase(data, sourceFile, sourceLineNum, timestamp)
  {
  }

  ~DynLogStringItem(void)
  {
  }

  /*
   *  IDynLogItem implementation
   */
  std::string serialise()
  {
    std::stringstream ss;
    std::string result, ts = asctime(&m_timestamp);

    if ( ts[ts.size()-1] == '\n' )
      ts[ts.size()-1] = '\0';

    ss << "[" << ts << "] " << m_data << " (" << m_sourceFile << ":" << (unsigned int)m_sourceLineNum << ")\n";
    result = ss.str();

    return result;
  }

  void serialise(char*& buffer, size_t& size)
  {
    std::string result = serialise();
    size = result.size();

    buffer = new char[size+1];
    memset(buffer, 0, size+1);
    memcpy(buffer, result.c_str(), size);
  }
};

}; // end of namespace Logging

#endif//_DYNLOGSTRINGITEM_H_