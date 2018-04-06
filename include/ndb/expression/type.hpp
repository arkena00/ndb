#ifndef EXPRESSION_TYPE_H_NDB
#define EXPRESSION_TYPE_H_NDB

#include <ndb/expression/code.hpp>

namespace ndb
{
    // default make for expression_type
    template<expr_type_code T, expr_category_code Ec>
    struct expression_type;
} // ndb

#endif // EXPRESSION_TYPE_H_NDB