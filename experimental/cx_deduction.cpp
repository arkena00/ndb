#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>

#include "debug/query.hpp"
#include <iostream>
#include <sstream>
#include <cxxabi.h>
#include <ndb/expression/deduction.hpp>

#include "database.hpp"

#include <kvasir/mpl/algorithm/filter.hpp>
#include <kvasir/mpl/algorithm/transform.hpp>
#include <kvasir/mpl/algorithm/extreme.hpp>
#include <kvasir/mpl/functional/call.hpp>
#include <kvasir/mpl/sequence/join.hpp>
#include <kvasir/mpl/sequence/front.hpp>
#include <kvasir/mpl/types/int.hpp>
#include <kvasir/mpl/types/list.hpp>
#include <kvasir/mpl/functions/arithmetic/plus.hpp>
namespace ndb
{
    template<class Expr>
    struct expr_is_field_t : std::conditional_t<Expr::type == expr_type_code::field, std::true_type, std::false_type>
    {
    };

    template<class Expr>
    struct expr_is_table_t : std::conditional_t<Expr::type == expr_type_code::table, std::true_type, std::false_type>
    {
    };
    template<class Table>
    constexpr unsigned char table_id_str = table_id<Table> + 48;
}





constexpr auto movie = models::library.movie;

namespace mpl = kvasir::mpl;

template<class Expr>
struct linearize_impl
{
    using make = mpl::list<>;
};

template<class Expr>
using linearize = typename linearize_impl<Expr>::make;

template<class Expr>
using linearize_rec_impl =
typename mpl::conditional<ndb::expr_is_scalar<Expr>>
::template f<mpl::listify, mpl::cfe<linearize>>
::template f<Expr>
;

template<class L, ndb::expr_type_code T, class R, ndb::expr_clause_code Clause>
struct linearize_impl<ndb::expression<L, T, R, Clause>>
{
    using make = typename mpl::join<>::template f<
    linearize_rec_impl<L>,
    linearize_rec_impl<R>
    >;
};


template<class T>
struct expr_size
{
    static constexpr auto value = 0;
};

template<class L, class R, ndb::expr_clause_code Clause>
struct expr_size<ndb::expression<L, ndb::expr_type_code::field, R, Clause>>
{
    static constexpr auto value = 2;
};

template<class L, class R, ndb::expr_clause_code Clause>
struct expr_size<ndb::expression<L, ndb::expr_type_code::table, R, Clause>>
{
    static constexpr auto value = 10;
};

template<class Expr>
struct cx_expr
{
    using linear_type = linearize<Expr>;
    using tables = mpl::eager::filter<linear_type, ndb::expr_is_table_t>;

    using main_table = typename
    mpl::call
    <
    mpl::unpack
    <
    mpl::filter<mpl::cfe<ndb::expr_is_field_t>, mpl::front<>>
    >
    , linear_type
    >::value_type::table;

    static constexpr auto size =
                          mpl::call
                          <
                          mpl::unpack
                          <
                          mpl::transform
                          <
                          mpl::cfe<expr_size>, mpl::extreme<mpl::plus<> >
    >
    >
    , linear_type
    >::value;
};


int main()
{
    using ndb::sqlite;

    std::stringstream result;

    try
    {
        ndb::initializer<sqlite> init;
        ndb::connect<dbs::zeta, sqlite>();


        const auto& user = models::library.user;


        auto expr = (movie.id, movie.name) << (movie);

        using Expr = decltype(expr);

        auto realname = abi::__cxa_demangle(typeid( cx_expr<Expr>::size ).name(), 0, 0, 0);
        std::cout << "\n" << realname;
        std::cout << "\n main_table" << ndb::table_id_str<cx_expr<Expr>::main_table>;
        std::cout << "\n size " << cx_expr<Expr>::size;


    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }


    return 0;
}