#include "DbWorkerPool.h"

DbWorkerPool::DbWorkerPool(DbInfo const &info, unsigned int nbWorker)
    : ThreadPool(), _queue(), _conn(info)
{
    _conn.open();

    for (int i = 0; i < nbWorker; ++i)
        create_thread(new DbWorker(&_queue, &_conn));
}

void DbWorkerPool::execute(std::string const &sql)
{
    _queue.enqueue(sql);
}

/* TODO: struct DbQuery
{
    const char* sql;
    bool done;
    Mutex mutex;
    Condition cond;
};

pDbResult DbWorkerPool::query(const char *sql)
{
    DbQuery* query = new DbQuery(sql);

    _appendQuery(query);

    Lock lock(query->mutex);

    while (!query->done)
        query->cond.wait(query->mutex);

    DbResult result = query.result;
    delete query;
    return result;

    boost::unique_lock<boost::mutex> lock(_mutex);

    _cond.wait(lock);
    _conn.query(sql);
    _cond.notify();

    return _conn.query(sql);
}*/

void DbWorkerPool::waitAll()
{
    join_all(); 
}
