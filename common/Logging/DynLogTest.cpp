// DynLogTest.cpp : Defines the entry point for the console application.
//

#include "../firecell/common/Logging/DynLog.h"

using namespace Logging;

int _tmain(int argc, _TCHAR* argv[])
{
  // create the logger
  IDynLogger* pLogger = createLogger();
  IDynLogWriter* pWriter = createLogWriter(LOGWRITER_TEXTFILE);

  // first lets create a file writer
  if ( pWriter )
  {
    ((IDynLogTextFileWriter*)pWriter)->setFilename("testlog.txt");
    if ( !pLogger->addWriter( pWriter ) )
    {
      pLogger->release();
      return -1;
    }
    pWriter->release();
  }

  // now create a console writer
  if ( (pWriter = createLogWriter(LOGWRITER_CONSOLE)) )
  {
    if ( !pLogger->addWriter( pWriter ) )
    {
      pLogger->release();
      return -2;
    }
    pWriter->release();
  }

  // add some log items
  pLogger->addLog("Log entry #1", __FILE__, __LINE__);
  pLogger->addLog("Another log entry (2)", __FILE__, __LINE__);
  pLogger->addLog("My last log entry...", __FILE__, __LINE__);

  // free up the logger.
  // this causes all allocated writers to be flushed and cleaned up
  pLogger->release();

	return 0;
}

