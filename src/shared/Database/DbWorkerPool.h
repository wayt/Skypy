#ifndef DBWORKERPOOL_H_
# define DBWORKERPOOL_H_

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include "DbQueue.h"
#include "DbConnection.h"
#include "ThreadPool.hpp"
#include "DbWorker.h"

class DbWorkerPool : ThreadPool
{
public:
    DbWorkerPool(DbInfo const &info, unsigned int nbWorker);
    void execute(std::string const &sql);
    /*pDbResult query(const char *sql);*/
    void waitAll();
private:
    DbQueue _queue;
    DbConnection _conn;
};

#endif
