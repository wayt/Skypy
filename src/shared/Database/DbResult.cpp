#include <iostream>
#include "DbResult.h"

DbResult::DbResult(MYSQL_RES *result, MYSQL_FIELD *fields, int nbFields, int nbRows)
    : _result(result), _fields(fields), _nbFields(nbFields), _nbRows(nbRows), _currentRow(NULL)
{
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(_result)))
    {
        for (int i = 0; i < _nbFields; ++i)
        {
            std::cout << "tab[" << _fields[i].name << "] = " << row[i] << std::endl;
        }
    }
}
