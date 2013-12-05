#include <iostream>
#include "DbResult.h"

DbResult::DbResult(MYSQL_RES *result, MYSQL_FIELD *fields, int nbFields, int nbRows)
    : _result(result), _fields(fields), _nbFields(nbFields), _nbRows(nbRows), _currentRow(NULL)
{
    _currentRow = new DbField[_nbFields];
}

bool DbResult::fetch()
{
    MYSQL_ROW row;

    row = mysql_fetch_row(_result);

    if (row == NULL)
        return false;

    for (int i = 0; i < _nbFields; ++i)
    {
        _currentRow[i].setValue(row[i]);
    }
    return true;
}

DbField const *DbResult::operator[](std::string const &index) const
{
    for (int i = 0; i < _nbFields; ++i)
    {
        if (index == _fields[i].name)
            return &(_currentRow[i]);
    }
    return NULL;
}
