#ifndef _DBIMYSQL_H_
#define _DBIMYSQL_H_

#include "IDBInterface.h"

class DBIMySql : public IDBInterface
{
public:
  DBIMySql(void);
  virtual ~DBIMySql(void);

  /*
   *  IDBInterface implementation
   */
  bool Connect(std::string server, short port, std::string dbname, std::string user, std::string pass);
  bool Disconnect();

private:

};

#endif//_DBIMYSQL_H_