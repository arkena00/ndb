#ifndef EXPRESSION_SQL_TYPE_NDB
#define EXPRESSION_SQL_TYPE_NDB

#include <ndb/expression/type.hpp>
#include <ndb/statement.hpp>
#include <iostream>

namespace ndb
{
    namespace internal
    {
        template<class Engine, class Build_type, class T>
        struct expr_builder;

        template<class...>
        struct expr_build_type;

        template<class Engine, class L, class R>
        struct expr_builder<Engine, expr_build_type<expressions::add_>, ndb::expression<expressions::assign_, L, R>>
        {
            template<int Pass, class Native_expression>
            static constexpr void process(Native_expression& ne)
            {
                if constexpr (Pass == 0)
                {
                    L::template make<Engine>(ne);
                }
                else R::template make<Engine>(ne);
                ne.append(',');
            }
        };

        template<class Engine, class... Assigns>
        struct expr_builder<Engine, expr_build_type<expressions::add_>, ndb::expression<expressions::list_, Assigns...>>
        {
            template<int Pass, class Native_expression>
            static constexpr void process(Native_expression& ne)
            {
                (expr_builder<Engine, expr_build_type<expressions::add_>, Assigns>::template process<Pass>(ne), ...);
            }
        };
    } // internal

    template<class Engine>
    struct expression_type<expressions::add_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class List>
        static constexpr void make(Native_expression& ne)
        {
            using Table = typename std::decay_t<typename List::template arg_at<0>::template arg_at<0>::template arg_at<0>>::table;

            ne.append(expression_code<expressions::add_, Engine, Native_expression::category>::value);
            ne.append("T");
            ne.append(ndb::table_id<Table> + '0');
            ne.append("(");

            internal::expr_builder<Engine, internal::expr_build_type<expressions::add_>, List>::template process<0>(ne);
            ne.pop();

            ne.append(") VALUES(");
            internal::expr_builder<Engine, internal::expr_build_type<expressions::add_>, List>::template process<1>(ne);
            ne.pop();
            ne.append(")");
        }
    };

//
    template<class Engine>
    struct expression_type<ndb::statement_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class... Args>
        static constexpr void make(Native_expression& s)
        {
            //s.append(sizeof...(Args));

            (Args::template make<Engine>(s), ...);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////             SCALAR             ////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    template<class Engine>
    struct expression_type<expressions::field_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Field>
        static constexpr void make(Native_expression& s)
        {
            s.append("F");
            s.append('0' + ndb::field_id<Field>);
        }
    };

    template<class Engine>
    struct expression_type<expressions::table_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Table>
        static constexpr void make(Native_expression& s)
        {
            s.append("T");
            s.append('0' + ndb::table_id<Table>);
        }
    };

    template<class Engine>
    struct expression_type<expressions::value_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Field>
        static constexpr void make(Native_expression& s)
        {
            s.append("$");
            s.add_value();
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////            OPERATOR            ////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    template<class Engine>
    struct expression_type<expressions::range_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Field, class T1, class T2>
        static constexpr void make(Native_expression& s)
        {
            Field::template make<Engine>(s);
            s.append(expression_code<expressions::range_, Engine, Native_expression::category>::value);
            T1::template make<Engine>(s);
            s.append(expression_code<expressions::logical_and_, Engine, Native_expression::category>::value);
            T2::template make<Engine>(s);
        }
    };
} // ndb


#endif // EXPRESSION_SQL_TYPE_NDB
