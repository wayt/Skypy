#include "DbWorkerPool.h"

DbWorkerPool::DbWorkerPool(DbInfo const &info, unsigned int nbWorker)
    : ThreadPool(), _queue(), _conns()
{
    for (int i = 0; i < nbWorker; ++i)
    {
        DbConnection *conn = new DbConnection(info);
        conn->open();
        create_thread(new DbWorker(&_queue, conn));
        _conns.push_back(conn);
    }
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
