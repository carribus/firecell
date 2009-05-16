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
#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "../../PThreadCond.h"
#include "../../interfaces/IService.h"
#include "../../interfaces/IServiceLogic.h"

class CDaemon : public IService
{
public:
  CDaemon();
  virtual ~CDaemon();

  //
  // IService Implementation
  bool ISRV_AttachLogic(IServiceLogic* pLogic);
  void ISRV_RunAsApp(bool bAsApp);
  int ISRV_Run(void* pData);
  int ISRV_Stop();
  void ISRV_GetBinPath(char* outBuffer, size_t& bufSize);

private:

  static void* ThreadFunction(void* pParam);

  bool                  m_bRunAsApp;
  IServiceLogic*        m_pLogic;
  pthread_t             m_thrd;
  PThreadCond           m_cond;
};

#endif//_DAEMON_H_

