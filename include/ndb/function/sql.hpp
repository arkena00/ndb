#ifndef FUNCTION_SQL_H_NDB
#define FUNCTION_SQL_H_NDB

#include <ndb/function/generic.hpp>
#include <ndb/engine.hpp>
#include <ndb/utility.hpp>

#include <iostream>

namespace ndb::functions
{
    template<class Engine, class Database, class Table>
    struct clear<Engine, Database, Table, ndb::expression_categories::sql>
    {
        static auto process()
        {
            const auto& engine = ndb::engine<Engine>::get();
            auto q = "DELETE FROM `" + std::string{ ndb::table_name<Table> } + "`";
            return engine.template exec<Database>(q);
        }
    };

    template<class Engine, class Database>
    struct remove<Engine, ndb::expression_categories::sql, Database>
    {
        static auto process()
        {
            return "DROP DATABASE `" + std::string{ ndb::database_name<Database> } + "`";
        }
    };
} // ndb::functions

#endif // FUNCTION_SQL_H_NDB