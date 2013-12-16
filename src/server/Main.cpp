#include <iostream>
#include "DbConnection.h"
#include "DbWorkerPool.h"

int main(int ac, char** av)
{
    DbInfo dbInfo;
    dbInfo.host = "localhost";
    dbInfo.user = "root";
    dbInfo.password = "";
    dbInfo.database = "skypy";
    dbInfo.port = 3306;

    DbConnection dbConnection(dbInfo);

    if (!dbConnection.open())
        std::cout << "Impossible de se connecter" << std::endl;
    else
        std::cout << "Vous etes dorenavant connecte au serveur MySQL" << std::endl;

    DbWorkerPool pool(dbInfo, 2);
    pool.execute("INSERT INTO account(id, username) VALUES(NULL, 'TOTO');");
    pool.waitAll();

    return 0;
}
