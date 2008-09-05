#ifndef _PEPACKET_H_
#define _PEPACKET_H_

#include <map>
#include <string>

using namespace std;

class PEPacket
{
  struct Field
  {
    string name;
    size_t elem_width;
    size_t elem_count;
    size_t offset;
  };

public:
  PEPacket(void);
  virtual ~PEPacket(void);

  void SetField(const string& name, size_t offset, size_t elem_width, size_t elem_count);
  bool FieldExists(const string& name);
  size_t GetFieldSize(const string& name);
  size_t GetFieldOffset(const string& name);
  bool GetField(const string& key, void* pDest, size_t size);
  void SetDataBlock(const char* pData, size_t blockLen);

private:

  typedef map< string, Field > FieldMap;
  FieldMap                      m_fields;
  char*                         m_pDataBlock;
};

#endif//_PEPACKET_H_
