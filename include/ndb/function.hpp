#ifndef FUNCTION_H_NDB
#define FUNCTION_H_NDB

#include <ndb/function/generic.hpp>

namespace ndb
{
    template<class Database, class Table>
    void clear(Table table)
    {
        ndb::functions::clear<Database::engine::expr_category(), Database, Table>::process();
    }

    template<class Database>
    void remove()
    {
        ndb::functions::remove<Database::engine::expr_category(), Database>::process();
    }
} // ndb

#endif // FUNCTION_H_NDB