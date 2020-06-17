#ifndef EXPRESSION_SQL_TYPE_NDB
#define EXPRESSION_SQL_TYPE_NDB

#include <ndb/expression/basic.hpp>
#include <ndb/statement.hpp>
#include <iostream>
#include <ndb/expression/deduction.hpp>

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
        template<class Native_expression, class Expr>
        static constexpr void make(Native_expression& ne)
        {
            using Table = ndb::deductions::table<Expr>;

            ne.append(expression_code<expressions::add_, Engine, Native_expression::category>::value);
            ne.append(ndb::table_name<Table>);
            ne.append("(");

            internal::expr_builder<Engine, internal::expr_build_type<expressions::add_>, Expr>::template process<0>(ne);
            ne.pop();

            ne.append(") VALUES(");
            internal::expr_builder<Engine, internal::expr_build_type<expressions::add_>, Expr>::template process<1>(ne);
            ne.pop();
            ne.append(")");
        }
    };

    template<class Engine>
    struct expression_type<expressions::add_ignore_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Expr>
        static constexpr void make(Native_expression& ne)
        {
            using Table = ndb::deductions::table<Expr>;

            ne.append(expression_code<expressions::add_ignore_, Engine, Native_expression::category>::value);
            ne.append(ndb::table_name<Table>);
            ne.append("(");

            internal::expr_builder<Engine, internal::expr_build_type<expressions::add_>, Expr>::template process<0>(ne);
            ne.pop();

            ne.append(") VALUES(");
            internal::expr_builder<Engine, internal::expr_build_type<expressions::add_>, Expr>::template process<1>(ne);
            ne.pop();
            ne.append(")");
        }
    };

    template<class Engine>
    struct expression_type<expressions::add_replace_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Expr>
        static constexpr void make(Native_expression& ne)
        {
            using Table = ndb::deductions::table<Expr>;

            ne.append(expression_code<expressions::add_replace_, Engine, Native_expression::category>::value);
            ne.append(ndb::table_name<Table>);
            ne.append("(");

            internal::expr_builder<Engine, internal::expr_build_type<expressions::add_>, Expr>::template process<0>(ne);
            ne.pop();

            ne.append(") VALUES(");
            internal::expr_builder<Engine, internal::expr_build_type<expressions::add_>, Expr>::template process<1>(ne);
            ne.pop();
            ne.append(")");
        }
    };

    //

    template<class Engine>
    struct expression_type<expressions::set_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Expr>
        static constexpr void make(Native_expression& ne)
        {
            using Table = ndb::deductions::table<Expr>;

            ne.append("UPDATE ");
            ne.append(ndb::table_name<Table>);
            ne.append(" SET ");

            Expr::template make<Engine>(ne);
        }
    };

    //

    template<class Engine>
    struct expression_type<ndb::statement_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class... Args>
        static constexpr void make(Native_expression& s)
        {
            (Args::template make<Engine>(s), ...);
        }
    };
/*
    template<class Engine>
    struct expression_type<ndb::statement_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class T1, class T2>
        static constexpr void make(Native_expression& s)
        {
            //s.append(sizeof...(Args));

            T1::template make<Engine>(s);
            T2::template make<Engine>(s);
        }
    };*/

    //

    template<class Engine>
    struct expression_type<expressions::table_all_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Table>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(ndb::table_name<Table>);
            ne.append(".*");
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////            RELATION            ////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    template<class Engine>
    struct expression_type<expressions::edge_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Source, class Target, class Connection>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(" FROM ");
            ne.append(ndb::table_name<Source>);
            ne.append(" INNER JOIN ");
            ne.append(ndb::table_name<Target>);
            ne.append(" ON ");
            std::decay_t<Connection>::template make<Engine>(ne);
        }
    };

    template<class Engine>
    struct expression_type<expressions::arc_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Source, class Edge, class Target>
        static constexpr void make(Native_expression& ne)
        {
            ne.append(" FROM ");
            ne.append(ndb::table_name<Source>);
            ne.append(" INNER JOIN ");
            ne.append(ndb::edge_name<Edge>);
            ne.append(" ON ");
            ne.append(ndb::field_name<std::decay_t<Edge>::source_field>);
            ne.append(" = ");
            ne.append(ndb::edge_name<Edge>);
            ne.append(ndb::field_name<std::decay_t<Edge>::source_field>);

            ne.append(" INNER JOIN ");
            ne.append(ndb::table_name<Target>);
            ne.append(" ON ");
            ne.append(ndb::field_name<std::decay_t<Edge>::target_field>);
            ne.append(" = ");
            ne.append(ndb::edge_name<Edge>);
            ne.append(ndb::field_name<std::decay_t<Edge>::target_field>);
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
            s.append(ndb::field_name<Field>);
        }
    };

    // field alias
    template<class Engine>
    struct expression_type<expressions::alias_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Expr, class Alias_value>
        static constexpr void make(Native_expression& ne)
        {
            std::decay_t<Expr>::template make<Engine>(ne);
            ne.append(expression_code<expressions::alias_, Engine, Native_expression::category>::value);
            ne.append("`");
            ne.append(ndb::cx_string<'A', static_cast<unsigned int>(Alias_value{})>);
            ne.append("`");
        }
    };

    template<class Engine>
    struct expression_type<expressions::table_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Table>
        static constexpr void make(Native_expression& s)
        {
            s.append(ndb::table_name<Table>);
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

    template<class Engine>
    struct expression_type<expressions::constant_, Engine, expression_categories::sql>
    {
        template<class Native_expression, class Constant>
        static constexpr void make(Native_expression& s)
        {
            s.append("constant");
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
