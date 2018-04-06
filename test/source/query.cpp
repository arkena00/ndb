#include "doctest/doctest.h"
#include "../config.hpp"

#include <ndb/initializer.hpp>
#include <ndb/query.hpp>

#include "../database.hpp"

// initialize engines
ndb::initializer<TEST_ENGINE> init;
// pack engines
typedef doctest::Types<TEST_ENGINE> Engines;

SCENARIO_TEMPLATE("query", Engine, Engines)
{
    // aliases
    static constexpr const models::library library;
    const auto& movie = library.movie;

    GIVEN("cleared database")
    {
        ndb::result<Engine> result;

        CHECK_NOTHROW( (ndb::connect<dbs::zeta, Engine>()) );

        WHEN("ADD data")
        {
            REQUIRE_NOTHROW((result = ndb::query<dbs::zeta>() + (movie.id = 1, movie.name = 2)));
            REQUIRE_NOTHROW((result = ndb::query<dbs::zeta>() + (movie.id = 3, movie.name = 4)));

            AND_WHEN("GET data")
            {
                REQUIRE_NOTHROW( (result = ndb::query<dbs::zeta>() << (movie.id, movie.name)) );

                THEN("data count match")
                {
                    CHECK(result.size() == 2);
                }

                AND_THEN("data match")
                {
                    CHECK(result[0][movie.id] == 1);
                    CHECK(result[0][movie.name] == "2");

                    CHECK(result[1][movie.id] == 3);
                    CHECK(result[1][movie.name] == "4");
                }
            }
        }
    }
}