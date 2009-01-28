/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include "../common/Logging/DynLog.h"
#include "../common/fctypes.h"
#ifdef _WIN32
  #include "../common/daemon/win/W32Service.h"
#else
  #include "../common/daemon/linux/Daemon.h"
#endif//_WIN32
#include "../common/threading.h"
#include "../common/cmdlineinfo.h"
#include "../common/interfaces/IService.h"
#include "FCLogicRouter.h"

IService* CreateServerObject();
void ReleaseServerObject(IService* pService);
bool HandleCommandLine(CCmdLineInfo& cmdLine, IService* pService);

///////////////////////////////////////////////////////////////////////

int main(int argc, FCSTR argv[])
{
  IService* pService = CreateServerObject();
  IServiceLogic* pLogic = new FCLogicRouter;
  CCmdLineInfo  cmdLine(argc, argv);

  // create the logger
  Logging::IDynLogWriter* pWriter = Logging::createLogWriter( Logging::LOGWRITER_TEXTFILE);
  ((Logging::IDynLogTextFileWriter*)pWriter)->setFilename("./FCServer.log");
  Logging::getLogger()->addWriter(pWriter);
  pWriter->release();

  if ( pService )
  {
    pService->ISRV_AttachLogic(pLogic);
    // handle any command line options passed in
    if ( HandleCommandLine(cmdLine, pService) )
    {
      // if we get here, we can boot the service/daemon
      pService->ISRV_Run(NULL);
    }
    // time to free up the resources
    pLogic->Free();
  }

  // and release the service/daemon object
  ReleaseServerObject(pService);
  
  Logging::getLogger()->release();

	return 0;
}

///////////////////////////////////////////////////////////////////////

IService* CreateServerObject()
{
  IService* ps = 0;

#ifdef _WIN32
  CW32Service* serv = new CW32Service;

  ps = serv;  
#else
  // instantiate daemons for other POSIX based systems
  CDaemon* serv = new CDaemon;
  ps = serv;
#endif

  return ps;
}

///////////////////////////////////////////////////////////////////////

void ReleaseServerObject(IService* pService)
{
  if ( !pService )
    return;

#ifdef _WIN32
  delete (CW32Service*)pService;
#else
  delete (CDaemon*)pService;
#endif
}

///////////////////////////////////////////////////////////////////////

bool HandleCommandLine(CCmdLineInfo& cmdLine, IService* pService)
{
  if ( !pService )
    return true;

#ifdef _WIN32
	if ( cmdLine.IsSwitchPresent("i") )
  {
		CW32Service::InstallService( (CW32Service*)pService );
    return false;
  }
	else if ( cmdLine.IsSwitchPresent("u") )
  {
		CW32Service::UninstallService( (CW32Service*)pService );
    return false;
  }
#endif//_WIN32

  if ( cmdLine.IsSwitchPresent("a") )
  {
    pService->ISRV_RunAsApp(true);
    // add a console writer to the log object
    Logging::IDynLogWriter* pWriter = Logging::createLogWriter( Logging::LOGWRITER_CONSOLE );
    if ( pWriter )
    {
      pWriter->setOutputFormat("[[time]] [data]");
      Logging::getLogger()->addWriter(pWriter);
      pWriter->release();
    }
  }

  return true;
}