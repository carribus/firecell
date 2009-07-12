#ifndef _SWPORTSCAN_H_
#define _SWPORTSCAN_H_

#include <vector>
#include "Company.h"
#include "Player.h"
#include "../common/interfaces/ISoftwareLogic.h"

class SWPortScan : public ISoftwareLogic
{
public:
  SWPortScan(void);
  virtual ~SWPortScan(void);

  bool Execute(const std::string& cmd, const std::string& args, std::string& result);

private:

  std::string getPortDetails(Company* pCompany);
  std::string getPortDetails(Player* pPlayer);
  std::vector<std::string> getArguments(const std::string& args);

};

#endif//_SWPORTSCAN_H_