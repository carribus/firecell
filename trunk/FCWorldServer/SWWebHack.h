#ifndef _SWWEBHACK_H_
#define _SWWEBHACK_H_

#include <vector>
#include "Company.h"
#include "Player.h"
#include "SWLogicBase.h"

class SWWebHack : public SWLogicBase
{
public:
  SWWebHack(void);
  virtual ~SWWebHack(void);

  bool Execute(Player* pPlayer, const std::string& cmd, const std::string& args, std::string& result);
};

#endif//_SWWEBHACK_H_