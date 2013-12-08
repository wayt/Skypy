#ifndef MUTEX_H_
# define MUTEX_H_

#include <boost/thread/thread.hpp>

class Mutex
{
public:
    Mutex() : _mutex() {}

    void lock() { _mutex.lock(); }
    void unlock() { _mutex.unlock(); }

    class ScopLock
    {
    public:
        ScopLock(Mutex& mutex) : _mutex(mutex) { _mutex.lock(); }
        ~ScopLock() { _mutex.unlock(); }
    private:
        Mutex& _mutex;
    };
private:
    boost::mutex _mutex;
};

#endif /* !MUTEX_H_ */
