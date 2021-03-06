#ifndef _DYNLOGTEXTFILEWRITER_H_
#define _DYNLOGTEXTFILEWRITER_H_

#include <fstream>
#include "DynLogItemBase.h"
#include "DynLogStringItem.h"
#include "IDynLog.h"

namespace Logging
{

/**
 *  \class DynLogTextfileWriter
 *  \brief This writer is used for log output to a specified file
 */
class DynLogTextfileWriter : public IDynLogTextFileWriter
{
public:
  DynLogTextfileWriter(void);
  ~DynLogTextfileWriter(void);

  /*
   *  IDynLogTextFileWriter implementation
   */
  void setOutputFormat(const std::string& format);
  bool startWriter();
  void stopWriter();
  E_LOGWRITER_TYPE getType();
  std::string getName();
  IDynLogItem* createLogItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp);
  bool writeItem(IDynLogItem* item);
  void setFilename(const std::string& filename);
  std::string getFilename();

private:

  std::string           m_filename;
  std::ofstream         m_file;  

  std::string           m_format;
};

}; // end of namespace Logging

#endif//_DYNLOGTEXTFILEWRITER_H_