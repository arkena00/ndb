#ifndef EXPRESSION_DEDUCTION_H_NDB
#define EXPRESSION_DEDUCTION_H_NDB

#include <ndb/utility.hpp>
#include <ndb/cx_error.hpp>
#include <ndb/expression/type.hpp>

namespace ndb
{
    namespace deductions
    {
        namespace internal
        {
            template<class... Exprs>
            struct extract_expr;

            template<class...>
            struct extract;

            template<class T>
            struct extract<T>
            {
                using result = ncx_error_type(extract, cx_err_query, T);
            };

            template<>
            struct extract_expr<>
            {
                using result = ::ndb::cx_err<>;
            };

            template<class Expr_0, class... Exprs>
            struct extract_expr<Expr_0, Exprs...>
            {
                using result = typename extract<Expr_0, Exprs...>::result;
            };

            template<class T, class... Args, class... Exprs>
            struct extract<ndb::expression<T, Args...>, Exprs...>
            {
                using result = typename extract_expr<Args..., Exprs...>::result;
            };

            // field found
            template<class Field, class... Exprs>
            struct extract<ndb::expression<ndb::expressions::field_, Field>, Exprs...>
            {
                using result = typename std::decay_t<Field>::table;
            };

            // table found
            template<class Table, class... Exprs>
            struct extract<ndb::expression<ndb::expressions::table_, Table>, Exprs...>
            {
                using result = typename std::decay_t<Table>;
            };
        } // internal

        template<class Expr>
        using table = typename internal::extract_expr<Expr>::result;

    } // deductions

    template<class Expr>
    void syntax_check(const Expr& expr)
    {
        using expr_type = typename Expr::type;

        //expect<expr_type, ndb::expressions::statement>
/*
        std::apply([](auto&&... arg)
                   {
                       ((std::cout << "\nTYPE_STR : " << type_str<decltype(arg)>()), ...);
                   }, expr.args());

        std::cout << "\nTYPE_STR : " << type_str<expr_type>();*/
    }

    template<class Statement, class R>
    struct deduction
    {
        static auto process(Statement s, R r)
        {
            auto clause_l = std::get<s.arg_count() - 1>(s.args());
            std::cout << "\ndeduce_L : " << type_str<decltype(clause_l)>();
            std::cout << "\ndeduce_R : " << type_str<R>();

            ndb::expression<ndb::expressions::source_> src;
            auto dedst  = ndb::expression<ndb::statement_, decltype(clause_l), decltype(src), decltype(r)> { std::tuple_cat(clause_l.args(), std::forward_as_tuple(std::move(src)), r.args()) };
            return dedst;

        }
    };
} // ndb

#endif // EXPRESSION_DEDUCTION_H_NDB

