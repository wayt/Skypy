#ifndef DBQUERY_H_
# define DBQUERY_H_

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include "DbConnection.h"

enum QueryType
{
    QUERY_ASYNC,
    QUERY_SYNC
};

class DbQuery
{
public:
    DbQuery(QueryType type, std::string const &sql);
    QueryType getType() const;
    pDbResult getResult() const;
    void setResult(pDbResult const result);
    char const *getSql() const;
    void wait();
    void notify();
private:
    boost::mutex _mutex;
    boost::condition_variable _cond;
    QueryType _type;
    pDbResult _result;
    std::string _sql;
    bool _done;
};

#endif

