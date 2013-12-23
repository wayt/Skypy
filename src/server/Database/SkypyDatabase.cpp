#include "SkypyDatabase.h"
#include "ConfigMgr.h"
#include <iostream>

SkypyDatabase::SkypyDatabase()
{
}

bool SkypyDatabase::initialize()
{
    DbInfo dbInfo;
    dbInfo.host = sConfig->getStringDefault("MySQL.host", "localhost");
    dbInfo.user = sConfig->getStringDefault("MySQL.user", "root");
    dbInfo.password = sConfig->getStringDefault("MySQL.password", "");
    dbInfo.database = sConfig->getStringDefault("MySQL.database", "skypy");
    dbInfo.port = sConfig->getIntDefault("MySQL.port", 3306);

    uint32 threads = sConfig->getIntDefault("MySQL.ThreadCount", 2);

    std::cout << "Opening MySQL connection " << dbInfo.user << "@" << dbInfo.host << ":" << dbInfo.port << " with " << threads << " threads" << std::endl;
    bool ok = true;
    if (!DbWorkerPool::initialize(dbInfo, threads))
    {
        std::cout << "Fail to init database" << std::endl;
        ok = false;
    }
    return ok;
}
