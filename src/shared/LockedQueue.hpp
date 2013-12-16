#ifndef LOCKEDQUEUE_H_
# define LOCKEDQUEUE_H_

#include <queue>
#include "SharedDefines.h"
#include "Mutex.hpp"

template<class T>
class LockedQueue
{
public:
    void add(T* data)
    {
        Mutex::ScopLock lock(_mutex);
        _queue.push(data);
    }

    T* get()
    {
        Mutex::ScopLock lock(_mutex);
        if (_queue.empty())
            return NULL;
        T* data = _queue.front();
        _queue.pop();
        return data;
    }

    uint32 size()
    {
        Mutex::ScopLock lock(_mutex);
        return _queue.size();
    }

    bool empty()
    {
        Mutex::ScopLock lock(_mutex);
        return _queue.empty();
    }

private:
    Mutex _mutex;
    std::queue<T*> _queue;
};

#endif /* !LOCKEDQUEUE_H_ */
