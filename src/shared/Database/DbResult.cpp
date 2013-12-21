#include <iostream>
#include "DbResult.h"

DbResult::DbResult(MYSQL_RES *result, MYSQL_FIELD *fields, int nbFields, int nbRows)
    : _result(result), _fields(fields), _nbFields(nbFields), _nbRows(nbRows), _currentRow(NULL)
{
    if (_nbFields > 0)
        _currentRow = new DbField[_nbFields];
}

DbResult::~DbResult()
{
    delete[] _currentRow;
    mysql_free_result(_result);
}

bool DbResult::fetch()
{
    MYSQL_ROW row;

    if (!_result)
        return false;

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
