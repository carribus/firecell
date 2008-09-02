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
#ifndef _FCLOGICROUTER_H_
#define _FCLOGICROUTER_H_

#include "interfaces/IServiceLogic.h"
#include "interfaces/ISocketServer.h"


class FCLogicRouter : public IServiceLogic
                    , public ISocketServerSink
{
public:
  FCLogicRouter(void);
  ~FCLogicRouter(void);

  //
  // IServiceLogic implementation
  const char* GetName()                           { return "FireCell Router Service"; }
  void Free();
  int Start();
  int Stop();

  //
  // ISocketServerSink implementation
	void OnConnect(FCSOCKET s);
	void OnDisconnect(FCSOCKET s, FCDWORD dwCode);
	void OnDataReceived(FCSOCKET s, FCBYTE* pData, FCINT nLen);


private:

  ISocketServer*                m_pSockServer;
};

#endif//_FCLOGICROUTER_H_