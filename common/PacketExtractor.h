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
#ifndef _PACKETEXTRACTOR_H_
#define _PACKETEXTRACTOR_H_

#include <vector>
#include <string>
#include "PEPacket.h"

/*
  ///////////////////////////////////////
  // sample code
  ///////////////////////////////////////

  #pragma pack(push)
  #pragma pack(1)
    struct stPacket
    {
      char  magic[5];         // magic header id (must be 'HELLO'
      short sVerMajor;        // major version
      short sVerMinor;        // minor version
      char pktType;            // type of packet
      long lDataLen;          // length of data
      char pData[32];         // payload of the packet
    } tmpPkt;
  #pragma pack(pop)


  PacketExtractor extractor;
  PEPacket* pPkt = NULL;

  const std::string PKTDEF_SAMPLE = "[" \
                               ":magic:1:5|" \
                               ":verMajor:2:1|" \
                               ":verMinor:2:1|" \
                               ":pktType:1:1|" \
                               ":dataLen:4:1|" \
                               ":data:*dataLen:1|" \
                               "]";

  extractor.Prepare(PKTDEF_SAMPLE);

  memcpy( tmpPkt.magic, "HELLO", 5 );
  tmpPkt.sVerMajor = 1;
  tmpPkt.sVerMinor = 2;
  tmpPkt.pktType = 0x0A;
  tmpPkt.lDataLen = 32;
  memcpy( tmpPkt.pData, "01234567890123456789012345678901", 32 );

  char* pStream = new char[sizeof(stPacket)];
  memcpy(pStream, &tmpPkt, sizeof(stPacket));
  size_t offset = 0;
  if ( (pPkt = extractor.Extract(pStream, offset, sizeof(stPacket))) )
  {
    short sVal = 0;
    size_t len = 0;
    char header[5];
    char data[32];
    pPkt->GetField("magic", header, 5);
    pPkt->GetField("verMajor", &sVal, sizeof(short));
    pPkt->GetField("verMinor", &sVal, sizeof(short));
    pPkt->GetField("dataLen", &len, sizeof(size_t));
    pPkt->GetField("data", data, sizeof(data));
    delete pPkt;
  }

  delete [] pStream;
*/

using namespace std;

/*
 *  Definition specific constants
 */
#define PKTDEF_START              '['
#define PKTDEF_FIELD_START        ':'
#define PKTDEF_ELEMWIDTH_START    PKTDEF_FIELD_START
#define PKTDEF_VARNAME_START      '*'
#define PKTDEF_ELEMCOUNT_START    PKTDEF_FIELD_START
#define PKTDEF_FIELD_END          '|'
#define PKTDEF_END                ']'

class PacketExtractor
{
  struct EPField
  {
    std::string name;
    std::string varRefWidth;
    size_t elem_width;
    std::string varRefCount;
    size_t elem_count;
  };

public:
  PacketExtractor(void);
  virtual ~PacketExtractor(void);

  bool Prepare(std::string strPacketFormat);
  PEPacket* Extract(const char* pSrc, size_t& streamOffset, const size_t streamLen);

protected:

  virtual bool ParseFieldDef(std::string strDef, PacketExtractor::EPField& field);
  virtual void ClearField(PacketExtractor::EPField& field);

  virtual bool ReadElement(const char* pSrc, size_t& streamOffset, const size_t streamLen, void* pDest, const size_t elemsize);

  void AddToExtractionPlan(PacketExtractor::EPField& field);
  void CalculateMinPacketSize();

private:

  size_t            m_minPktSize;
  typedef vector<EPField> FieldVector;
  FieldVector       m_ep;
};

#endif//_PACKETEXTRACTOR_H_