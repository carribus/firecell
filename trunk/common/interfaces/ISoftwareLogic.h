#ifndef _ISOFTWARELOGIC_H_
#define _ISOFTWARELOGIC_H_

#ifndef _FCCLIENT_COMPILE_

#include <string>

struct ISoftwareLogic
{
  virtual bool Execute(const std::string& cmd, const std::string& args, std::string& result) = 0;
};

#endif//_FCCLIENT_COMPILE_

#endif//_ISOFTWARELOGIC_H_