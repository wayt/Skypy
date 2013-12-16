#ifndef DBWORKER_H_
# define DBWORKER_H_

#include "DbConnection.h"
#include "DbQueue.h"

class DbWorker
{
public:
    DbWorker(DbQueue *queue, DbConnection *_conn);
    void run();
private:
    DbQueue *_queue;
    DbConnection *_conn;
};

#endif
