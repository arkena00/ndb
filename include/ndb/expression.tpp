/*! expression<Left_expression, expression_type ( [operators, field, value] ), Right_expression>
 * expression::type : [operator, field, value, function]
 * expression::clause : get, condition
 *
 * Void expression :
 * expression<>
 *
 * Scalar types :
 * expression<Encapsuled_type, Scalar_type>
 *
 * Process of static_make to generate a Native_expression:
 * Given expression<L, Type, R>
 * call expression<L, Type, R>::static_make
 * call expression_type<Type, Native_expression::expr_category>::static_make(lhs_, rhs_, ne);
 * Default for all Type of expression :
 *          L::template static_make<Pass>(ne);
 *          ne.push_back(expr_code<T, expr_category_code::sql>::value);
 *          R::template static_make<Pass>(ne);
 *
 * static_make can be specialized for Type
 * recursion stop on scalar expressions (specialized expressions by expression_type) ex : expression<T, spe, ...>
 *
 * scalar expressions call :
 * native_expression<Native_expression::expr_category, Type>::static_make<Expr, Clause>(ne);
 *
 * to append the expression to the Native_expression object
 */

#include <ndb/expression/scalar.hpp>
#include <ndb/expression/operator.hpp>
#include <ndb/expression/type.hpp>

namespace ndb
{

} // ndb