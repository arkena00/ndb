#include <ndb/utility.hpp>

namespace ndb
{
    template<class Type, class... Args>
    constexpr expression<Type, Args...>::expression(Args&&... args) :
        args_( std::forward<Args>(args)... )
    {}

    template<class Type, class... Args>
    constexpr expression<Type, Args...>::expression(std::tuple<Args...> args) :
        args_( std::move(args) )
    {}

    template<class Type, class... Args>
    template<class... Ts>
    constexpr auto expression<Type, Args...>::operator()(const Ts&... ts) const
    {
        return Type{}(ts...);
    }

    template<class Type, class... Args>
    constexpr const std::tuple<Args...>& expression<Type, Args...>::args() const
    {
        return args_;
    }

    template<class Type, class... Args>
    constexpr auto expression<Type, Args...>::arg_count() const
    {
        return sizeof...(Args);
    }

    template<class Type, class... Args>
    template<class F>
    constexpr void expression<Type, Args...>::eval(F&& f) const
    {
        if constexpr( Type::is_scalar ) f(*this);
        else
        {
            std::apply([&f](auto&&... arg)
                       {
                           (arg.eval(f), ...);
                       }, args_);
        }
    }

    template<class Type, class... Args>
    template<class Engine, class Native_expression>
    constexpr void expression<Type, Args...>::make(Native_expression& ne)
    {
        expression_type<Type, Engine, Engine::expr_category>::template make<Native_expression, Args...>(ne);
    }

    template<class T>
    constexpr auto expr_make(T&& v)
    {
        if constexpr (ndb::is_expression<T>) return std::move(v);
        else if constexpr (ndb::is_field<T>)
        {
            return ndb::expression<ndb::expressions::field_, T>{ std::forward<T>(v) };
        }
        else if constexpr (ndb::is_table<T>) return ndb::expression<ndb::expressions::table_, T> {};
            // return value expression
        else return ndb::expression<expressions::value_, T> { std::forward<T>(v) };
    }
} // ndb