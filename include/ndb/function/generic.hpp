#ifndef FUNCTION_GENERIC_H_NDB
#define FUNCTION_GENERIC_H_NDB

#include <ndb/expression/code.hpp>
#include <ndb/cx_error.hpp>

namespace ndb::functions
{
    template<ndb::expr_category_code Expr_category, class Database, class Table>
    struct clear
    {
        static void process()
        {
            ncx_error(clear, cx_err_not_implemented);
        }
    };

    template<ndb::expr_category_code Expr_category, class Database>
    struct remove
    {
        static void process()
        {
            ncx_error(remove, cx_err_not_implemented);
        }
    };
} // ndb::functions

#endif // FUNCTION_GENERIC_H_NDB