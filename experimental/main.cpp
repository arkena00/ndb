#include <iostream>

#include <ndb/expression.hpp>

#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>
#include <ndb/engine.hpp>
#include <ndb/function.hpp>

#include "debug/query.hpp"

#include "database.hpp"

static constexpr const models::library library;
static constexpr const databases::project project;


int main()
{
    const auto& movie = library.movie;
    const auto& music = library.music;

    try
    {
        ndb::initializer<ndb::sqlite> init;
        ndb::connect<dbs::zeta>();
        ndb::clear<dbs::zeta>(movie);

        ndb::query<dbs::zeta>().test(  (ndb::now(), movie.id, ndb::count( (movie.name, movie.id) )) );

        //ndb::query<dbs::zeta>() << (  ndb::count(movie.id) << (movie.id == 1) );

    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception : " << e.what();
    }
    return 0;
}