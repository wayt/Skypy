Database
========

Pre-requis
----------

Il nous faut la librairie MySQL :
Sous Debian/Ubuntu : `apt-get install libmysqlclient-dev`

Exemple d'implementation basique
--------------------------------

```cpp
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

DbResult *result = dbConnection.query("SELECT id, username FROM account");

while (result->fetch())
{
    std::cout << (*result)["username"]->getValue() << std::endl;
}

```

Exemple d'implementation complete
---------------------------------

```cpp
DbInfo dbInfo;
dbInfo.host = "localhost";
dbInfo.user = "root";
dbInfo.password = "";
dbInfo.database = "skypy";
dbInfo.port = 3306;

DbWorkerPool pool(dbInfo, 4);
pool.execute("INSERT INTO account(id, username) VALUES(NULL, 'TOTO');");

DbResult *result = pool.query("SELECT id, username FROM account");

while (result->fetch())
{
    std::cout << (*result)["username"]->getValue() << std::endl;
}

pool.waitAll();

```
