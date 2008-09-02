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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "Daemon.h"

CDaemon::CDaemon()
: m_pLogic(NULL)
, m_bRunAsApp(false)
{
}

///////////////////////////////////////////////////////////////////

CDaemon::~CDaemon()
{
}

///////////////////////////////////////////////////////////////////

bool CDaemon::ISRV_AttachLogic(IServiceLogic* pLogic)
{
  if ( !pLogic )
    return false;
  
  m_pLogic = pLogic;
  
  return true;  
}

///////////////////////////////////////////////////////////////////

void CDaemon::ISRV_RunAsApp(bool bAsApp)
{
  m_bRunAsApp = bAsApp;
}

///////////////////////////////////////////////////////////////////

int CDaemon::ISRV_Run(void* pData)
{
  if ( !m_bRunAsApp )
  {
    pid_t pid;
  
    // clear the file creation mask
    umask(0);
  
    if ( (pid = fork()) < 0 )
    {
      // something bad happened here...
      return -1;
    }
    else if ( pid != 0 )
    {
      return 0;
    }
  
    setsid();

    // execute the harnessed service logic here...
    m_pLogic->Start();
  }
  else
  {
    if ( !m_pLogic )
      return 0;
    
    m_pLogic->Start();

    printf("Press 'q' to stop...\n");
    while ( getchar() != 'q' )
    {
      printf(".");
    }
  }
  
  
  return 0;
}

///////////////////////////////////////////////////////////////////

int CDaemon::ISRV_Stop()
{
  if ( !m_pLogic )
    return -1;
  
  // TODO: Flesh this out
  
  return 0;  
}
