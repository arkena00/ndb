#ifndef EXPRESSION_FORM_NDB
#define EXPRESSION_FORM_NDB

//! \brief generic forms

#include <ndb/expression/category.hpp>
#include <ndb/expression/code.hpp>

#include <type_traits>

namespace ndb
{
    enum class expression_forms { none, scalar, brace, op_args, a_op_b, op_a_b };

    template<expression_forms Form, expression_categories Category>
    struct expression_form;

    template<expression_categories Category>
    struct expression_form<expression_forms::none, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            (std::decay_t<Args>::make(ne), ...);
        }
    };

    template<expression_categories Category>
    struct expression_form<expression_forms::scalar, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(expression_code<Type, Category>::value);
        }
    };

    template<expression_categories Category>
    struct expression_form<expression_forms::brace, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            ne.append('(');
            (std::decay_t<Args>::make(ne), ...);
            ne.append(')');
        }
    };

    template<expression_categories Category>
    struct expression_form<expression_forms::op_args, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(expression_code<Type, Category>::value);
            (std::decay_t<Args>::make(ne), ...);
        }
    };
    template<expression_categories Category>
    struct expression_form<expression_forms::a_op_b, Category>
    {
        template<class Type, class Native_expression, class T1, class T2>
        static constexpr void make(Native_expression& ne)
        {
            T1::make(ne);
            ne.append(expression_code<Type, Category>::value);
            T2::make(ne);
        }
    };

    template<expression_categories Category>
    struct expression_form<expression_forms::op_a_b, Category>
    {
        template<class Type, class Native_expression, class T1, class T2>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(expression_code<Type, Category>::value);
            T1::make(ne);
            T2::make(ne);
        }
    };
} // ndb

#endif // EXPRESSION_FORM_NDB
