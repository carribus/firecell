#ifndef _DBIMYSQLRESULTSET_H_
#define _DBIMYSQLRESULTSET_H_

#ifndef HAVE_STRTOK_R
  #define HAVE_STRTOK_R
#endif//HAVE_STRTOK_R
#ifdef _WIN32
#include "my_global.h"
#endif//_WIN32
#include "mysql.h"

class DBIMySqlResultSet
{
public:
  DBIMySqlResultSet(void);
  ~DBIMySqlResultSet(void);

  void Process(MYSQL_RES* result);
};

#endif//_DBIMYSQLRESULTSET_H_