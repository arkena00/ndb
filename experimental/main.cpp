#define NDB_QUERY_DEBUG

#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>
#include <ndb/query.hpp>
#include <iostream>
#include <sstream>

#include <ndb/expression/deduce.hpp>
#include <ndb/preprocessor.hpp>

#include <cxxabi.h>

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

/*
    namespace ndb {
// associate generic type to cpp type
template<class Engine>
struct cpp_type_<Engine, ndb::string_>
{
    using type = std::string;
};}

struct custom_string
{

};*/

// database

ndb_table(  movie
          , ndb_field_id
          , ndb_field(name, std::string, ndb::size<255>)
          , ndb_field(duration, std::chrono::duration<double, std::ratio<3600>>)
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
namespace ndb
{
    template<class>
    struct type_map;

    template<class T, class... Types>
    struct type_get;




    template<class T>
    struct __cpp_type
    {
        using type = typename type_map<T>::type;
    };

    template<class T>
    struct __ndb_type
    {
        using type =typename type_get<T, ndb_types_t>::type;
    };


    template<class Cpp_type>
    struct type_get< Cpp_type, ndb_types<> >
    {
        using type = cx_err_type_map_not_found<Cpp_type>;
    };

    template<class Cpp_type, class Ndb_type, class... Types>
    struct type_get<Cpp_type, ndb_types<Ndb_type, Types...>>
    {
        using tested_type = typename __cpp_type<Ndb_type>::type;
        using type = std::conditional_t<std::is_same_v<Cpp_type, tested_type>, Ndb_type, typename type_get< Cpp_type, ndb_types<Types...> >::type >;
    };



    template<>
    struct type_map<ndb::types::float_> { using type = float; };

    template<>
    struct type_map<ndb::types::int_> { using type = int; };
}



int main()
{
    using ndb::sqlite;
    // using db = dbs::zeta;
    using db = ndb::databases::exp_main::library_;
    constexpr auto library = ndb::models::library;



    std::stringstream result;

    try
    {
        ndb::initializer<sqlite> init;
        //ndb::config<db>(ndb::connection_flag::read_only);
        ndb::connect<db, sqlite>("database");

        //! add records
        //ndb::query<db>() + (library.movie.name = "Interstellar");
        //ndb::query<db>() + (library.movie.name = "Watchmen");

        //ndb::query<db>() + (library.movie.name = "Watchmen");
        //ndb::query<db>() >> ((library.movie.name = ndb::now()) << (library.movie.id == 1));

        std::string tg = "ello";
        ndb::query<db>() >> ((library.movie.name = ndb::now(), library.movie.name = "Watchmen") << (library.movie.name == tg));

        using namespace ndb;

        std::cout << "\n" << abi::__cxa_demangle(typeid( __cpp_type<float_>::type ).name(), 0, 0, 0);


        std::cout << "\n" << abi::__cxa_demangle(typeid( __ndb_type<std::string>::type ).name(), 0, 0, 0);



        /*

        auto [interstellar] = ndb::oquery<db>() << (library.movie.id == 1);
        std::cout << interstellar.id << " | " << interstellar.name << std::endl;

        for (auto [id, name] : ndb::oquery<db>() << library.movie)
        {
            std::cout << "id : " << id << std::endl;
            std::cout << "name : " << name << std::endl;
        }*/

        //auto q = (movie.id << movie);
        //auto z = ((movie.id, q) << movie);

    /*
        auto z = (movie.id << movie);
        auto u = ndb::limit(3);
        ndb::query<dbs::zeta> v;
        ndb::result<dbs::zeta> res = z << u;
        v = z << u;
    */

    /*
        ndb::query_view<dbs::zeta> q;
        q << (movie.id, movie.name, ndb::get(movie.image));
        std::cout << q.view();*/

    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}