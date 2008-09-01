/*
 *  Daemon.cpp
 *  Daemon1
 *
 *  Created by Peter Mares on 2008/09/01.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "Daemon.h"

CDaemon::CDaemon()
{
}

///////////////////////////////////////////////////////////////////

CDaemon::~CDaemon()
{
}

///////////////////////////////////////////////////////////////////

int CDaemon::Run()
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
  
  return 0;
}