#include "DbWorker.h"

DbWorker::DbWorker(DbQueue *queue, DbConnection *conn) : _queue(queue), _conn(conn)
{
}

void DbWorker::run()
{
    if (!_queue)
        return;

    DbQuery *query = NULL;
    while (1337)
    {
        query = _queue->dequeue();
        if (query == NULL)
            continue;
        if (query->getType() == QUERY_ASYNC)
            _conn->execute(query->getSql());
        else
        {
            query->setResult(_conn->query(query->getSql()));
            query->notify();
        }
    }
}
