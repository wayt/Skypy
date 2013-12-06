#ifndef DBRESULT_H_
# define DBRESULT_H_

#include <cstdlib>
#include <mysql.h>
#include "DbField.h"

class DbResult
{
private:
    MYSQL_RES *_result;
    MYSQL_FIELD *_fields;
    int _nbFields;
    int _nbRows;
    DbField *_currentRow;
public:
    DbResult(MYSQL_RES *result, MYSQL_FIELD *fields, int nbFields, int nbRows);
    ~DbResult();
    bool fetch();
    DbField const *operator[](std::string const &index) const;
};

#endif

