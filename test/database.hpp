#include <ndb/preprocessor.hpp>

ndb_table(
movie
, ndb_field(id, int)
, ndb_field(name, std::string, ndb::size<255>)
, ndb_field(image, std::string, ndb::size<255>)
)

ndb_model(library, movie)

ndb_project(
query,
ndb_database(db, library, ndb::sqlite)
)

namespace dbs
{
    using zeta = ndb::databases::query::db_;
}