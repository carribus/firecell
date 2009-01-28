#ifndef _DYNLOGNULLWRITER_H_
#define _DYNLOGNULLWRITER_H_

#include "IDynLog.h"

namespace Logging
{

/**
 *  \class DynLogNullWriter
 *  \brief This writer is used as a null output device.
 *
 *  If you intend to create your own Log writer, then you can use this as a template for your code
 */
class DynLogNullWriter : public IDynLogWriter
{
public:
  DynLogNullWriter(void);
  ~DynLogNullWriter(void);

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