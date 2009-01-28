#include "DynLogger.h"
#include "DynLogNullWriter.h"
#include "DynLogConsoleWriter.h"
#include "DynLogTextfileWriter.h"
#include "DynLog.h"

namespace Logging
{

/**
 *  \brief This function must be called to create a new instance of the logger
 */
IDynLogger* getLogger()
{
  IDynLogger* pLogger = DynLogger::instance();

  return pLogger;
}

/**
 *  \brief This function must be called when you are ready to release the logger for good
 */
void destroyLogger()
{
  DynLogger::destroy();
}

/**
 *  \brief Creates a log writer object
 *
 *  Call this function to create a log writer object. The kind of object created is specified by the type parameter
 *
 *  @param type   This parameter will indicate the type of logwriter object to create
 *  @return       If successful, this function will return a pointer to an IDynLogWriter interface that represents the
 *                request object that has been created. If the function has failed, NULL will be returned.
 */
IDynLogWriter* createLogWriter(E_LOGWRITER_TYPE type)
{
  switch ( type )
  {
  case  LOGWRITER_NULL:
    return ( new DynLogNullWriter );
    break;

  case  LOGWRITER_TEXTFILE:
    return ( new DynLogTextfileWriter );

  case  LOGWRITER_CONSOLE:
    return ( new DynLogConsoleWriter );
    break;
  }

  return NULL;
}

}; // end of namespace Logging