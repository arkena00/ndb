#ifndef EXPRESSION_H_NDB
#define EXPRESSION_H_NDB

#include <ndb/expression/type.hpp>
#include <ndb/expression/native.hpp>
#include <ndb/expression/utility.hpp>

/*! expression<Left_expression, expression_type ( [operators, field, value] ), Right_expression>
 * expression::type : [operator, field, value]
 * expression::clause : get, condition
 */

namespace ndb
{
    struct expression_base {};

    template<class L, expr_type_code T = expr_type_code::value, class R = void, expr_clause_code Clause = expr_clause_code::none>
    struct expression : expression_base
    {
        static constexpr auto type = T;

        const L lhs_;
        const R rhs_;

        constexpr expression(const L& lhs, const R& rhs) :
            lhs_(lhs),
            rhs_(rhs)
        {}

        static constexpr auto clause()
        {
            return L::clause() | R::clause() | Clause;
        }

        template<class F>
        constexpr void eval(F&& f) const
        {
            lhs_.eval(f);
            rhs_.eval(f);
        }

        template<class F>
        static constexpr void static_eval(F&& f)
        {
            L::static_eval(f);
            R::static_eval(f);
        }

        template<class Native_expression>
        constexpr void make(Native_expression& ne) const
        {
            expression_type<T, Native_expression::expr_category>::make(lhs_, rhs_, ne);
        }

        template<int Pass = 0, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            expression_type<T, Native_expression::expr_category>::template static_make<L, R, Pass>(ne);
        }
    };
} // ndb

namespace ndb
{
////////////////////////////////////////////////////////////////////////////////
////////////////////////     SPE SCALAR EXPRESSION      ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    // init
    template<class Expr, expr_clause_code Clause>
    struct expression<Expr, expr_type_code::init, void, Clause>
    {
        static constexpr auto type = expr_type_code::init;

        Expr expr_;

        constexpr explicit expression(const Expr& e) : expr_(e) {}

        template<class F>
        constexpr void eval(F&& f) const
        {
            expr_.eval(f);
        }

        template<class F>
        static constexpr void static_eval(F&& f)
        {
            Expr::static_eval(f);
        }

        template<class Native_expression>
        constexpr void make(Native_expression& ne) const
        {
            native_expression<Native_expression::expr_category, expr_type_code::init>::template make<Clause>(expr_, ne);
        }

        template<int Pass = 0, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            native_expression<Native_expression::expr_category, expr_type_code::init>::template static_make<Expr, Clause>(ne);
        }

        constexpr static auto clause()
        {
            return Clause;
        }
    };

    // table
    template<class T, expr_clause_code Clause>
    struct expression<T, expr_type_code::table, void, Clause>
    {
        static constexpr auto type = expr_type_code::table;

        constexpr explicit expression(){}

        template<class F>
        constexpr void eval(F&& f) const
        {
            f(*this);
        }

        template<class F>
        static constexpr void static_eval(F&& f)
        {
            f(expression<T, expr_type_code::table, void, Clause> {});
        }

        template<class Native_expression>
        constexpr void make(Native_expression& ne) const
        {
            native_expression<Native_expression::expr_category, expr_type_code::table>::template make<Clause>(*this, ne);
        }

        template<int Pass = 0, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            native_expression<Native_expression::expr_category, expr_type_code::table>::template static_make<T, Clause>(ne);
        }

        constexpr static auto clause()
        {
            return Clause;
        }
    };

    // field
    template<class T, expr_clause_code Clause>
    struct expression<T, expr_type_code::field, void, Clause>
    {
        using value_type = T;
        static constexpr auto type = expr_type_code::field;

        constexpr explicit expression() {}

        template<class F>
        constexpr void eval(F&& f) const
        {
            f(*this);
        }

        template<class F>
        static constexpr void static_eval(F&& f)
        {
            f(expression<T, expr_type_code::field, void, Clause> {});
        }

        template<class Native_expression>
        constexpr void make(Native_expression& ne) const
        {
            native_expression<Native_expression::expr_category, expr_type_code::field>::template make<Clause>(*this, ne);
        }

        template<int Pass = 0, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            native_expression<Native_expression::expr_category, expr_type_code::field>::template static_make<T, Clause>(ne);
        }

        constexpr static auto clause()
        {
            return Clause;
        }
    };

    // value
    template<class T, expr_clause_code Clause>
    struct expression<T, expr_type_code::value, void, Clause>
    {
        static constexpr auto type = expr_type_code::value;

        const T& value_;

        constexpr explicit expression(const T& n) :
            value_(n)
        {}

        auto value() const
        {
            return value_;
        }

        template<class F>
        constexpr void eval(F&& f) const
        {
            f(*this);
        }

        template<class F>
        static constexpr void static_eval(F&& f)
        {
            f(expression<T, expr_type_code::value, void, Clause> { 0 });
        }

        template<class Native_expression>
        constexpr void make(Native_expression& ne) const
        {
            native_expression<Native_expression::expr_category, expr_type_code::value>::template make<Clause>(*this, ne);
        }

        template<int Pass = 0, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            native_expression<Native_expression::expr_category, expr_type_code::value>::template static_make<T, Clause>(ne);
        }

        constexpr static auto clause()
        {
            return expr_clause_code::none;
        }
    };
} // ndb

