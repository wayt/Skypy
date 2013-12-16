#include "DbQueue.h"

DbQueue::DbQueue() : _queue(), _mutex(), _cond()
{
}

void DbQueue::enqueue(std::string const &sql)
{
    boost::unique_lock<boost::mutex> lock(_mutex);

    _queue.push(sql);
    _cond.notify_one();
}

std::string DbQueue::dequeue()
{
    boost::unique_lock<boost::mutex> lock(_mutex);

    while (!_queue.size())
        _cond.wait(lock);

    std::string ret = _queue.front();
    _queue.pop();

    return ret;
}

unsigned int DbQueue::size()
{
    boost::unique_lock<boost::mutex> lock(_mutex);

    return _queue.size();
}
