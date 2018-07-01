#ifndef EXPRESSION_FUNCTION_H_NDB
#define EXPRESSION_FUNCTION_H_NDB

#include <ndb/expression.hpp>
#include <ndb/expression/code.hpp>
#include <ndb/expression/utility.hpp>
#include <ndb/expression/operator.hpp>

namespace ndb
{
    template<class... Ts>
    constexpr auto get(const Ts&... ts)
    {
        auto expr = ((ndb::expr_make(ts), ...));
        return ndb::expression<ndb::expression_types::get, decltype(expr)> { expr };
    }

    template<class... Ts>
    constexpr auto source(const Ts&... ts)
    {
        auto expr = ((ndb::expr_make(ts), ...));
        return ndb::expression<ndb::expression_types::source, decltype(expr)> { expr };
    }

    template<class T>
    constexpr auto filter(const T& t)
    {
        return ndb::expression<ndb::expression_types::filter, decltype(t)> { t };
    }

    template<class T, class N1, class N2>
    constexpr auto range(const T& t, const N1& a, const N2& b)
    {
        return ndb::expression<expression_types::range, T, N1, N2> { t, a, b };
    }
} // ndb

#endif // EXPRESSION_FUNCTION_H_NDB