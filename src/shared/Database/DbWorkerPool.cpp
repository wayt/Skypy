#include "DbWorkerPool.h"

DbWorkerPool::DbWorkerPool()
    : ThreadPool(), _queue(), _conns()
{
}

bool DbWorkerPool::initialize(DbInfo const& info, unsigned int nbWorker)
{
    for (int i = 0; i < nbWorker; ++i)
    {
        DbConnection *conn = new DbConnection(info);
        if (!conn->open())
            return false;
        create_thread(new DbWorker(&_queue, conn));
        _conns.push_back(conn);
    }
    return true;
}

std::string const& DbWorkerPool::getLastError() const
{
    for (std::list<DbConnection*>::const_iterator itr = _conns.begin();
            itr != _conns.end(); ++itr)
        if ((*itr)->hasError())
            return (*itr)->getLastError();
    static std::string error = "";
    return error;
}

void DbWorkerPool::execute(std::string const &sql)
{
    _queue.enqueue(new DbQuery(QUERY_ASYNC, sql));
}

pDbResult DbWorkerPool::query(std::string const &sql)
{
    DbQuery* query = new DbQuery(QUERY_SYNC, sql);

    _queue.enqueue(query);

    pDbResult result = query->getResult();

    query->wait();

    delete query;

    return result;
}

void DbWorkerPool::waitAll()
{
    join_all();
}
