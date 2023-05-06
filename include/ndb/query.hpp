#ifndef QUERY_H_NDB
#define QUERY_H_NDB

#include <ndb/engine.hpp>
#include <ndb/expression.hpp>
#include <ndb/expression/operator.hpp>
#include <ndb/option.hpp>
#include <ndb/result.hpp>
#include <ndb/setup.hpp>
#include <ndb/statement.hpp>

namespace ndb
{
    class query_base{};

    template<class Database, class Option = ndb::option<>>
    class basic_query : query_base
    {
    public:
        using Engine = typename Database::engine;

        constexpr basic_query() {}

        template<class T>
        constexpr auto operator<<(T&& t)
        {
            const auto& engine = ndb::engine<Engine>::get();
            auto expr = ndb::statement << ndb::expr_make(t);

            return engine.template exec<Database, Option>(std::move(expr));
        }

        template<class T>
        constexpr auto operator>>(T&& t)
        {
            const auto& engine = ndb::engine<Engine>::get();
            auto expr = ndb::statement << ndb::expressions::set(ndb::expr_make(t));

            return engine.template exec<Database, Option>(std::move(expr));
        }

        template<class T>
        constexpr auto operator+(T&& t) const
        {
            const auto& engine = ndb::engine<Engine>::get();
            auto expr = ndb::statement << ndb::expressions::add(ndb::expr_make(t));

            return engine.template exec<Database, Option>(std::move(expr));
        }

        template<class T>
        constexpr auto operator-(T&& t)
        {
            const auto& engine = ndb::engine<Engine>::get();
            auto expr = ndb::statement << ndb::expressions::del(ndb::expr_make(t));

            return engine.template exec<Database, Option>(std::move(expr));
        }
    };

    template<class Database, class Option = ndb::option<>>
    using query = basic_query<Database, Option>;

    template<class Database, class Option = ndb::option<>>
    using oquery = basic_query<Database, ndb::option<ndb::query_option::object>>;


    // utility class to display querys
    template<class Database>
    class query_str : query_base
    {
        using Engine = typename Database::engine;
    public:
        constexpr query_str() {}

        template<class T>
        constexpr auto operator=(const T& t)
        {
            const auto& engine = ndb::engine<Engine>::get();

            auto expr = ndb::expr_make(t);
            //auto e = ndb::expression<decltype(expr), expr_type_code::root, void, expr_clause_code::get> { std::move(expr) };

            //return Engine::to_string(e);
        }

        template<class T>
        constexpr auto operator<<(const T& t)
        {
            const auto& engine = ndb::engine<Engine>::get();

            auto expr = ndb::statement << ndb::expr_make(t);

            return Engine::to_string(expr);
        }

        template<class Expr>
        constexpr auto operator+(const Expr& expr)
        {
            const auto& engine = ndb::engine<Engine>::get();
            //auto e = ndb::expression<Expr, expr_type_code::root, void, expr_clause_code::add> { expr };

            //return Engine::to_string(e);
        }

        template<class Expr>
        constexpr auto operator-(const Expr& expr)
        {
            const auto& engine = ndb::engine<Engine>::get();
            //auto e = ndb::expression<Expr, expr_type_code::root, void, expr_clause_code::del> { expr };

            //return Engine::to_string(e);
        }
    };
} // ndb

#endif // QUERY_H_NDB