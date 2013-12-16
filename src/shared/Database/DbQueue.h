#ifndef DBQUEUE_H_
# define DBQUEUE_H_

#include <string>
#include <queue>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

class DbQueue
{
public:
    DbQueue();
    void enqueue(std::string const &sql);
    std::string dequeue();
    unsigned int size();

private:
    std::queue<std::string> _queue;
    boost::mutex _mutex;
    boost::condition_variable _cond;
};

#endif
