#ifndef _IDYNLOG_H_
#define _IDYNLOG_H_

#include <string>
#include "RefCounter.h"

#ifndef NULL
#define NULL 0
#endif//NULL

namespace Logging
{

#define DYNLOG_FORMAT_TIME    "[time]"
#define DYNLOG_FORMAT_DATA    "[data]"
#define DYNLOG_FORMAT_FILE    "[file]"
#define DYNLOG_FORMAT_LINE    "[line]"
#define DYNLOG_DEFAULTFORMAT  "[" DYNLOG_FORMAT_TIME "] " DYNLOG_FORMAT_DATA " (" DYNLOG_FORMAT_FILE ":" DYNLOG_FORMAT_LINE ")"

 /**
 *  \brief This enumeration is used to specify the type of log writer
 */
enum E_LOGWRITER_TYPE
{
  LOGWRITER_NONE,
  LOGWRITER_NULL,             //!< A Null log writer - consumes log items like a normal writer, but has no output
  LOGWRITER_TEXTFILE,         //!< A text file writer - dumps log items to a pre-specified text file location
  LOGWRITER_CONSOLE           //!< A console writer - dumps log items to a console
};

/**
 *  \interface IDynLogItem
 *  \ingroup DynLog
 *  \brief The IDynLogItem interface is the base interface that all log item implementations must expose
 */
struct IDynLogItem : public IRefCounter
{
  /**
   *  \brief Return the data that this log item was coninterfaceed with
   */
  virtual std::string getData() = 0;

  /**
   *  \brief Return the source file name that was passed in during the log call
   */
  virtual const char* getSourceFile() = 0;

  /**
   *  \brief Return the source file's line number at which the log call occurred
   */
  virtual size_t getSourceLineNum() = 0;

  /**
   *  \brief Serialises the content of the item to a string in a format specified by the log item's implementation
   */
  virtual std::string serialise() = 0;

  /**
   *  \brief Serialises the content of the item and copies it to the buffer specified.
   *
   *  @param buffer   A pointer that will receive an allocated buffer with the serialised contents of the log item.
   *  @param size     A reference variable that will receive the length of the allocated buffer returned in parameter 1
   */
  virtual void serialise(char*& buffer, size_t& size) = 0;
};

/**
 *  \interface IDynLogWriter
 *  \ingroup DynLog
 *  \brief The IDynLogWriter interface exposes functionality a particular writer.
 *
 *  If you wish to create your own writer object (for eg a database writer), you 
 *  will need to implement the IDynLogWriter interface in your object, as well as
 *  create a IDynLogItem derived object that can serialise itself to your required
 *  format.
 */
struct IDynLogWriter : public IRefCounter
{
  virtual void setOutputFormat(const std::string& format = DYNLOG_DEFAULTFORMAT) = 0;
  /**
   *  \brief Allows the writer to initialise any sub-components it may have before it starts writing. This is automatically called
   *         when a writer is passed into the IDynLogger::addWriter() method
   */
  virtual bool startWriter() = 0;

  /**
   *  \brief Allows the writer to shut itself down gracefully. This is automatically called when the IDynLogger object
   *         is destroyed (i.e. its last reference is released)
   */
  virtual void stopWriter() = 0;

  /**
   *  \brief Query the logger for its type (see Logging::E_LOGWRITER_TYPE)
   */
  virtual E_LOGWRITER_TYPE getType() = 0;

  /**
   *  \brief Returns the name of the writer object
   */
  virtual std::string getName() = 0;

  /**
   *  \brief The writer create the IDynLogItem object that it has been configured to work with
   */
  virtual IDynLogItem* createLogItem(const std::string& data, const char* sourceFile, size_t sourceLineNum, tm& timestamp) = 0;

  /**
   *  \brief The writer will serialise the item data to its intended destination when this function is called
   */
  virtual bool writeItem(IDynLogItem* item) = 0;
};

/**
 *  \interface IDynLogTextFileWriter
 *  \ingroup DynLog
 *  \brief This interface is an extension of IDynLogWriter and exposes the required functions for setting/getting a target filename
 */
struct IDynLogTextFileWriter : public IDynLogWriter
{
  /**
   *  \brief Set the output filename for the writer
   */
  virtual void setFilename(const std::string& filename) = 0;

  /**
   *  \brief Get the file this writer is logging to
   */
  virtual std::string getFilename() = 0;
};

/**
 *  \interface IDynLogger
 *  \ingroup DynLog
 *  \brief The IDynLogger interface is used by the consumers of the DynLog framework.
 *
 *  IDynLogger is received as the result of a call to createLogger().
 */
struct IDynLogger : public IRefCounter
{
  /**
   *  \brief Attach a writer endpoint to the logger.
   *
   *  Use this function to attach writers to the logger object. You can attach more than one in the
   *  case that you need to output logs to more than one destination.
   *
   *  @return If successful, the count of attached writers will be returned. If the function fails, 0 will be returned
   */ 
  virtual size_t addWriter(IDynLogWriter* writer) = 0;

  /**
   *  \brief Retrieves the writer object attached to the logger at a specified index
   */
  virtual IDynLogWriter* getWriter(size_t index) = 0;

  /**
   *  \brief Returns the number of writers attached to the logger object
   */
  virtual size_t getWriterCount() = 0;

  /**
   *  \brief Call flush to give priority to the logger to flush any pending back logs
   */
  virtual void flush() = 0;

  /**
   *  \brief Retrieves the size of the logger's backlog
   */
  virtual size_t getBackLog() = 0;

  /**
   *  \brief Adds data to be logged
   *
   *  A sample call to this would look something like this:
   *
   *  \code
   *  IDynLogger* pLogger = createLogger(LOGWRITER_TEXTFILE);
   *  .
   *  .
   *  pLogger->addLog("This is a log item", __FILE__, __LINE__);
   *  \endcode
   */
  virtual size_t addLog(const std::string& data, const char* sourceFile, size_t sourceLineNum, size_t logLevel) = 0;

  virtual std::string formatLog(const char* data, ...) = 0;

  virtual void setLogLevelFilter(size_t maxLevelToLog) = 0;
};

}; // end of Logging namespace

#endif//_IDYNLOG_H_