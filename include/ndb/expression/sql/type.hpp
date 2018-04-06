#ifndef EXPRESSION_SQL_TYPE_H_NDB
#define EXPRESSION_SQL_TYPE_H_NDB

#include <ndb/expression/sql/code.hpp>
#include <ndb/expression/type.hpp>
#include <ndb/expression/utility.hpp>

namespace ndb
{
    // default make for expression_type
    template<expr_type_code T>
    struct expression_type<T, expr_category_code::sql>
    {
        template<class L, class R, int Pass, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            L::template static_make<Pass>(ne);
            ne.push_back(expr_code<T, expr_category_code::sql>::value);
            R::template static_make<Pass>(ne);
        }
    };

    // shift_left
    template<>
    struct expression_type<expr_type_code::op_shift_left, expr_category_code::sql>
    {
        template<class L, class R, int Pass, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            L::template static_make<Pass>(ne);

            // get << source
            if constexpr (expr_has_clause<R, expr_clause_code::source>)
            {
                ne.push_back(clause_code<R::clause(), expr_category_code::sql>::value);
            }

            // get << not source
            if constexpr (!expr_has_clause<L, expr_clause_code::source> && !expr_has_clause<R, expr_clause_code::source>)
            {
                ne.push_back(clause_code<expr_clause_code::source, expr_category_code::sql>::value);
                ne.push_back("T");
                ne.push_back(deduce_source<L>() + 48);
            }

            // not condition << condition
            if constexpr (!(expr_has_clause<L, expr_clause_code::condition>) && expr_has_clause<R, expr_clause_code::condition>)
            {
                ne.push_back(clause_code<expr_clause_code::condition, expr_category_code::sql>::value);
            }

            R::template static_make<Pass>(ne);
        }
    };

    // assign
    template<>
    struct expression_type<expr_type_code::op_assign, expr_category_code::sql>
    {
        template<class L, class R, int Pass, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            if constexpr (Pass == 0)
            {
                L::template static_make<Pass>(ne);
            }
            else R::template static_make<Pass>(ne);
        }
    };
} // ndb

#endif // EXPRESSION_SQL_TYPE_H_NDB
