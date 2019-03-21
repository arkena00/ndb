#ifndef EXPRESSION_FORM_NDB
#define EXPRESSION_FORM_NDB

//! \brief generic forms

#include <ndb/expression/category.hpp>
#include <ndb/expression/code.hpp>

#include <type_traits>

namespace ndb
{
    enum class expression_forms { none, scalar, brace, functional_args, op_args, a_op_b, arg_op };

    template<expression_forms Form, class Engine, expression_categories Category = Engine::expr_category>
    struct expression_form;

    template<class Engine, expression_categories Category>
    struct expression_form<expression_forms::none, Engine, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            (std::decay_t<Args>::template make<Engine>(ne), ...);
        }
    };

    template<class Engine, expression_categories Category>
    struct expression_form<expression_forms::scalar, Engine, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(expression_code<Type, Engine, Category>::value);
        }
    };

    template<class Engine, expression_categories Category>
    struct expression_form<expression_forms::brace, Engine, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            ne.append('(');
            (std::decay_t<Args>::template make<Engine>(ne), ...);
            ne.append(')');
        }
    };

    template<class Engine, expression_categories Category>
    struct expression_form<expression_forms::op_args, Engine, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(expression_code<Type, Engine, Category>::value);
            (std::decay_t<Args>::template make<Engine>(ne), ...);
        }
    };

    template<class Engine, expression_categories Category>
    struct expression_form<expression_forms::arg_op, Engine, Category>
    {
        template<class Type, class Native_expression, class T1>
        static constexpr void make(Native_expression& ne)
        {
            std::decay_t<T1>::template make<Engine>(ne);
            ne.append(expression_code<Type, Engine, Category>::value);
        }
    };

    template<class Engine, expression_categories Category>
    struct expression_form<expression_forms::a_op_b, Engine, Category>
    {
        template<class Type, class Native_expression, class T1, class T2>
        static constexpr void make(Native_expression& ne)
        {
            std::decay_t<T1>::template make<Engine>(ne);
            ne.append(expression_code<Type, Engine, Category>::value);
            std::decay_t<T2>::template make<Engine>(ne);
        }
    };

    template<class Engine, expression_categories Category>
    struct expression_form<expression_forms::functional_args, Engine, Category>
    {
        template<class Type, class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(expression_code<Type, Engine, Category>::value);
            ne.append('(');
            (std::decay_t<Args>::template make<Engine>(ne), ...);
            ne.append(')');
        }
    };
} // ndb

#endif // EXPRESSION_FORM_NDB
