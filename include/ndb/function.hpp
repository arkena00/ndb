#ifndef FUNCTION_H_NDB
#define FUNCTION_H_NDB

#include <ndb/function/generic.hpp>

namespace ndb
{
    template<class Database, class Table>
    void clear(Table table)
    {
        ndb::functions::clear<typename Database::engine, Database, Table>::process();
    }

    template<class Database>
    void remove()
    {
        ndb::functions::remove<typename Database::engine, Database>::process();
    }
} // ndb

#endif // FUNCTION_H_NDB