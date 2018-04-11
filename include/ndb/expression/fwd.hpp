#ifndef EXPRESSION_NATIVE_H_NDB
#define EXPRESSION_NATIVE_H_NDB

namespace ndb
{
    template<expr_category_code, expr_type_code>
    class native_expression;

    // default make for expression_type
    template<expr_type_code T, expr_category_code Ec>
    struct expression_type;
} // ndb

#endif // EXPRESSION_NATIVE_H_NDB
