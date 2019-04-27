#ifndef EXPRESSION_OPERATOR_H_NDB
#define EXPRESSION_OPERATOR_H_NDB

#include <ndb/expression/basic.hpp>
#include <ndb/expression/deduction.hpp>

namespace ndb
{
    template<class L, class R>
    using enable_expression = std::enable_if_t<(ndb::is_expression<L> || ndb::is_expression<R>
                                                || ndb::is_field<L> || ndb::is_field<R>
                                                || ndb::is_table<L> || ndb::is_table<R>)
                                                && !ndb::is_query<L>
    >;

    // op_list
    template<class L, class R, class = ndb::enable_expression<L, R>>
    constexpr const auto operator,(L&& l, R&& r)
    {
        auto expr_l = ndb::expr_make(std::forward<L>(l));
        auto expr_r = ndb::expr_make(std::forward<R>(r));
        return ndb::expression<ndb::expressions::list_, decltype(expr_l), decltype(expr_r)> { std::move(expr_l), std::move(expr_r) };
    }

    template<class L, class R, class = ndb::enable_expression<L, R>>
    constexpr const auto operator==(L&& l, R&& r)
    {
        auto expr_l = ndb::expr_make(std::forward<L>(l));
        auto expr_r = ndb::expr_make(std::forward<R>(r));
        return ndb::expression<ndb::expressions::equal_, decltype(expr_l), decltype(expr_r)> { std::move(expr_l), std::move(expr_r) };
    }

    template<class L, class R, class = ndb::enable_expression<L, R>>
    constexpr const auto operator&&(L&& l, R&& r)
    {
        auto expr_l = ndb::expr_make(std::forward<L>(l));
        auto expr_r = ndb::expr_make(std::forward<R>(r));
        return ndb::expression<ndb::expressions::logical_and_, decltype(expr_l), decltype(expr_r)> { std::move(expr_l), std::move(expr_r) };
    }

    template<class L, class R, class = ndb::enable_expression<L, R>>
    constexpr const auto operator||(L&& l, R&& r)
    {
        auto expr_l = ndb::expr_make(std::forward<L>(l));
        auto expr_r = ndb::expr_make(std::forward<R>(r));
        return ndb::expression<ndb::expressions::logical_or_, decltype(expr_l), decltype(expr_r)> { std::move(expr_l), std::move(expr_r) };
    }

    template<class... Ls, class R>
    constexpr const auto operator<<(ndb::expression<Ls...> l, R&& r)
    {
        auto expr_l = ndb::expr_make(std::move(l));
        auto expr_r = ndb::expr_make(std::forward<R>(r));

        return ndb::expression<ndb::statement_, decltype(expr_l), decltype(expr_r)> { std::move(expr_l), std::move(expr_r) };
    }

    // add clause to statement
    template<class... Ls, class R>
    constexpr const auto operator<<(const ndb::expression<ndb::statement_, Ls...>& l, R&& r)
    {
        // msvc fix
        if constexpr (sizeof...(Ls) == 0)
        {
            auto expr_r = ndb::expr_make(std::forward<R>(r));
            return ndb::expression<ndb::statement_, decltype(expr_r)> { (std::move(expr_r)) };
        }
        // normal compiler
        else
        {
            auto expr_r = ndb::expr_make(std::forward<R>(r));
            return ndb::expression<ndb::statement_, Ls..., decltype(expr_r)> { std::tuple_cat(l.args(), std::forward_as_tuple(std::move(expr_r))) };
        }
    }

} // ndb

//expression_construct<statement, T>


#endif // EXPRESSION_OPERATOR_H_NDB
