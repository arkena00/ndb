#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>

#include "debug/query.hpp"
#include <iostream>

#include "database.hpp"

int main()
{
    using ndb::sqlite;


    ndb::initializer<sqlite> init;
    ndb::connect<dbs::zeta, sqlite>();

    int a = 11;
    double b = 22;
    auto c = std::chrono::system_clock::now();

    const auto& movie = models::library.movie;
    //const auto& sound = library.sound;

    ndb::query<dbs::zeta>() << ndb::get( movie.id );

    // table
    ndb::query<dbs::zeta>() << movie;

    // field
    ndb::query<dbs::zeta>() << ( movie.id );
    // field_list

    ndb::query<dbs::zeta>() << ( movie.id, movie.image );
    // field << table
    ndb::query<dbs::zeta>() << ( movie.id << movie );

    // coondition
    ndb::query<dbs::zeta>() << ( movie.id == a );
    ndb::query<dbs::zeta>() << ( movie.id == a && movie.name == b );

    // field << table << condition
    ndb::query<dbs::zeta>() << ( (movie.id) << movie << (movie.id == a && movie.name == b) );
    ndb::query<dbs::zeta>() << ( (movie.id, movie.name) << movie << (movie.id == a && movie.name == b) );
    // field << condition
    ndb::query<dbs::zeta>() << ( (movie.id, movie.name, movie.image) << (movie.id == a && movie.name == b) );

    // field << join
    //ndb::query<db::library>{} << movie.name << (  movie.user_id == user.id );

    // add
    ndb::query<dbs::zeta>() + ((movie.id = 3, movie.name = 5));

    // del
    ndb::query<dbs::zeta>() - ( movie.id == 3 );


    ndb::query<dbs::zeta>() + ((movie.id = 3, movie.name = 5));

    // (movie.name = "test")
    //  ndb::count(movie.id) -movie << movie.id == 3 ndb::del << movie.id == 3

    //ndb_pquery(list_movies, movie.genre, movie.actor);
    //qs::list_movies(genre_enum::action, { "john", "sheppard" });

    //ndb::pquery<dbs::alpha>() << (movie.id == 3);

    return 0;
}