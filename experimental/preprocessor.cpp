#include <ndb/preprocessor.hpp>
#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>
#include <ndb/query.hpp>

namespace tables
{
    ndb_table(movie,
              ndb_field(id, int, 8),
              ndb_field(name, std::string, 255)
    )
    ndb_table(music,
              ndb_field(id, int, 8),
              ndb_field(image, std::string, 255)
    )
}

namespace models
{
    struct library
    {
        using movie_ = tables::movie<library>; static constexpr movie_ movie {};
        using music_ = tables::music<library>; static constexpr music_ music {};

        using Detail_ = ndb::model_detail
        <
        ndb::entity<movie_, music_>
        >;
    };
}

namespace databases
{
    struct project
    {
        static constexpr struct alpha_ : ndb::database<project, models::library, ndb::mongo>{} alpha{};
        static constexpr struct zeta_ : ndb::database<project, models::library, ndb::sqlite>{} zeta{};

        using Detail_ = ndb::database_detail<ndb::entity<alpha_, zeta_>>;
    };
} // databases

namespace dbs
{
    using alpha = databases::project::alpha_;
    using zeta = databases::project::zeta_;
}

static constexpr const models::library library;

int main()
{
    using ndb::sqlite;

    ndb::initializer<sqlite> init;
    ndb::connect<dbs::zeta>();

    const auto& movie = library.movie;

    // field
    ndb::query<dbs::zeta>() << (movie.id);
}