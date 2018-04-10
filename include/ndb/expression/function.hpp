#ifndef EXPRESSION_FUNCTION_H_NDB
#define EXPRESSION_FUNCTION_H_NDB

#include <ndb/expression.hpp>
#include <ndb/expression/code.hpp>

namespace ndb
{
    namespace expr_function
    {
        struct count;
        struct now;
    } // expr_function


    template<class T>
    constexpr auto count(const T& t)
    {
        auto func = ndb::expression<expr_function::count, expr_type_code::function>();
        auto expr = ndb::expr_make(t);
        return ndb::expression<decltype(func), expr_type_code::op_function, decltype(expr)> { func, expr };
    }

    constexpr auto now()
    {
        auto func = ndb::expression<expr_function::now, expr_type_code::function>();
        auto expr = ndb::expression<>{};
        return ndb::expression<decltype(func), expr_type_code::op_function, decltype(expr)> { func, expr };
    }

    /*
    constexpr auto limit(int a, int b)
    {
        auto func = ndb::expression<void, expr_type_code::keyword>{};
        auto expr = ndb::expression<int>{ 3 };
        //auto expr = ndb::expr_make(t);
        return ndb::expression<decltype(func), expr_type_code::op_function, decltype(expr)> { func, expr };
    }*/
} // ndb

#endif // EXPRESSION_FUNCTION_H_NDB
