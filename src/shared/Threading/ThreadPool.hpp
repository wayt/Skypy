#ifndef THREADPOOL_H_
# define THREADPOOL_H_

#include "Thread.hpp"

class ThreadPool
{
public:

    template<class T>
    void create_thread(T *func) { _threads.push_back(new Thread(func)); }
    void join_all()
    {
        for (std::list<Thread*>::iterator itr = _threads.begin(); itr != _threads.end(); ++itr)
            (*itr)->join();
    }

private:
    std::list<Thread*> _threads;
};

#endif /* !THREADPOOL_H_ */
