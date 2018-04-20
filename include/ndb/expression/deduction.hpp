#ifndef EXPRESSION_DEDUCTION_H_NDB
#define EXPRESSION_DEDUCTION_H_NDB

#include <ndb/expression/function.hpp>
#include <iostream>

//TODO make all deductions in one browse

namespace ndb
{
    // deduce source
    template<class Expr>
    constexpr auto deduce_source_id()
    {
        // get source from first field
        return Expr::static_eval([&](auto&& e)
        {
            if constexpr (e.type == expr_type_code::field)
            {
                using Table = typename std::decay_t<decltype(e)>::value_type::table;
                return ndb::table_id<Table>;
            }
        });
    }
} // ndb

#endif // EXPRESSION_DEDUCTION_H_NDB
