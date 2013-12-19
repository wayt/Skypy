#ifndef QSYNCHRONIZEDQUEUE_H
#define QSYNCHRONIZEDQUEUE_H

#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

template<typename T>
class QSynchronizedQueue
{
public:
    QSynchronizedQueue() :
        _q(),
        _mutex(QMutex::NonRecursive),
        _cond()
    {}
    virtual ~QSynchronizedQueue() {}

    T dequeue();
    void enqueue(const T &t);

    T& head();

    void clear();

    bool isEmpty();

    int size();

private:
    QQueue<T> _q;
    QMutex _mutex;
    QWaitCondition _cond;
};

template<typename T>
T QSynchronizedQueue<T>::dequeue()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock);

    while (_q.isEmpty())
        _cond.wait(&_mutex);

    return _q.dequeue();
}

template<typename T>
T& QSynchronizedQueue<T>::head()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock);

    while (_q.isEmpty())
        _cond.wait(&_mutex);

    return _q.head();
}

template<typename T>
void QSynchronizedQueue<T>::enqueue(const T &t)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock);

    _q.enqueue(t);
    _cond.wakeOne();
}

template<typename T>
void QSynchronizedQueue<T>::clear()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock);

    _q.clear();
}

template<typename T>
bool QSynchronizedQueue<T>::isEmpty()
{
    return _q.isEmpty();
}

template<typename T>
int QSynchronizedQueue<T>::size()
{
    return _q.size();
}

#endif // QSYNCHRONIZEDQUEUE_H
