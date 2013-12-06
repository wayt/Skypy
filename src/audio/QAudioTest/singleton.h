#ifndef SINGLETON_H
#define SINGLETON_H

template<typename T>
class Singleton
{
public:
    virtual ~Singleton() {}

    static T* instance()
    {
        static T *_instance;

        if (!_instance)
            _instance = new T();

        return _instance;
    }

protected:
    Singleton() {}
    Singleton(const Singleton &other) {}
    Singleton& operator=(const Singleton &other) { return *this; }
};

#endif // SINGLETON_H
