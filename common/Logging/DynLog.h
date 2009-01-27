/**
 *  \defgroup DynLog
 */
#ifndef _DYNLOG_H_
#define _DYNLOG_H_

#include "IDynLog.h"

namespace Logging
{

/**
 *  \brief This function must be called to create a new instance of the logger
 */
IDynLogger* createLogger();

/**
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