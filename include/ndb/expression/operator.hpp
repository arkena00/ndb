#ifndef EXPRESSION_OPERATOR_H_NDB
#define EXPRESSION_OPERATOR_H_NDB

#include <ndb/expression/type.hpp>
#include <ndb/expression/utility.hpp>

namespace ndb
{
    // op_list
    template<class L, class R, class = ndb::enable_expression<L, R>>
    constexpr const auto operator,(const L& l, const R& r)
    {
        auto lhs = ndb::expr_make(l);
        auto rhs = ndb::expr_make(r);
        return ndb::expression<ndb::expressions::list, decltype(lhs), decltype(rhs)> { lhs, rhs };
    }

    template<class L, class R, class = ndb::enable_expression<L, R>>
    constexpr const auto operator==(const L& l, const R& r)
    {
        auto lhs = ndb::expr_make(l);
        auto rhs = ndb::expr_make(r);
        return ndb::expression<ndb::expressions::equal, decltype(lhs), decltype(rhs)> { lhs, rhs };
    }

    template<class L, class R, class = ndb::enable_expression<L, R>>
    constexpr const auto operator&&(const L& l, const R& r)
    {
        auto lhs = ndb::expr_make(l);
        auto rhs = ndb::expr_make(r);
        return ndb::expression<ndb::expressions::logical_and, decltype(lhs), decltype(rhs)> { lhs, rhs };
    }

    template<class L, class R, class = ndb::enable_expression<L, R>>
    constexpr const auto operator<<(const L& l, const R& r)
    {
        auto lhs = ndb::expr_make(l);
        auto rhs = ndb::expr_make(r);

        return ndb::expression<expressions::statement, decltype(lhs), decltype(rhs)> { lhs, rhs };
    }

    // add clause to statement
    template<class... Ls, class R>
    constexpr const auto operator<<(const ndb::expression<ndb::expressions::statement, Ls...>& l, const R& r)
    {
        auto rhs = ndb::expr_make(r);
        return ndb::expression<expressions::statement, Ls..., R> { std::tuple_cat(l.args, rhs.args) };
    }

} // ndb

//expression_construct<statement, T>


#endif // EXPRESSION_OPERATOR_H_NDB
