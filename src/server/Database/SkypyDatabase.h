#ifndef SKYPYDATABASE_H_
# define SKYPYDATABASE_H_

#include "SharedDefines.h"
#include "DbWorkerPool.h"
#include "Singleton.hpp"

class SkypyDatabase : public DbWorkerPool, public Singleton<SkypyDatabase>
{
public:
    SkypyDatabase();

    bool initialize();
};

#define sSkypyDb SkypyDatabase::instance()

#endif /* !SKYPYDATABASE_H_ */
