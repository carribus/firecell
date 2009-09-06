#ifndef _SWPORTSCAN_H_
#define _SWPORTSCAN_H_

#include <vector>
#include "Company.h"
#include "Player.h"
#include "SWLogicBase.h"

class SWPortScan : public SWLogicBase
{
public:
  SWPortScan(void);
  virtual ~SWPortScan(void);

  bool Execute(Player* pPlayer, const std::string& cmd, const std::string& args, std::string& result);

private:

  std::string getPortDetails(Company* pCompany);
  std::string getPortDetails(Player* pPlayer);
};

#endif//_SWPORTSCAN_H_