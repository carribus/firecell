#include "DynLogItemBase.h"

namespace Logging
{

DynLogItemBase::DynLogItemBase(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp, const std::string& format)
: m_data(data)
, m_sourceFile(sourceFile)
, m_sourceLineNum(sourceLineNum)
, m_timestamp(timestamp)
, m_format(format)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

DynLogItemBase::~DynLogItemBase(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string DynLogItemBase::getData()
{
  return m_data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const char* DynLogItemBase::getSourceFile()
{
  return m_sourceFile;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t DynLogItemBase::getSourceLineNum()
{
  return m_sourceLineNum;
}

}