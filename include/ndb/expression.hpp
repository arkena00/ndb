#ifndef EXPRESSION_H_NDB
#define EXPRESSION_H_NDB

//! .tpp include operators to overload them

namespace ndb
{
    template<class Type>
    struct expression_type;

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

        template<class... Ts>
        constexpr auto operator()(const Ts&... ts) const
        {
            return Type{}(ts...);
        }

        template<class Native_expression>
        static constexpr auto make(Native_expression& ne)
        {
            expression_type<Type>::template make<Native_expression, Args...>(ne);
        }
    };

    namespace expressions
    {
        static constexpr ndb::expression<filter_> filter;
    }

} // ndb

#include <ndb/expression.tpp>

#endif // EXPRESSION_H_NDB