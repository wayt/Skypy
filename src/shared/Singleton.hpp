#ifndef SINGLETON_H_
# define SINGLETON_H_

#include <memory>

template<class T>
class Singleton
{
public:
    static T* instance()
    {
        static T* obj = NULL;
        if (!obj)
            obj = new T();
        return obj;
    }
};

#endif /* !SINGLETON_H_ */
