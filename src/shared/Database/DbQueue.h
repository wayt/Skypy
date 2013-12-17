#ifndef DBQUEUE_H_
# define DBQUEUE_H_

#include <string>
#include <queue>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include "DbQuery.h"

class DbQueue
{
public:
    DbQueue();
    void enqueue(DbQuery *query);
    DbQuery *dequeue();
    unsigned int size();

private:
    std::queue<DbQuery *> _queue;
    boost::mutex _mutex;
    boost::condition_variable _cond;
};

#endif
