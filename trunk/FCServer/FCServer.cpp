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
#include "../common/fctypes.h"
#ifdef _WIN32
  #include "win/W32Service.h"
  // include the the Visual Studio compatible Structured Exception handling version of the pthreads-win32 library
  #pragma comment(lib, "../common/pthreads-win32/lib/pthreadVSE2.lib")
#else
  #include "linux/Daemon.h"
#endif//_WIN32
#include "../common/cmdlineinfo.h"
#include "interfaces/IService.h"
#include "FCLogicRouter.h"

IService* CreateServerObject();
void ReleaseServerObject(IService* pService);
bool HandleCommandLine(CCmdLineInfo& cmdLine, IService* pService);

///////////////////////////////////////////////////////////////////////

#include "../common/PacketExtractor.h"

int main(int argc, FCSTR argv[])
{
  IService* pService = CreateServerObject();
  IServiceLogic* pRouter = new FCLogicRouter;
  CCmdLineInfo  cmdLine(argc, argv);

/*
  ** TEMPORARY **
  sample packet definition (for testing purposes only)

  struct stPacket
  {
    char  magic[5];         // magic header id (must be 'HELLO'
    short sVerMajor;        // major version
    short sVerMinor;        // minor version
    char pktType            // type of packet
    long lDataLen;          // length of data
    char* pData;            // payload of the packet
  };

  // definition format
  start of definition = [[
    start of field      = :
      field name          = alphanumeric sequence up to next ':'
    element width start = :
      element width (size)= value
    number of elements  = [0..inf] OR
                          (reference to previous member definition)
    end of field        = |
  end of definition   = ]]

  // sample code
  PacketExtractor extractor;

  const string PKTDEF_SAMPLE = "[" \
                               ":magic:1:5|" \
                               ":verMajor:2:1|" \
                               ":verMinor:2:1|" \
                               ":pktType:1:1|" \
                               ":dataLen:4:1|" \
                               ":data:*dataLen:1|" \
                               "]";

  extractor.Prepare(PKTDEF_SAMPLE);
*/

  if ( pService )
  {
    pService->ISRV_AttachLogic(pRouter);
    // handle any command line options passed in
    if ( HandleCommandLine(cmdLine, pService) )
    {
      // if we get here, we can boot the service/daemon
      pService->ISRV_Run(NULL);
    }
    // time to free up the resources
    pRouter->Free();
  }

  // and release the service/daemon object
  ReleaseServerObject(pService);

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
  }

  return true;
}