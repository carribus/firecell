#ifndef _COMPANY_H_
#define _COMPANY_H_

#include <string>
#include "../common/fctypes.h"
#include "InGameIPAddress.h"

using namespace std;

class Company
{
public:
  Company(void);
  ~Company(void);

  void SetID(FCULONG id)                    { m_id = id; }
  void SetName(const string& name)          { m_name = name; }
  void SetCityID(FCULONG id)                { m_cityID = id; }
  void SetIP(const InGameIPAddress& ip)     { m_ip = ip; }
  FCULONG GetID()                           { return m_id; }
  string GetName()                          { return m_name; }
  FCULONG GetCityID()                       { return m_cityID; }
  InGameIPAddress& GetIP()                  { return m_ip; }

private:

  FCULONG       m_id;
  string        m_name;
  FCULONG       m_cityID;
  InGameIPAddress m_ip;
};

#endif//_COMPANY_H_