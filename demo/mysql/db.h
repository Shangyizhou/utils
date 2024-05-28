#include <iostream>
#include <mysql/mysql.h>
#include <assert.h>

class DB
{
private:
    MYSQL*      mysql;  // 连接 MySQL 的指针
    MYSQL_RES*  result; // 指向查询结果的指针
    MYSQL_ROW   row;    // 按行返回的查询信息

public:
    DB();
    ~DB();

    bool InitDB(std::string host, 
                std::string user, 
                std::string password, 
                std::string database_name);
    
    bool Execute(std::string sql);
};