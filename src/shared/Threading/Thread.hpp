#ifndef THREAD_H_
# define THREAD_H_

#include <boost/thread/thread.hpp>
#include <functional>

class Thread
{
public:
    //Thread(std::function<void(void)> &func) : _thread(func) {}
    template<class T>
    Thread(T& obj) : _thread(boost::bind(&T::run, &obj)) {}

    void join() { _thread.join(); }

    static void Sleep(unsigned int ms) { boost::this_thread::sleep(boost::posix_time::milliseconds(ms)); }
private:
    boost::thread _thread;
};

#endif /* !THREAD_H_ */
