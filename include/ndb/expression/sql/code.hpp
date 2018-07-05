#ifndef EXPRESSION_SQL_CODE_NDB
#define EXPRESSION_SQL_CODE_NDB

#include <ndb/expression/category.hpp>
#include <ndb/expression/type.hpp>
#include <ndb/expression/code.hpp>

namespace ndb
{

    template<> struct expression_code<expressions::get, expression_categories::sql> { static constexpr const char* value = "SELECT "; };
    template<> struct expression_code<expressions::assign, expression_categories::sql> { static constexpr const char* value = "="; };
    template<> struct expression_code<expressions::equal, expression_categories::sql> { static constexpr const char* value = "="; };
    template<> struct expression_code<expressions::logical_and, expression_categories::sql> { static constexpr const char* value = " AND "; };
    template<> struct expression_code<expressions::list, expression_categories::sql> { static constexpr const char* value = ","; };
    template<> struct expression_code<expressions::del_, expression_categories::sql> { static constexpr const char* value = " DELETE "; };
    template<> struct expression_code<expressions::add, expression_categories::sql> { static constexpr const char* value = " INSERT INTO "; };
    template<> struct expression_code<expressions::source_, expression_categories::sql> { static constexpr const char* value = " FROM "; };
    template<> struct expression_code<expressions::filter_, expression_categories::sql> { static constexpr const char* value = " WHERE "; };
    template<> struct expression_code<expressions::range, expression_categories::sql> { static constexpr const char* value = " BETWEEN "; };

    template<> struct expression_code<expressions::statement, expression_categories::sql> { static constexpr const char* value = "[root_]"; };
    template<> struct expression_code<expressions::substatement, expression_categories::sql> { static constexpr const char* value = "[]"; };
    template<> struct expression_code<expressions::value, expression_categories::sql> { static constexpr const char* value = "$"; };
} // ndb

#endif // EXPRESSION_SQL_CODE_NDB
