#ifndef EXPRESSION_TYPE_NDB
#define EXPRESSION_TYPE_NDB

#include <ndb/expression/form.hpp>
#include <ndb/expression/utility.hpp>
#include <ndb/expression.hpp>

namespace ndb
{
    template<class Type, class... Args>
    struct expression;


    namespace expressions
    {
        enum class group { command, scalar };

        template<expression_forms Form = expression_forms::op_args>
        struct basic_type
        {
            static constexpr auto form = Form;
        };

        struct statement_ : basic_type<expression_forms::none>{};
        struct substatement_ : basic_type<expression_forms::brace>{};

        struct list_ : basic_type<expression_forms::a_op_b>{};
        struct function_ : basic_type<>{};

        // filter
        struct range_ : basic_type<>{};
        struct assign_ : basic_type<expression_forms::a_op_b>{};
        struct equal_ : basic_type<expression_forms::a_op_b>{};

        // scalar
        struct field_ : basic_type<>{};
        struct table_ : basic_type<>{};
        struct value_ : basic_type<>{};

        // command
        struct get_ : basic_type<>
        {
            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                // msvc fix : use (( )) to compile
                auto expr = ((ndb::expr_make(t), ...));
                return ndb::expression<get_, decltype(expr)> { std::move(expr) };
            }
        };
        static constexpr ndb::expression<get_> get;

        struct set : basic_type<>{};

        struct add_ : basic_type<>
        {
            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                // msvc fix : use (( )) to compile
                auto expr = ((ndb::expr_make(t), ...));
                return ndb::expression<add_, decltype(expr)> { std::move(expr) };
            }
        };
        static constexpr ndb::expression<add_> add;

        static constexpr struct del_ : basic_type<>{} del;

        // clause
        struct source_ : basic_type<>
        {
            template<class Ts>
            constexpr auto operator()(const Ts& ts) const
            {
                //auto expr = ((ndb::expr_make(ts), ...));
                auto expr = ndb::expression<ndb::expressions::table_, Ts>{ ts };
                return ndb::expression<source_, decltype(expr)> { expr };
            }
        };
        static constexpr ndb::expression<source_> source;

        struct filter_ : basic_type<>
        {
            template<class T>
            constexpr auto operator()(const T& t) const
            {
                return ndb::expression<filter_, T> { t };
            }
        };
        static constexpr ndb::expression<filter_> filter;


        // operation
        struct logical_and_ : basic_type<expression_forms::a_op_b>{};
    }



    // generic
    template<class Type, class Engine, expression_categories Expr_category>
    struct expression_type
    {
        // make all args
        template<class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            expression_form<Type::form, Engine, Native_expression::category>::template make<Type, Native_expression, Args...>(ne);
        }

        template<class F, class... Args>
        static constexpr void eval(F&& f, const std::tuple<Args...>& args)
        {
            std::apply([&](auto&& arg)
                       {
                           //arg.eval(std::forward<F>(f), arg.args);
                       }, args);
        }
    };


} // ndb


#endif // EXPRESSION_TYPE_NDB
