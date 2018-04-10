#ifndef EXPRESSION_CODE_H_NDB
#define EXPRESSION_CODE_H_NDB

#include <ndb/setup.hpp>

namespace ndb
{
    enum class expr_type_code
    {
        value,
        field,
        table,
        function,
        init,
        op_and,
        op_or,
        op_equal,
        op_assign,
        op_shift_left,
        op_list,
        op_function
    };

    enum class expr_clause_code
    {
        none       = 0,
        get        = 1,
        set        = 2,
        add        = 4,
        del        = 8,
        source     = 16,
        condition  = 32,
        join       = 64,
        value_list = 128,
        limit      = 256
    };

    enum class expr_category_code
    {
        sql = 0,
        bson
    };

    constexpr expr_clause_code operator|(expr_clause_code lhs, expr_clause_code rhs)
    {
        return static_cast<expr_clause_code>(static_cast<char>(lhs) | static_cast<char>(rhs));
    }

    constexpr expr_clause_code operator&(expr_clause_code lhs, expr_clause_code rhs)
    {
        return static_cast<expr_clause_code>(static_cast<char>(lhs) & static_cast<char>(rhs));
    }

    template<expr_type_code, expr_category_code>
    struct expr_code
    {
        static constexpr const char* value = " _undefined_ ";
    };

    template<expr_clause_code, expr_category_code>
    struct clause_code
    {
        static constexpr const char* value = " _undefined_ ";
    };

    template<class Function, expr_category_code>
    struct function_code
    {
        static constexpr const char* value = " _undefined_ ";
    };
} // ndb

#endif // EXPRESSION_CODE_H_NDB
