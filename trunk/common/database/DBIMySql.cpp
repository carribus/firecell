#define HAVE_STRTOK_R
#ifdef _WIN32
#include "my_global.h"
#endif//_WIN32
#include "mysql.h"
#include "dbimysql.h"

DBIMySql::DBIMySql(void)
{
  mysql_library_init(0, NULL, NULL);
}

DBIMySql::~DBIMySql(void)
{
  mysql_library_end();
}

IDBconnection* DBIMySql::Connect(std::string server, short port, std::string dbname, std::string user, std::string pass)
{
//  MYSQL* sql = mysql_init(NULL);

//  mysql_close(sql);
/*
  if ( ! mysql_real_connect(sql,
                            server.c_str(),
                            user.c_str(),
                            pass.c_str(),
                            dbname.c_str(),
                            port, 
                            NULL, 
                            0) )
  {
    // failed to connect to the DB
    return false;
  }
*/
  return true;
}

bool DBIMySql::Disconnect()
{
  return false;
}

void DBIMySql::Release()
{
  delete this;
}