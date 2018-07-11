#ifndef EXPRESSION_DEDUCE_H_NDB
#define EXPRESSION_DEDUCE_H_NDB

#include <ndb/utility.hpp>
#include <iostream>
#include <ndb/expression/type.hpp>

namespace ndb
{
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

#endif // EXPRESSION_DEDUCE_H_NDB

