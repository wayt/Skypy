#ifndef QSYNCHRONIZEDQUEUE_H
#define QSYNCHRONIZEDQUEUE_H

#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>

template<typename T>
class QSynchronizedQueue : public QQueue<T>
{
public:
    QSynchronizedQueue() :
        QQueue<T>(),
        _mutex(QMutex::NonRecursive),
        _cond()
    {}
    virtual ~QSynchronizedQueue() {}

    T dequeue();
    void enqueue(const T &t);

    /*bool isEmpty();

    int size();*/

private:
    QMutex _mutex;
    QWaitCondition _cond;
};

template<typename T>
T QSynchronizedQueue<T>::dequeue()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock);

    while (this->isEmpty())
        _cond.wait(&_mutex);

    return QQueue<T>::dequeue();
}

template<typename T>
void QSynchronizedQueue<T>::enqueue(const T &t)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock);

    QQueue<T>::enqueue(t);
    _cond.wakeOne();
}

/*template<typename T>
bool QSynchronizedQueue<T>::isEmpty()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock);

    return QQueue<T>::isEmpty();
}

template<typename T>
int QSynchronizedQueue<T>::size()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock);

    return QQueue<T>::size();
}*/

#endif // QSYNCHRONIZEDQUEUE_H
