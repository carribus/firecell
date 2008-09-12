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
#include "PEPacketHelper.h"

PEPacketHelper::PEPacketHelper(void)
{
}

///////////////////////////////////////////////////////////////////////

PEPacketHelper::~PEPacketHelper(void)
{
}

///////////////////////////////////////////////////////////////////////

void PEPacketHelper::CreatePacket(PEPacket& pkt, FCBYTE type, FCSHORT msgID)
{
  CreateBasePacket(pkt, type);
  pkt.SetFieldValue("msg", &msgID);
}

///////////////////////////////////////////////////////////////////////

void PEPacketHelper::SetPacketData(PEPacket& pkt, void* pData, size_t dataLen)
{
  if ( pkt.FieldExists("dataLen") )
  {
    pkt.AddField("data", dataLen, 1, pData);
    pkt.SetFieldValue("dataLen", &dataLen);
  }
}

///////////////////////////////////////////////////////////////////////

void PEPacketHelper::CreateBasePacket(PEPacket& pkt, FCBYTE type)
{
  unsigned char header[8] = { 0x0A, 0x0A, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40 };

  pkt.AddField("magic", 1, 8, header);
  pkt.AddField("type", 1, 1, &type);
  pkt.AddField("msg", 2, 1);
  pkt.AddField("dataLen", 4, 1);
}

