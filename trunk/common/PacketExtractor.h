#ifndef _PACKETEXTRACTOR_H_
#define _PACKETEXTRACTOR_H_

#include <map>
#include <string>

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
    string name;
    string varRefWidth;
    size_t elem_width;
    string varRefCount;
    size_t elem_count;
  };

public:
  PacketExtractor(void);
  virtual ~PacketExtractor(void);

  bool Prepare(string strPacketFormat);

protected:

  virtual bool ParseFieldDef(string strDef, PacketExtractor::EPField& field);
  virtual void ClearField(PacketExtractor::EPField& field);

  virtual bool ReadElement(const char* pSrc, size_t& streamOffset, const size_t streamLen, void* pDest, const size_t elemsize);
};

#endif//_PACKETEXTRACTOR_H_