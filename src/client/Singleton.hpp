#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template<class T>
class Singleton
{
public:
    static T* instance()
    {
        static T* obj = 0x0;
        if (!obj)
            obj = new T();
        return obj;
    }
};
#endif // SINGLETON_HPP
