#include <iostream>
#include "DbConnection.h"

int main(int ac, char **av) {

    DbInfo dbInfo;
    dbInfo.host = "localhost";
    dbInfo.user = "root";
    dbInfo.password = "";
    dbInfo.database = "skypy";
    dbInfo.port = 3306;

    DbConnection dbConnection(dbInfo);

    if (!dbConnection.open())
        std::cout << "Impossible de se connecter" << std::endl;

    dbConnection.execute("INSERT INTO account(id, username) VALUES(NULL, 'TOTO')");

    pDbResult result = dbConnection.query("SELECT * FROM account");
    while (result->fetch())
    {
        std::cout << (*result)["username"]->getValue() << std::endl;
    }

    dbConnection.close();

    return 0;
}
