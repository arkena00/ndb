#include "../test.hpp"

#include <ndb/preprocessor.hpp>
#include <ndb/engine/type.hpp>
#include <ndb/function.hpp>

ndb_table(
    movie
    , ndb_field_id
    , ndb_field(name, std::string, ndb::size<255>)
    , ndb_field(duration, std::chrono::duration<double, std::ratio<3600>>)
)

ndb_model(library, movie)

ndb_project(type_test,
            ndb_database(library, library, ndb::sqlite)
)

namespace dbs
{
    using zeta = ndb::databases::type_test::library_;
}


template<>
struct type_map<ndb::types::float_> { using type = float; };



template <typename Engine>
class type : public testing::Test
{
public:
    type()
    {
        ndb::connect<dbs::zeta, Engine>( );
        ndb::clear<dbs::zeta>(ndb::models::library.movie);
    }
};

typedef ::testing::Types<TEST_ENGINE> Engines;
TYPED_TEST_CASE( type, Engines );

TYPED_TEST(type, general)
{
    using Engine = TypeParam;

    ASSERT_TRUE(( std::is_same_v< ndb::cpp_type_t<ndb::string_, Engine>, std::string > ));

    ASSERT_TRUE(( std::is_same_v< ndb::cpp_type_t<ndb::string_, Engine>, std::string > ));
}
