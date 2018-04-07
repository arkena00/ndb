#ifndef FUNCTION_SQL_H_NDB
#define FUNCTION_SQL_H_NDB

#include <ndb/function/generic.hpp>

#include <iostream>

namespace ndb::functions
{
    template<class Database, class Table>
    struct clear<ndb::sqlite, Database, Table>
    {
        static void process()
        {
            std::cout << "TRUNCATE TABLE `T" + std::to_string(ndb::table_id<Table>) + "`";
        }
    };

    template<class Database>
    struct remove<ndb::sqlite, Database>
    {
        static void process()
        {
            std::cout << "DROP DATABASE `D" + std::to_string(ndb::database_id<Database>) + "`";
        }
    };
} // ndb::functions

#endif // FUNCTION_SQL_H_NDB