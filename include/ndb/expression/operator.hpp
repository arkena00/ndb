#ifndef EXPRESSION_OPERATOR_H_NDB
#define EXPRESSION_OPERATOR_H_NDB

#include <ndb/expression/type.hpp>
#include <ndb/expression/utility.hpp>
#include <ndb/expression/deduce.hpp>

namespace ndb
{
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
    constexpr const auto operator<<(const L& l, const R& r)
    {
        auto expr_l = ndb::expr_make(l);
        auto expr_r = ndb::expr_make(r);

        return ndb::expression<expressions::statement_, decltype(expr_l), decltype(expr_r)> { expr_l, expr_r };
    }

    // add clause to statement
    template<class... Ls, class R>
    constexpr const auto operator<<(const ndb::expression<ndb::expressions::statement_, Ls...>& l, R&& r)
    {
        auto expr_r = ndb::expr_make(std::forward<R>(r));
        /*
        auto expr_deduced = ndb::deduction<decltype(l), decltype(expr_r)>::process(l, expr_r);
        return expr_deduced;*/
        return ndb::expression<expressions::statement_, Ls..., R> { std::tuple_cat(l.args(), expr_r.args()) };
    }

} // ndb

//expression_construct<statement, T>


#endif // EXPRESSION_OPERATOR_H_NDB
