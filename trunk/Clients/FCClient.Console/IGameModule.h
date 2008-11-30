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
#ifndef _IGAMEMODULE_H_
#define _IGAMEMODULE_H_

class FCServerObj;
class PEPacket;
class BaseSocket;

struct IGameModule;

struct IGameModuleSink
{
  virtual void CloseModule(IGameModule* pModule) = 0;
};

struct IGameModule
{
  virtual void RegisterSink(IGameModuleSink* pSink) = 0;
  virtual void SetCharacterID(FCULONG character_id) = 0;
  virtual void SetServerObject(FCServerObj* pServer) = 0;
  virtual void QueueForAction() = 0;
  virtual bool OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket) = 0;
};

#endif//_IGAMEMODULE_H_