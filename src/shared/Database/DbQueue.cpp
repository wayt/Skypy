#include "DbQueue.h"

DbQueue::DbQueue() : _queue(), _mutex(), _cond()
{
}

void DbQueue::enqueue(std::string const &sql)
{
    boost::mutex::scoped_lock lock(_mutex);

    _queue.push(sql);
    _cond.notify();
}

std::string const &DbQueue::dequeue()
{
    boost::mutex::scoped_lock lock(_mutex);

    while (!_queue.size())
        _cond.wait(_mutex);

    std::string ret = _queue.front();
    _queue.pop();

    return ret;
}

unsigned int DbQueue::size()
{
    boost::mutex::scoped_lock lock(_mutex);

    return _queue.size();
}
