//#define NDB_DEBUG_QUERY

#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>
#include <ndb/query.hpp>
#include <ndb/statement.hpp>
#include <iostream>
#include <sstream>

#include <ndb/preprocessor.hpp>

/* tree_table

template<class Table, class Depth>
ndb::hierarchy_table<Table>
{
    static constexpr struct parent_id_ : ndb::field<Table, int> {} parent_id {};
    static constexpr struct depth_ : ndb::field<Table, int> {} depth {};
    static constexpr struct path_ : ndb::field<Table, ndb::bytes, ndb::size<Depth>> {} path {};
}
struct my_htable : ndb::table<Model>, ndb::hierarchy_table<my_htable, depth<10>>
{
    // name
}

query() << (myhtable.name) << ndb::hierarchy_table::childs(id)

 template<class Table>
ndb::connection_table<Table>
{
    static constexpr struct source_id_ : ndb::field<Table, int> {} source_id {};
    static constexpr struct target_id_ : ndb::field<Table, int> {} target_id {};
}




struct my_table : ndb::table<Model>, ndb::connection_table<my_ctable>
{
    // id
    // name
    static constexpr struct connexions_ : ndb::field<my_table, int> {} connexions {};
}

 */

// database

/*
ndb_table(
movie
, ndb_field_id
, ndb_field(f1, char)
, ndb_field(f2, int8_t, ndb::size<255>)
, ndb_field(f3, int16_t, ndb::size<255>)
, ndb_field(f3, int32_t, ndb::size<255>)
)*/

ndb_table(movie
          , ndb_field_id
          , ndb_field(name, std::string, ndb::size<255>)
          , ndb_field(director, std::string, ndb::size<255>)
)
ndb_table(music,
          ndb_field(id, int),
          ndb_field(image, std::string)
)
ndb_model(library, movie, music)

ndb_project(exp_main,
            ndb_database(library, library, ndb::sqlite)
)

template<class T>
constexpr auto zeta(const T& t)
{
    return ndb::expression<ndb::expressions::filter_, T> { t };
}


int main()
{
    using ndb::sqlite;
    // using db = dbs::zeta;
    using db = ndb::databases::exp_main::library_;
    constexpr auto& library = ndb::models::library;
    constexpr auto& movie = ndb::models::library.movie;

    std::stringstream result;

    try
    {
        ndb::initializer<sqlite> init;
        ndb::connect<db>();

        using namespace ndb::expressions;

        //TESTS
        // statement
        // composition
        // nested
        // dynamic
        // deduction : clause, keywork

        //auto xpr = (movie.id, ndb::get(movie.name), movie.id) << movie;
        //auto xpr2 = ndb::get(movie.id = 0, movie.name = 1, movie.id = 2) ;

        //auto sub1 = ndb::get(movie.id) ;
        //auto xpr = ndb::statement << (movie.id, movie.name) << (movie) << ( movie.id == ndb::substatement( sub1 ) );
        //auto xpr2 = xpr << ndb::filter( movie.id == ndb::substatement( sub1 ) );

        //auto xpr_del = ndb::expressions::source << ndb::filter(movie.id == 3);

        int x = 3;
        constexpr auto xpr_del =  ndb::statement << ndb::filter(movie.id == 99);
        constexpr auto xpr_add = ndb::statement << ndb::add(movie.id = 3);

        //ndb::query<db>() << (ndb::add(movie.name = std::string("eryz"), movie.director = std::string("director")));


        /*
        ndb::query<db>() <<
        (
            ndb::get(movie.id, movie.name)
            << ndb::source(movie)
            << ndb::filter(movie.id == 99)
        );*/


        constexpr auto expr = ndb::statement << ndb::del << ndb::source(movie) << ndb::filter(movie.id == 3);

        std::string output;

        using Expr = std::decay_t<decltype(expr)>;
        constexpr auto sql = ndb::native_expression<Expr, ndb::sqlite>{};
        std::cout << "\nsql str :" << sql.str();
        std::cout << "\n str : " << ndb::type_str<Expr>();

        ndb::syntax_check(xpr_del);

        ndb::dynamic_statement<db> stmt;
        stmt << ndb::get(movie.id, movie.name) << ndb::source(movie) << ndb::filter;
        for (int i = 0; i < 3; ++i)
        {
            stmt << (movie.id == 3);

            if (i < 3 - 1) stmt << ndb::logical_and;
        }
        std::cout << "\ndyn output : " << stmt.native();

    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}