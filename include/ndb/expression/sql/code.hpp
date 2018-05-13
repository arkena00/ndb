#ifndef EXPRESSION_SQL_CODE_H_NDB
#define EXPRESSION_SQL_CODE_H_NDB

#include <ndb/expression/code.hpp>
#include <ndb/expression/function.hpp>

//TODO use only one struct

namespace ndb
{
    template<> struct expr_code<expr_type_code::op_and, expr_category_code::sql> { constexpr static auto value = " AND "; };
    template<> struct expr_code<expr_type_code::op_or, expr_category_code::sql> { constexpr static auto value = " OR "; };
    template<> struct expr_code<expr_type_code::op_equal, expr_category_code::sql> { constexpr static auto value = " = "; };
    template<> struct expr_code<expr_type_code::op_assign, expr_category_code::sql> { constexpr static auto value = ""; };
    template<> struct expr_code<expr_type_code::op_list, expr_category_code::sql> { constexpr static auto value = ", "; };
    template<> struct expr_code<expr_type_code::op_function, expr_category_code::sql> { constexpr static auto value = ""; };
    template<> struct expr_code<expr_type_code::keyword, expr_category_code::sql> { constexpr static auto value = ""; };

    // keywords
    template<> struct keyword_code<expr_keyword_code::get, expr_category_code::sql> { constexpr static auto value = "SELECT "; };
    template<> struct keyword_code<expr_keyword_code::set, expr_category_code::sql> { constexpr static auto value = "UPDATE "; };
    template<> struct keyword_code<expr_keyword_code::add, expr_category_code::sql> { constexpr static auto value = "INSERT INTO "; };
    template<> struct keyword_code<expr_keyword_code::del, expr_category_code::sql> { constexpr static auto value = "DELETE "; };

    template<> struct keyword_code<expr_keyword_code::source, expr_category_code::sql> { constexpr static auto value = " FROM "; };
    template<> struct keyword_code<expr_keyword_code::condition, expr_category_code::sql> { constexpr static auto value = " WHERE "; };
    template<> struct keyword_code<expr_keyword_code::limit, expr_category_code::sql> { constexpr static auto value = " LIMIT "; };
    template<> struct keyword_code<expr_keyword_code::group, expr_category_code::sql> { constexpr static auto value = " GROUP BY "; };

    template<> struct keyword_code<expr_keyword_code::values, expr_category_code::sql> { constexpr static auto value = " VALUES "; };

    template<> struct keyword_code<expr_keyword_code::now, expr_category_code::sql> { constexpr static auto value = " DATETIME"; };
    template<> struct keyword_code<expr_keyword_code::count, expr_category_code::sql> { constexpr static auto value = " COUNT"; };
} // ndb

#endif // EXPRESSION_SQL_CODE_H_NDB
