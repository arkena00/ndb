#ifndef EXPRESSION_SQL_NATIVE_H_NDB
#define EXPRESSION_SQL_NATIVE_H_NDB

#include <ndb/expression/code.hpp>
#include <ndb/expression/utility.hpp>
#include <ndb/expression/native.hpp>
#include <array>

namespace ndb
{
    template<>
    struct native_expression<expr_category_code::sql, expr_type_code::init>
    {
        template<class Expr, expr_clause_code Clause, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            // get
            ne.push_back(clause_code<Clause, expr_category_code::sql>::value);

            // get : table
            if constexpr (Expr::clause() == expr_clause_code::source)
            {
                ne.push_back("* FROM ");
            }

            // get : condition
            if constexpr (Expr::clause() == expr_clause_code::condition)
            {
                if constexpr (Clause == expr_clause_code::get)
                {
                    ne.push_back("*");
                }

                ne.push_back(clause_code<expr_clause_code::source, expr_category_code::sql>::value);
                ne.push_back("T");
                ne.push_back(deduce_source<Expr>() + 48);
                ne.push_back(clause_code<expr_clause_code::condition, expr_category_code::sql>::value);
            }
            // add
            if constexpr (Clause == expr_clause_code::add)
            {
                ne.push_back("T");
                ne.push_back(deduce_source<Expr>() + 48);
                ne.push_back('(');
            }

            Expr::static_make(ne);

            // get : fields
            if constexpr (Expr::clause() == expr_clause_code::get)
            {
                ne.push_back(clause_code<expr_clause_code::source, expr_category_code::sql>::value);
                ne.push_back("T");
                ne.push_back(deduce_source<Expr>() + 48);
            }

            // add
            if constexpr (Clause == expr_clause_code::add)
            {
                ne.push_back(')');
                ne.push_back(clause_code<expr_clause_code::value_list, expr_category_code::sql>::value);
                ne.push_back('(');
                Expr::template static_make<1>(ne);
                ne.push_back(')');
            }

            // terminate the string
            ne.push_back('\0');
        }
    };

    // table
    template<>
    struct native_expression<expr_category_code::sql, expr_type_code::table>
    {
        template<class T, expr_clause_code Clause, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            ne.push_back('T');
            ne.push_back(ndb::table_id<T> + 48);
        }
    };

    // field
    template<>
    struct native_expression<expr_category_code::sql, expr_type_code::field>
    {
        template<class T, expr_clause_code Clause, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            ne.push_back('F');
            ne.push_back(ndb::field_id<T> + 48);
        }
    };

    // value
    template<>
    struct native_expression<expr_category_code::sql, expr_type_code::value>
    {
        template<class T, expr_clause_code Clause, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            ne.push_back_value();
        }
    };
} // ndb

#endif // EXPRESSION_SQL_NATIVE_H_NDB
