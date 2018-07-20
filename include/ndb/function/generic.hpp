#ifndef FUNCTION_GENERIC_H_NDB
#define FUNCTION_GENERIC_H_NDB

#include <ndb/cx_error.hpp>

namespace ndb::functions
{
    template<class Engine, class Database, class Table, ndb::expression_categories Expr_category = Engine::expr_category>
    struct clear
    {
        static auto process()
        {
            ncx_error_not_implemented(Engine);
        }
    };

    template<class Engine, ndb::expression_categories Expr_category, class Database>
    struct remove
    {
        static auto process()
        {
            ncx_error_not_implemented(Engine);
        }
    };
} // ndb::functions

#endif // FUNCTION_GENERIC_H_NDB