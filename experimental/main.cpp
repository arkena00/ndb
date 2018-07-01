//#define NDB_DEBUG_QUERY

#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>
#include <ndb/query.hpp>
#include <iostream>
#include <sstream>

#include <ndb/expression/deduce.hpp>
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


//#include "database.hpp"





/*
// binary
template<class Type>
struct expression_make<ndb::expression<Type, T1, T2>>
{
    static void process(std::string& s)
    {
        expression_make<T1>::process(s);
        s+="_";
        s += expr_code<Type>::value ;
        expression_make<T2>::process(s);
    }
};

/*
// unary
template<class Type, class T>
struct expression_make<ndb::expression<Type, T>>
{
    static void process(std::string& s)
    {
        s += expr_code<Type>::value ;
        expression_make<T>::process(s);
    }
};

//! statement< clause_list >
template< class... Args>
struct expression_make<ndb::expression<ndb::expression_types::statement, Args...>>
{
    static void process(std::string& s)
    {
        std::cout << sizeof...(Args);

        (expression_make<Args>::process(s), ...);
    }
};


template<class... Args>
struct expression_make<ndb::expression<ndb::expression_types::get, Args...>>
{
    static void process(std::string& s)
    {
        s += "(";
        s += "SELECT ";
        (expression_make<Args>::process(s), ...);
        s += ")";
    }
};
*/




namespace ndb
{
    struct statement_
    {
        template<class Expr>
        constexpr auto operator<<(const Expr& e) const
        {
            auto expr = ndb::expr_make(e);
            return ndb::expression<ndb::expression_types::statement, decltype(expr)> { expr };
        }
    };
    static constexpr statement_ statement;
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


        //auto xpr = (movie.id, ndb::get(movie.name), movie.id) << movie;
        //auto xpr2 = ndb::get(movie.id = 0, movie.name = 1, movie.id = 2) ;

        auto xpr = ndb::statement << ndb::get(movie.id, movie.name)  << ndb::source(movie) << ndb::filter( movie.id == 3 );



        std::string output;

        /*
        get< list< list<A, B>, C > >
         expr< get<get_list<F, GET<>> >

         */


        using Expr = std::decay_t<decltype(xpr)>;
       // std::cout << ndb::type_str<Expr>();
        Expr::make(output);
        std::cout << "\noutput : " << output;

        /*
        output = "";
        //std::cout << "\n\n" << ndb::type_str<decltype(xpr2)>();
        expression_make<std::decay_t<decltype(xpr2)>>::process(output);
        std::cout << "\noutput : " << output;*/


    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}