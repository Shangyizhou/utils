#include "db.h"

DB::DB()
{
    mysql = mysql_init(nullptr);
    assert(mysql != nullptr);
}

DB::~DB()
{
    if (!mysql)
        mysql_close(mysql);
}

bool DB::InitDB(std::string host, 
                std::string user, 
                std::string password, 
                std::string database_name)
{
    mysql = mysql_real_connect( mysql, 
                                host.c_str(), 
                                user.c_str(), 
                                password.c_str(), 
                                database_name.c_str(), 
                                3306, NULL, 0);
    if (mysql == nullptr) {
        std::cout << "InitDB() failed" << std::endl;
        exit(1);
    }
    
    return true;
}

bool DB::Execute(std::string sql)
{
    // 成功返回 0
    if (mysql_query(mysql, sql.c_str()))
    {
        std::cout << "Query Error: " << mysql_error(mysql) << std::endl;
        return false;
    }

    // 获取查询结果
    result = mysql_store_result(mysql);
    if (!result) 
    {
        // 更新情况
        if (mysql_field_count(mysql) == 0)
        {
            int num_rows = mysql_affected_rows(mysql);
            return true;
        } else { // 出错情况
            std::cout << "Get Result Error: " << mysql_error(mysql);
            return false;
        }
    }
    
    int num_fields = mysql_num_fields(result);
    int num_rows = mysql_num_rows(result);
    for (int i = 0; i < num_rows; ++i) 
    {
        row = mysql_fetch_row(result);

        for (int j = 0; j < num_fields; ++j)
        {
            std::cout << row[j] << " \t\t";
        }
        std::cout << std::endl;
    }
}