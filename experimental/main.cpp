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

ndb_table(movie,
          ndb_field_id,
          ndb_field(name, std::string, ndb::size<255>)
)
ndb_table(music,
          ndb_field(id, int),
          ndb_field(image, std::string)
)
ndb_model(library, movie, music)

ndb_project(my_project,
            ndb_database(library, library, ndb::sqlite)
)


//#include "database.hpp"


int main()
{
    using ndb::sqlite;
    // using db = dbs::zeta;
    using db = ndb::databases::my_project::library_;
    constexpr auto library = ndb::models::library;

    std::stringstream result;

    try
    {
        ndb::initializer<sqlite> init;
        ndb::connect<db, sqlite>();



        //! add records
        //ndb::query<db>() + (library.movie.name = "Interstellar");
        //ndb::query<db>() + (library.movie.name = "Watchmen");

        auto [test] = ndb::oquery<db>() << (library.music);
        std::cout << "\n___" << test.image;


        auto [interstellar] = ndb::oquery<db>() << (library.movie.id == 1);
        std::cout << interstellar.id << " | " << interstellar.name << std::endl;

        for (auto [id, name] : ndb::oquery<db>() << library.movie)
        {
            std::cout << "id : " << id << std::endl;
            std::cout << "name : " << name << std::endl;
        }

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