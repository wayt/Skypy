#include "DbQueue.h"

DbQueue::DbQueue() : _queue(), _mutex(), _cond()
{
}

void DbQueue::enqueue(DbQuery *query)
{
    boost::unique_lock<boost::mutex> lock(_mutex);

    _queue.push(query);
    _cond.notify_one();
}

DbQuery *DbQueue::dequeue()
{
    boost::unique_lock<boost::mutex> lock(_mutex);

    while (!_queue.size())
        _cond.wait(lock);

    DbQuery *ret = _queue.front();
    _queue.pop();

    return ret;
}

unsigned int DbQueue::size()
{
    boost::unique_lock<boost::mutex> lock(_mutex);

    return _queue.size();
}
