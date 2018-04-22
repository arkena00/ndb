#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>

#include "debug/query.hpp"
#include <iostream>
#include <sstream>

#include "database.hpp"

int main()
{
    using ndb::sqlite;

    std::stringstream result;

    ndb::initializer<sqlite> init;
    ndb::connect<dbs::zeta, sqlite>();

    int a = 11;
    double b = 22;
    auto c = std::chrono::system_clock::now();

    const auto& movie = models::library.movie;
    const auto& user = models::library.user;

    ndb::query<dbs::zeta>() << ndb::get(user.id, user.name);

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



    return 0;

}