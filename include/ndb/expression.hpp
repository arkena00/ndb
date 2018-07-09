#ifndef EXPRESSION_H_NDB
#define EXPRESSION_H_NDB

//! .tpp include operators to overload them

#include <tuple>
#include <ndb/utility.hpp>
#include <iostream>
#include "utility.hpp"

namespace ndb
{
    template<class Type, class Engine, expression_categories Expr_category = Engine::expr_category()>
    struct expression_type;

    struct expression_base {};

    template<class Type, class... Args>
    struct expression : expression_base
    {
        using type = Type;

        std::tuple<Args...> args_;

        const std::tuple<Args...>& args() const { return args_; }

        inline constexpr expression(Args&&... args) : args_(std::forward<Args>(args)...) {}
        inline constexpr expression(std::tuple<Args...> args) : args_(std::move(args)) {}

        template<class... Ts>
        constexpr auto operator()(const Ts&... ts) const
        {
            return Type{}(ts...);
        }

        constexpr auto arg_count() const { return sizeof...(Args); }

        template<class F>
        constexpr void eval(F&& f) const
        {
            if constexpr( ndb::expr_is_scalar<decltype(*this)> ) f(*this);
            else
            {
                std::apply([&f](auto&&... arg)
                           {
                               (arg.eval(f), ...);
                           }, args_);
            }
        }

        template<class Engine, class Native_expression>
        static constexpr auto make(Native_expression& ne)
        {
            expression_type<Type, Engine, Engine::expr_category()>::template make<Native_expression, Args...>(ne);
        }
    };

    template<class T>
    constexpr auto expr_make(T&& v)
    {
        if constexpr (ndb::is_expression<T>) return v;
        else if constexpr (ndb::is_field<T>)
        {
            return ndb::expression<ndb::expressions::field_, T>{ v };
        }
        else if constexpr (ndb::is_table<T>) return ndb::expression<ndb::expressions::table_, T> {};
            // return value expression
        else return ndb::expression<expressions::value_, T> { std::forward<T>(v) };
    }
} // ndb

#include <ndb/expression.tpp>

#endif // EXPRESSION_H_NDB