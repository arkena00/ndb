#ifndef EXPRESSION_SCALAR_H_NDB
#define EXPRESSION_SCALAR_H_NDB

#include <ndb/expression/code.hpp>

namespace ndb
{
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

    // function
    template<class Function, expr_clause_code Clause>
    struct expression<Function, expr_type_code::function, void, Clause>
    {
        static constexpr auto type = expr_type_code::value;

        constexpr explicit expression()
        {}


        template<class F>
        constexpr void eval(F&& f) const
        {
            //f(*this);
        }

        template<class F>
        static constexpr void static_eval(F&& f)
        {
            //f(expression<Function, expr_type_code::function, void, Clause> { 0 });
        }

        template<class Native_expression>
        constexpr void make(Native_expression& ne) const
        {
            native_expression<Native_expression::expr_category, expr_type_code::function>::template make<Clause>(*this, ne);
        }

        template<int Pass = 0, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            native_expression<Native_expression::expr_category, expr_type_code::function>::template static_make<Function, Clause>(ne);
        }

        constexpr static auto clause()
        {
            return expr_clause_code::none;
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

    // value void
    template<expr_clause_code Clause>
    struct expression<void, expr_type_code::value, void, Clause>
    {
        static constexpr auto type = expr_type_code::value;

        constexpr explicit expression()
        {}

        auto value() const
        {
            return 0;
        }

        template<class F>
        constexpr void eval(F&& f) const
        {
            f(*this);
        }

        template<class F>
        static constexpr void static_eval(F&& f)
        {
            f(expression<void, expr_type_code::value, void, Clause>{});
        }

        template<class Native_expression>
        constexpr void make(Native_expression& ne) const
        {
            native_expression<Native_expression::expr_category, expr_type_code::value>::template make<Clause>(*this, ne);
        }

        template<int Pass = 0, class Native_expression>
        static constexpr void static_make(Native_expression& ne)
        {
            native_expression<Native_expression::expr_category, expr_type_code::value>::template static_make<void, Clause>(ne);
        }

        constexpr static auto clause()
        {
            return expr_clause_code::none;
        }
    };
} // ndb

#endif // EXPRESSION_SCALAR_H_NDB
