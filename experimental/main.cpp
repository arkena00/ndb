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

        ndb::query<dbs::zeta>() + ( movie.id = 3 );
        ndb::query<dbs::zeta>() + ( movie.id = 5 );
        ndb::query<dbs::zeta>() + ( movie.id = 5 );

        using namespace ndb;

        query<dbs::zeta> q;
        auto res = q << (now(), now(), movie.id ) ;

        for (auto line : res)
        {
            std::cout << "\n__" <<line[movie.id] << line[0].get<std::string>() << line[1].get<std::string>();
        }

        ndb::query<dbs::zeta>() << (  ndb::count(movie.id) << (movie.id == 1) );

    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception : " << e.what();
    }
    return 0;
}