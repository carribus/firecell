/**
 *  \defgroup DynLog DynLog Logging Framework
 */
#ifndef _DYNLOG_H_
#define _DYNLOG_H_

#include "IDynLog.h"

#define DYNLOG_FORMAT \
  Logging::getLogger()->formatLog

#define DYNLOG_ADDLOG1(str) \
  Logging::getLogger()->addLog(str, __FILE__, __LINE__, 1);

#define DYNLOG_ADDLOG2(str) \
  Logging::getLogger()->addLog(str, __FILE__, __LINE__, 2);

#define DYNLOG_ADDLOG3(str) \
  Logging::getLogger()->addLog(str, __FILE__, __LINE__, 3);

#define DYNLOG_ADDLOG4(str) \
  Logging::getLogger()->addLog(str, __FILE__, __LINE__, 4);

#define DYNLOG_ADDLOG5(str) \
  Logging::getLogger()->addLog(str, __FILE__, __LINE__, 5);

#define DYNLOG_ADDLOG( str ) DYNLOG_ADDLOG1(str)

namespace Logging
{

/**
 *  \ingroup DynLog
 *  \brief This function must be called to retrieve the singleton instance of the logger object
 */
IDynLogger* getLogger();

/**
 *  \ingroup DynLog
 *  \brief This function must be called when you are ready to release the logger for good
 */
void destroyLogger();

/**
 *  \ingroup DynLog
 *  \brief Creates a log writer object
 *
 *  Call this function to create a log writer object. The kind of object created is specified by the type parameter
 *
 *  @param type   This parameter will indicate the type of logwriter object to create
 *  @return       If successful, this function will return a pointer to an IDynLogWriter interface that represents the
 *                request object that has been created. If the function has failed, NULL will be returned.
 */
IDynLogWriter* createLogWriter(E_LOGWRITER_TYPE type);

};  // end of namespace Logging
#endif//_DYNLOG_H_