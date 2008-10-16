#include "DBIMySqlResultSet.h"

DBIMySqlResultSet::DBIMySqlResultSet(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIMySqlResultSet::~DBIMySqlResultSet(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void DBIMySqlResultSet::Process(MYSQL_RES* result)
{
  MYSQL_ROW row;
  unsigned int numFields = mysql_num_fields(result);
  MYSQL_FIELD* fields = mysql_fetch_fields(result);

  while ( (row = mysql_fetch_row(result)) )
  {
#error You left off here
  }
}