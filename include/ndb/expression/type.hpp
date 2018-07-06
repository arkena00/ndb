#ifndef EXPRESSION_TYPE_NDB
#define EXPRESSION_TYPE_NDB

#include <ndb/expression/form.hpp>

namespace ndb
{
    template<class Type, class... Args>
    struct expression;

    namespace expressions
    {
        template<expression_forms Form = expression_forms::op_args>
        struct basic_type
        {
            static constexpr auto form = Form;
        };

        struct statement : basic_type<expression_forms::none>{};
        struct substatement : basic_type<expression_forms::brace>{};

        struct list : basic_type<expression_forms::a_op_b>{};
        struct function : basic_type<>{};

        // filter
        struct range : basic_type<>{};
        struct assign : basic_type<>{};
        struct equal : basic_type<expression_forms::a_op_b>{};

        // scalar
        struct command : basic_type<>{};
        struct scalar : basic_type<>{};
        struct field : basic_type<>{};
        struct table_ : basic_type<>{};
        struct value : basic_type<>{};

        // clause
        struct get : basic_type<>{};
        struct set : basic_type<>{};
        struct add : basic_type<>{};
        static constexpr struct del_ : basic_type<>{} del;

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
        //static constexpr ndb::expression<source_> source;

        struct filter_ : basic_type<>
        {
            template<class T>
            constexpr auto operator()(const T& t) const
            {
                return ndb::expression<filter_, T> { t };
            }
        };
        //static constexpr ndb::expression<filter_> filter;

        struct clause_list : basic_type<>{};

        struct logical_and : basic_type<expression_forms::a_op_b>{};
    }


    // generic
    template<class Type>
    struct expression_type
    {
        // make all args
        template<class Native_expression, class... Args>
        static constexpr void make(Native_expression& ne)
        {
            expression_form<Type::form, Native_expression::category>::template make<Type, Native_expression, Args...>(ne);
        }
    };


    // scalar
    template<>
    struct expression_type<expressions::field>
    {
        template<class Native_expression, class Field>
        static constexpr void make(Native_expression& s)
        {
            s.append("F");
        }
    };

    template<>
    struct expression_type<expressions::value>
    {
        template<class Native_expression, class Field>
        static constexpr void make(Native_expression& s)
        {
            s.append("$");
        }
    };

    // specific
    template<>
    struct expression_type<expressions::range>
    {
        template<class Native_expression, class Field, class T1, class T2>
        static constexpr void make(Native_expression& s)
        {
            Field::make(s);
            s.append(expression_code<expressions::range, Native_expression::category>::value);
            T1::make(s);
            s.append(expression_code<expressions::logical_and, Native_expression::category>::value);
            T2::make(s);
        }
    };

    template<>
    struct expression_type<expressions::table_>
    {
        template<class Native_expression, class Field>
        static constexpr void make(Native_expression& s)
        {
            s.append("TTT");
        }
    };


//
    template<>
    struct expression_type<ndb::expressions::statement>
    {
        template<class Native_expression, class... Args>
        static constexpr void make(Native_expression& s)
        {
            //s.append(sizeof...(Args));


            /*
            ndb::for_each

            make_get<
            make_source<
            make_filter<


deduction<T1, T2>::make
     deduction_make<T1
     source::make
     T2::make



            st_order<get, source, filter, sort>

             stmt << sort()
             require



             */

            (Args::make(s), ...);
        }
    };
} // ndb


#endif // EXPRESSION_TYPE_NDB
