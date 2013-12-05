#ifndef DBCONNECTION_H_
# define DBCONNECTION_H_

#include <string>
#include <memory>
#include <mysql.h>
#include "DbResult.h"

struct DbInfo
{
    std::string host;
    int port;
    std::string user;
    std::string password;
    std::string database;
};


class DbConnection
{
private:
    DbInfo const &_info;
    MYSQL *_conn;
public:
    DbConnection(DbInfo const &info);
    bool hasError() const;
    bool open();
    void close();
    bool execute(const char *sql);
    std::shared_ptr<DbResult> query(const char *sql);
};

typedef std::shared_ptr<DbResult> pDbResult;

#endif
