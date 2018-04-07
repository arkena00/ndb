#ifndef FUNCTION_GENERIC_H_NDB
#define FUNCTION_GENERIC_H_NDB

#include <ndb/cx_error.hpp>

namespace ndb::functions
{
    template<class Engine, class Database, class Table>
    struct clear
    {
        static void process()
        {
            ncx_error(clear, cx_err_not_implemented, Engine, Database, Table);
        }
    };

    template<class Engine, class Database>
    struct remove
    {
        static void process()
        {
            ncx_error(remove, cx_err_not_implemented, Engine, Database);
        }
    };
} // ndb::functions

#endif // FUNCTION_GENERIC_H_NDB