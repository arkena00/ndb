#ifndef EXPRESSION_H_NDB
#define EXPRESSION_H_NDB

#include <ndb/expression/type.hpp>

//! .tpp include operators to overload them

namespace ndb
{
    struct expression_base {};

    template<class Type, class... Args>
    struct expression : expression_base
    {
        using type = Type;

        std::tuple<Args...> args;

        //template<class... Z>
        //constexpr expression(Z... args) {}

        inline constexpr expression(Args... args_) : args(args_...) {}
        inline constexpr expression(std::tuple<Args...> args_) : args(args_) {}


        template<class Native_expression>
        static constexpr auto make(Native_expression& ne)
        {
            expression_type<Type>::template make<Native_expression, Args...>(ne);
        }

    };

    template<class Type, class... Args>
    struct scalar_expression : expression_base
    {
        using type = Type;

        template<class Native_expression>
        static constexpr auto make(Native_expression& ne)
        {
            ne += "S";
        }
    };


    template<class T>
    struct expression<expression_types::field, T> : scalar_expression<expression_types::field, T>
    {
        std::tuple<T> args;

        constexpr expression() {}

        template<class Native_expression>
        static constexpr auto make(Native_expression& ne)
        {
            ne += "F";
        }

    };

    template<class T>
    struct expression<expression_types::table, T> : scalar_expression<expression_types::table, T>
    {
        std::tuple<T> args;

        constexpr expression() {}

        template<class Native_expression>
        static constexpr auto make(Native_expression& ne)
        {
            ne += "T";
        }

    };

} // ndb

#include <ndb/expression.tpp>

#endif // EXPRESSION_H_NDB