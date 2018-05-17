#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>

#include "debug/query.hpp"
#include <iostream>
#include <sstream>

#include "database.hpp"

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

int main()
{
    using ndb::sqlite;

    std::stringstream result;

    try
    {
        ndb::initializer<sqlite> init;
        ndb::connect<dbs::zeta, sqlite>();

        int a = 11;
        double b = 22;
        auto c = std::chrono::system_clock::now();

        const auto& movie = models::library.movie;
        const auto& user = models::library.user;

        ndb::query<dbs::zeta>() << (ndb::get(movie.id) << ndb::source(movie));


        //auto q = (movie.id << movie);
        //auto z = ((movie.id, q) << movie);

    /*
        auto z = (movie.id << movie);
        auto u = ndb::limit(3);
        ndb::query<dbs::zeta> v;
        ndb::result<dbs::zeta> res = z << u;
        v = z << u;
    */

        ndb::query_view<dbs::zeta> q;
        q << (movie.id, movie.name, ndb::get(movie.image));
        std::cout << q.view();

    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}