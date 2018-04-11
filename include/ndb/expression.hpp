#ifndef EXPRESSION_H_NDB
#define EXPRESSION_H_NDB

#include <ndb/expression/code.hpp>
#include <ndb/expression/fwd.hpp>

//! .tpp include operators to overload them

namespace ndb
{
    struct expression_base {};

    template<class L = void, expr_type_code T = expr_type_code::value, class R = void, expr_clause_code Clause = expr_clause_code::none>
    struct expression : expression_base
    {
        static constexpr auto type = T;

        const L lhs_;
        const R rhs_;

        constexpr expression(const L& lhs, const R& rhs);

        template<class F>
        constexpr void eval(F&& f) const;

        template<class Native_expression>
        constexpr void make(Native_expression& ne) const;

        template<class F>
        static constexpr void static_eval(F&& f);

        template<int Pass = 0, class Native_expression>
        static constexpr void static_make(Native_expression& ne);

        static constexpr auto clause();
    };
} // ndb

#include <ndb/expression.tpp>

#endif // EXPRESSION_H_NDB