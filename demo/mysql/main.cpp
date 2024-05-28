#include "db.h"
#include <memory>

int main()
{
    std::string host = "127.0.0.1";
    std::string user = "root";
    std::string password = "200166_Shangjkld";
    std::string database_name = "webapp";

    std::unique_ptr<DB> db(new DB);
    db->InitDB(host, user, password, database_name);
    db->Execute("SELECT * FROM users");

    return 0;
}