////////////////////////////////////////////////////////////////////////////////
////////////////////////            OPERATOR            ////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace ndb
{
    template<expr_clause_code Clause = expr_clause_code::none, class T>
    constexpr auto expr_make(const T& v)
    {
        if constexpr (ndb::is_expression<T>) return v;
        else if constexpr (ndb::is_field<T>)
        {
            if constexpr (Clause == expr_clause_code::none)
            {
                return ndb::expression<T, ndb::expr_type_code::field, void, ndb::expr_clause_code::get> {};
            }
            else return ndb::expression<T, ndb::expr_type_code::field, void, Clause> {};
        }
        else if constexpr (ndb::is_table<T>) return ndb::expression<T, ndb::expr_type_code::table, void, ndb::expr_clause_code::source> {};
        else return ndb::expression<T> { v };
    }
}

// op_list
template<class L, class R, class = ndb::enable_expression<L, R>>
constexpr const auto operator,(const L& l, const R& r)
{
    auto lhs = ndb::expr_make(l);
    auto rhs = ndb::expr_make(r);
    return ndb::expression<decltype(lhs), ndb::expr_type_code::op_list, decltype(rhs)> { lhs, rhs };
}

// op_equal
template<class L, class R, class = ndb::enable_expression<L, R>>
constexpr const auto operator==(const L& l, const R& r)
{
    auto lhs = ndb::expr_make<ndb::expr_clause_code::condition>(l);
    auto rhs = ndb::expr_make(r);
    return ndb::expression<decltype(lhs), ndb::expr_type_code::op_equal, decltype(rhs), ndb::expr_clause_code::condition> { lhs, rhs };
}

// op_and
template<class L, class R, class = ndb::enable_expression<L, R>>
constexpr const auto operator&&(const L& lhs, const R& rhs)
{
    return ndb::expression<L, ndb::expr_type_code::op_and, R> { lhs, rhs };
}

// op_or
template<class L, class R, class = ndb::enable_expression<L, R>>
constexpr const auto operator||(const L& lhs, const R& rhs)
{
    return ndb::expression<L, ndb::expr_type_code::op_or, R> { lhs, rhs };
}

// op_shif_left
template<class L, class R, class = ndb::enable_expression<L, R>>
constexpr const auto operator<<(const L& l, const R& r)
{
    auto lhs = ndb::expr_make(l);
    auto rhs = ndb::expr_make(r);
    return ndb::expression<decltype(lhs), ndb::expr_type_code::op_shift_left, decltype(rhs)> { lhs, rhs };
}

#endif // EXPRESSION_H_NDB