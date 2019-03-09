#ifndef EXPRESSION_SQL_CODE_NDB
#define EXPRESSION_SQL_CODE_NDB

#include <ndb/expression/category.hpp>
#include <ndb/expression/type.hpp>
#include <ndb/expression/code.hpp>

namespace ndb
{
    template<class Engine> struct expression_code<statement_, Engine, expression_categories::sql> { static constexpr const char* value = "[root_]"; };

    template<class Engine> struct expression_code<expressions::get_, Engine, expression_categories::sql> { static constexpr const char* value = "SELECT "; };
    template<class Engine> struct expression_code<expressions::assign_, Engine, expression_categories::sql> { static constexpr const char* value = "="; };
    template<class Engine> struct expression_code<expressions::equal_, Engine, expression_categories::sql> { static constexpr const char* value = "="; };
    template<class Engine> struct expression_code<expressions::logical_and_, Engine, expression_categories::sql> { static constexpr const char* value = " AND "; };
    template<class Engine> struct expression_code<expressions::logical_or_, Engine, expression_categories::sql> { static constexpr const char* value = " OR "; };
    template<class Engine> struct expression_code<expressions::list_, Engine, expression_categories::sql> { static constexpr const char* value = ","; };
    template<class Engine> struct expression_code<expressions::del_, Engine, expression_categories::sql> { static constexpr const char* value = "DELETE "; };
    template<class Engine> struct expression_code<expressions::add_, Engine, expression_categories::sql> { static constexpr const char* value = "INSERT INTO "; };
    template<class Engine> struct expression_code<expressions::add_ignore_, Engine, expression_categories::sql> { static constexpr const char* value = "INSERT OR IGNORE INTO "; };
    template<class Engine> struct expression_code<expressions::source_, Engine, expression_categories::sql> { static constexpr const char* value = " FROM "; };
    template<class Engine> struct expression_code<expressions::filter_, Engine, expression_categories::sql> { static constexpr const char* value = " WHERE "; };
    template<class Engine> struct expression_code<expressions::range_, Engine, expression_categories::sql> { static constexpr const char* value = " BETWEEN "; };

    template<class Engine> struct expression_code<expressions::alias_, Engine, expression_categories::sql> { static constexpr const char* value = " AS "; };

    template<class Engine> struct expression_code<expressions::count_, Engine, expression_categories::sql> { static constexpr const char* value = " COUNT"; };

    template<class Engine> struct expression_code<expressions::substatement_, Engine, expression_categories::sql> { static constexpr const char* value = "[]"; };
    template<class Engine> struct expression_code<expressions::value_, Engine, expression_categories::sql> { static constexpr const char* value = "$"; };
    template<class Engine> struct expression_code<expressions::all_, Engine, expression_categories::sql> { static constexpr const char* value = "*"; };
} // ndb

#endif // EXPRESSION_SQL_CODE_NDB
