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

dbConnection.query("SELECT * FROM account");
```
