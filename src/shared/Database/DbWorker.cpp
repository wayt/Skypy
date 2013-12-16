#include "DbWorker.h"

DbWorker::DbWorker(DbQueue *queue, DbConnection *conn) : _queue(queue), _conn(conn)
{
}

void DbWorker::run()
{
    if (!_queue)
        return;

    std::string request;
    while (1337)
    {
        request = _queue->dequeue();
        _conn->execute(request.c_str());
    }
}
