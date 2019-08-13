#ifndef EXPRESSION_BASIC_NDB
#define EXPRESSION_BASIC_NDB

#include <ndb/expression/form.hpp>
#include <ndb/expression.hpp>
#include <ndb/type.hpp>

namespace ndb
{
    template<class Type, class... Args>
    class expression;

    namespace expressions
    {
        enum class group { command, scalar };

        //! @param Return_type used by alias system to get the return type of an expression
        //! @param Expression_type used to create default operator()
        template<expression_forms Form = expression_forms::op_args, class Return_type = void>
        struct basic_expression
        {
            static constexpr auto form = Form;
            static constexpr bool is_scalar = false;
            using return_type = Return_type;
        };

        //! basic_expression with default operator()
        //! @param Expression_type used to create default operator()
        template<class Expression_type, expression_forms Form = expression_forms::op_args, class Return_type = void>
        struct basic_expression_op
        {
            static constexpr auto form = Form;
            static constexpr bool is_scalar = false;
            using return_type = Return_type;

            //! default operator when expr(...) is called
            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                auto expr = (ndb::expr_make(t), ...);
                return ndb::expression<Expression_type, decltype(expr)> { std::move(expr) };
            }
        };


        struct statement_ : basic_expression<expression_forms::none>{};

        struct substatement_ : basic_expression<expression_forms::brace>{};

        struct list_ : basic_expression<expression_forms::a_op_b>{};
        struct function_ : basic_expression<>{};

        // filter
        struct range_ : basic_expression<>{};
        struct assign_ : basic_expression<expression_forms::a_op_b>{};
        struct equal_ : basic_expression<expression_forms::a_op_b>{};

        // scalar
        struct all_ : basic_expression<>{};
        static constexpr ndb::expression<all_> all;

        struct table_all_ : basic_expression<expression_forms::none>{ static constexpr bool is_scalar = true; };

        struct field_ : basic_expression<>{ static constexpr bool is_scalar = true; };
        struct table_ : basic_expression<>{ static constexpr bool is_scalar = true; };
        struct value_ : basic_expression<>{ static constexpr bool is_scalar = true; };

        struct alias_ : basic_expression<>
        {
            static constexpr bool is_scalar = true;

            template<unsigned int Alias_id, class Expr>
            constexpr auto operator()(std::integral_constant<unsigned int, Alias_id>, const Expr& expr) const
            {
                using alias_type = std::integral_constant<unsigned int, Alias_id>;
                return ndb::expression<alias_, decltype(expr), alias_type> { std::move(expr), alias_type{} };
            }
        };
        static constexpr ndb::expression<alias_> as{};

        // command
        struct get_ : basic_expression<>
        {
            constexpr auto operator()() const
            {
                auto expr = ndb::expressions::all;
                return ndb::expression<get_, decltype(expr)> { std::move(expr) };
            }

            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                auto expr = (ndb::expr_make(t), ...);
                return ndb::expression<get_, decltype(expr)> { std::move(expr) };
            }
        };
        static constexpr ndb::expression<get_> get;

        struct set_ : basic_expression<>
        {
            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                auto expr = (ndb::expr_make(t), ...);
                return ndb::expression<set_, decltype(expr)> { std::move(expr) };
            }
        };
        static constexpr ndb::expression<set_> set;

        struct add_ : basic_expression<>
        {
            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                auto expr = (ndb::expr_make(t), ...);
                return ndb::expression<add_, decltype(expr)> { std::move(expr) };
            }
        };
        static constexpr ndb::expression<add_> add;

        struct add_ignore_ : basic_expression<>
        {
            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                auto expr = (ndb::expr_make(t), ...);
                return ndb::expression<add_ignore_, decltype(expr)> { std::move(expr) };
            }
        };
        static constexpr ndb::expression<add_ignore_> add_ignore;

        struct add_replace_ : basic_expression<>
        {
            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                auto expr = (ndb::expr_make(t), ...);
                return ndb::expression<add_replace_, decltype(expr)> { std::move(expr) };
            }
        };
        static constexpr ndb::expression<add_replace_> add_replace;

        struct del_ : basic_expression<>{};
        static constexpr ndb::expression<del_> del;

        struct clear_ : basic_expression<>
        {
            template<class... Ts>
            constexpr auto operator()(Ts&&... t) const
            {
                auto expr = (ndb::expr_make(t), ...);
                return ndb::expression<add_, decltype(expr)> { std::move(expr) };
            }
        };

        // clause
        struct source_ : basic_expression<>
        {
            template<class Ts>
            constexpr auto operator()(const Ts& ts) const
            {
                //auto expr = ((ndb::expr_make(ts), ...));
                auto expr = ndb::expression<ndb::expressions::table_, Ts>{ ts };
                return ndb::expression<source_, decltype(expr)> { std::move(expr) };
            }
        };
        static constexpr ndb::expression<source_> source;

        // relation
        struct edge_ : basic_expression<>
        {
            static constexpr bool is_scalar = true;

            template<class Source, class Target, class Connection>
            constexpr auto operator()(const Source& s, const Target& t, const Connection& c) const
            {
                return ndb::expression<edge_, decltype(s), decltype(t), decltype(c)> { s, t, c };
            }
        };
        static constexpr ndb::expression<edge_> edge;

        struct arc_ : basic_expression<>
        {
            static constexpr bool is_scalar = true;

            template<class Source, class Edge, class Target>
            constexpr auto operator()(const Source& s, const Edge& e, const Target& t) const
            {
                return ndb::expression<arc_, decltype(s), decltype(e), decltype(t)> { s, e, t };
            }
        };
        static constexpr ndb::expression<arc_> arc;

        struct filter_ : basic_expression<>
        {
            template<class T>
            constexpr auto operator()(const T& t) const
            {
                return ndb::expression<filter_, T> { t };
            }
        };
        static constexpr ndb::expression<filter_> filter;


        struct sort_ : basic_expression_op<sort_, expression_forms::op_args>{};
        static constexpr ndb::expression<sort_> sort;

        struct asc_ : basic_expression_op<asc_, expression_forms::arg_op>{};
        static constexpr ndb::expression<asc_> asc;
        struct desc_ : basic_expression_op<desc_, expression_forms::arg_op>{};
        static constexpr ndb::expression<desc_> desc;


        // operation
        struct logical_and_ : basic_expression<expression_forms::a_op_b>{};
        static constexpr ndb::expression<logical_and_> logical_and;

        struct logical_or_ : basic_expression<expression_forms::a_op_b>{};
        static constexpr ndb::expression<logical_or_> logical_or;

        // function
        // expression_type
        // count_ : function_expression<return_type, form = f>
        struct count_ : basic_expression<expression_forms::functional_args, int64_>
        {
            template<class T>
            constexpr auto operator()(const T& t) const
            {
                auto expr_field = ndb::expression<ndb::expressions::field_, T>{ t };
                return ndb::expression<count_, decltype(expr_field)> { expr_field };
            }
        };
        static constexpr ndb::expression<count_> count;

        // lower(movie.name) : function_expression<ndb::string_ // expr

    } // expression

    using namespace expressions;

    // instances



    // generic
    template<class Type, class Engine, expression_categories Expr_category>
    struct expression_type
    {
        // make all args
        template<class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            if constexpr (sizeof...(Args) == 0)
            {
                ne.append(expression_code<Type, Engine, Native_expression::category>::value);
            }
            else expression_form<Type::form, Engine, Native_expression::category>::template make<Type, Native_expression, Args...>(ne);
        }
    };
} // ndb

#endif // EXPRESSION_BASIC_NDB
