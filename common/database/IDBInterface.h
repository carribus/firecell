#ifndef _IDBINTERFACE_H_
#define _IDBINTERFACE_H_

#include <string>

struct IDBInterface
{
  virtual bool Connect(std::string server, short port, std::string dbname, std::string user, std::string pass) = 0;
  virtual bool Disconnect() = 0;
};

#endif//_IDBINTERFACE_H_