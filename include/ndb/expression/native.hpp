#ifndef EXPRESSION_NATIVE_H_NDB
#define EXPRESSION_NATIVE_H_NDB

#include <ndb/expression/category.hpp>

namespace ndb
{
    template<class Expr, class Engine, expression_categories Expr_category = Engine::expr_category>
    class native_expression;
} // ndb

#endif // EXPRESSION_NATIVE_H_NDB
