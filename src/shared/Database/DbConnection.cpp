#include <iostream>
#include "DbConnection.h"

DbConnection::DbConnection(DbInfo const &info) : _info(info), _conn(NULL)
{
}

bool DbConnection::hasError() const
{
    if (*mysql_error(_conn))
    {
        std::cerr << mysql_error(_conn) << std::endl;
        return true;
    }
    return false;
}

bool DbConnection::open()
{
    _conn = mysql_init(NULL);

    if (_conn == NULL)
    {
        std::cerr << "Fail mysql_init" << std::endl;
        return false;
    }

    mysql_real_connect(_conn, _info.host.c_str(), _info.user.c_str(), _info.password.c_str(), _info.database.c_str(), _info.port, NULL, 0);

    if (hasError())
    {
        close();
        return false;
    }
    return true;
}

bool DbConnection::execute(const char *sql)
{
    mysql_query(_conn, sql);

    return !hasError();
}

pDbResult DbConnection::query(const char *sql)
{
    if (!execute(sql))
        return NULL;

    MYSQL_RES *result = mysql_store_result(_conn);
    if (hasError())
        return NULL;

    int nbFields = mysql_num_fields(result);
    int nbRows = mysql_affected_rows(_conn);
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    return std::shared_ptr<DbResult>(new DbResult(result, fields, nbFields, nbRows));
}

void DbConnection::close()
{
    if (_conn)
    {
        mysql_close(_conn);
        _conn = NULL;
    }
}
