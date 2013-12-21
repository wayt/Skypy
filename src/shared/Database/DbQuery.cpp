#include "DbQuery.h"

DbQuery::DbQuery(QueryType type, std::string const &sql)
    : _mutex(), _cond(), _type(type), _result(), _sql(sql), _done(false)
{
}

QueryType DbQuery::getType() const
{
    return _type;
}

DbResultPtr DbQuery::getResult() const
{
    return _result;
}

char const *DbQuery::getSql() const
{
    return _sql.c_str();
}

void DbQuery::setResult(DbResultPtr const result)
{
    _result = result;
}

void DbQuery::wait()
{
    boost::unique_lock<boost::mutex> lock(_mutex);

    while (!_done)
        _cond.wait(lock);
}

void DbQuery::notify()
{
    _done = true;
    _cond.notify_one();
}
