/*
 *  Daemon.h
 *  Daemon1
 *
 *  Created by Peter Mares on 2008/09/01.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _DAEMON_H_
#define _DAEMON_H_

class CDaemon
{
public:
  CDaemon();
  virtual ~CDaemon();
  
  int Run();
  
private:
};

#endif//_DAEMON_H_

