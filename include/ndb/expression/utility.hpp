#ifndef EXPRESSION_UTILITY_H_NDB
#define EXPRESSION_UTILITY_H_NDB

#include <ndb/utility.hpp>
#include <ndb/expression/type.hpp>

namespace ndb
{
    struct statement_;


    template<class L, class R>
    using enable_expression = std::enable_if_t<(ndb::is_expression<L> || ndb::is_expression<R>
                                               || ndb::is_field<L> || ndb::is_field<R>
                                               || ndb::is_table<L> || ndb::is_table<R>) && !std::is_same_v<L, ndb::statement_>
    >;



    template<class T>
    constexpr auto expr_make(const T& v)
    {
        if constexpr (ndb::is_expression<T>) return v;
        else if constexpr (ndb::is_field<T>)
        {
            return ndb::expression<ndb::expressions::field, T>{ v };
        }
        else if constexpr (ndb::is_table<T>) return ndb::expression<ndb::expressions::table_, T> {};
        // return value expression
        else return ndb::expression<expressions::value, T> { v };
    }
} // ndb

#endif // EXPRESSION_UTILITY_H_NDB