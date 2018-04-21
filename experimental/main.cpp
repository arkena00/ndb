#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>

#include "debug/query.hpp"
#include <iostream>
#include <sstream>

#include "database.hpp"


#define Query(Q) { ndb::query_view<dbs::zeta> q; Q; result << __COUNTER__ << " | " << #Q << "\n  \\ " << q.view() ; } \
 { bool q_ok = true; ndb::query<dbs::zeta> q; try { Q; } catch(...) { q_ok = false; } result << "\n  \\ "; if (q_ok) result << "OK"; else result << "FAIL";  result << "\n"; }


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


    // fields
    try
    {
        ndb::query<dbs::zeta>() << user.id;
    }
    catch(...) {std::cerr << "ERROR";}

    /*
    Query((   q << user.id   ));
    Query((   q << ( movie.id, movie.image )   ));
    // fields << table
    Query((   q << ( (movie.id, movie.image) << movie)   ));

*/


    // join
    //Query((   q << (movie.name << (  movie.id == user.id ))   ));
/*

    // field << join
    //ndb::query<db::library>{} << movie.name << (  movie.user_id == user.id );


    // (movie.name = "test")
    //  ndb::count(movie.id) -movie << movie.id == 3 ndb::del << movie.id == 3

    //ndb_pquery(list_movies, movie.genre, movie.actor);
    //qs::list_movies(genre_enum::action, { "john", "sheppard" });

    //ndb::pquery<dbs::alpha>() << (movie.id == 3);

    return 0;*/

    std::cout << result.str();

    std::cout << "_";

    return 0;

}