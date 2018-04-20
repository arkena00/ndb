#ifndef QUERY_H_NDB
#define QUERY_H_NDB

#include <ndb/setup.hpp>
#include <ndb/expression.hpp>
#include <ndb/engine.hpp>
#include <ndb/result.hpp>
#include <iostream>

namespace ndb
{
    template<class Database, class Engine = typename Database::engine>
    class query
    {
    public:
        constexpr query() {}

        template<class T>
        constexpr auto operator<<(const T& t)
        {
            const auto& engine = ndb::engine<Engine>::get();

            auto expr = ndb::expr_make(t);
            auto e = ndb::expression<decltype(expr), expr_type_code::root, void, expr_clause_code::get> { std::move(expr) };

            std::cout << ndb::sqlite::to_string(e) << "\n";
            return 0;

            //return engine.template exec<Database>(e);
        }

        template<class Expr>
        constexpr auto operator+(const Expr& expr) const
        {
            const auto& engine = ndb::engine<Engine>::get();
            auto e = ndb::expression<Expr, expr_type_code::root, void, expr_clause_code::add> { expr };

            std::cout << "\n" << ndb::sqlite::to_string(e);
            return 0;
        }

        template<class Expr>
        constexpr auto operator-(const Expr& expr)
        {
            const auto& engine = ndb::engine<Engine>::get();
            auto e = ndb::expression<Expr, expr_type_code::root, void, expr_clause_code::del> { expr };

            std::cout << "\n" << ndb::sqlite::to_string(e);
            return 0;
        }
    };

    //template<class Database>
    //static constexpr auto query = query_impl<Database>();
} // ndb

#endif // QUERY_H_NDB