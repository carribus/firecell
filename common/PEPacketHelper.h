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
#ifndef _PEPACKETHELPER_H_
#define _PEPACKETHELPER_H_

#include "fctypes.h"
#include "protocol/fcprotocol.h"
#include "PEPacket.h"

class PEPacketHelper
{
public:
  PEPacketHelper(void);
  ~PEPacketHelper(void);

  static void CreatePacket(PEPacket& pkt, FCBYTE type, FCSHORT msgID, ServiceType target = ST_Router);
  static void SetPacketData(PEPacket& pkt, void* pData, size_t dataLen);

protected:

  static void CreateBasePacket(PEPacket& pkt, FCBYTE type, ServiceType target);
};

#endif//_PEPACKETHELPER_H_