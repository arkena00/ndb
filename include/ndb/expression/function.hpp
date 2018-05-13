#ifndef EXPRESSION_FUNCTION_H_NDB
#define EXPRESSION_FUNCTION_H_NDB

#include <ndb/expression.hpp>
#include <ndb/expression/code.hpp>
#include <ndb/expression/utility.hpp>
#include <ndb/expression/operator.hpp>

namespace ndb
{
    namespace expr_function
    {
        struct count;
        struct now;
    } // expr_function


    // keyword
    template<class... Ts>
    constexpr auto get(const Ts&... t)
    {
        auto keyword = ndb::expr_make_keyword<expr_keyword_code::get>();

        if constexpr (sizeof...(Ts) == 0)
        {
            auto expr = ndb::expr_make_keyword<expr_keyword_code::all>();
            return ndb::expression<decltype(keyword), expr_type_code::keyword, decltype(expr), expr_clause_code::get> { keyword, expr };
        }
        else
        {
            // msvc fix : use (( )) to compile
            auto expr = ((ndb::expr_make(t), ...));
            return ndb::expression<decltype(keyword), expr_type_code::keyword, decltype(expr), expr_clause_code::get> { keyword, expr };
        }
    }

    template<class... Ts>
    constexpr auto source(const Ts&... t)
    {
        auto keyword = ndb::expr_make_keyword<expr_keyword_code::source>();
        auto expr = (ndb::expr_make(t), ...);
        return ndb::expression<decltype(keyword), expr_type_code::keyword, decltype(expr), expr_clause_code::source> { keyword, expr };
    }

    template<class T>
    constexpr auto filter(const T& t)
    {
        auto keyword = ndb::expr_make_keyword<expr_keyword_code::condition>();
        auto expr = ndb::expr_make(t);
        return ndb::expression<decltype(keyword), expr_type_code::keyword, decltype(expr), expr_clause_code::condition> { keyword, expr };
    }

    constexpr auto limit(int count, int offset = 0)
    {
        auto keyword = ndb::expr_make_keyword<expr_keyword_code::limit>();
        auto expr = ndb::expression<int, expr_type_code::value>{ count };
        return ndb::expression<decltype(keyword), expr_type_code::keyword, decltype(expr)> { keyword, expr };
    }

    // function
    template<class T>
    constexpr auto count(const T& t)
    {
        auto keyword = ndb::expr_make_keyword<expr_keyword_code::count>();
        auto expr = ndb::expr_make(t);
        return ndb::expression<decltype(keyword), expr_type_code::op_function, decltype(expr)> { keyword, expr };
    }

    constexpr auto now()
    {
        auto keyword = ndb::expr_make_keyword<expr_keyword_code::now>();
        auto expr = ndb::expr_make();
        return ndb::expression<decltype(keyword), expr_type_code::op_function, decltype(expr)> { keyword, expr };
    }
} // ndb

#endif // EXPRESSION_FUNCTION_H_NDB