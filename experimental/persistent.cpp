#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>

#include <ndb/persistent.hpp>

/*
#define ndb_persistent(NAME, TYPE, __VA_ARGS__) ndb::persistent<TYPE> NAME{ this, #NAME, __VA_ARGS__ };

#define ndb_internal_make_persitent_impl(NODE, NAME, MODEL_NAME, DB_ENGINE) static constexpr struct NAME##_ : ::ndb::database< ::ndb::databases::PROJECT_NAME, ::ndb::models::MODEL_NAME##_, DB_ENGINE >{} NAME{};
#define ndb_internal_make_persitent_impl_args(data, item) (data, item)

#define ndb_internal_make_persitent(r, data, item) BOOST_PP_EXPAND(ndb_internal_make_persitent_impl ndb_internal_make_persitent_impl_args(data, ndb_internal_unpack(item)) )

#define ndb_persistent_root(NODE, __VA_ARGS__) \
struct NODE : ndb::persistent_group { \
using persistent_group::persistent_group; \
BOOST_PP_LIST_FOR_EACH( \
        ndb_internal_make_persitent, \
        NODE, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
    ) \
};

#define ndb_persistent_group(NODE, __VA_ARGS__) \
struct NODE : ndb::persistent_group { \
using persistent_group::persistent_group; \
BOOST_PP_LIST_FOR_EACH( \
        ndb_internal_make_persitent, \
        NODE, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
    ) \
} NODE{ this, #NODE };
*/

struct nxi : ndb::persistent_group
{
    using persistent_group::persistent_group;

    struct : ndb::persistent_group
    {
        using persistent_group::persistent_group;

        ndb::persistent<std::string> home{ this, "home", "test" };

        struct : ndb::persistent_group
        {
            using persistent_group::persistent_group;

            ndb::persistent<std::string> min_width{ this, "min_width" };
            ndb::persistent<std::string> max_width{ this, "max_width" };
        } tabs{ this, "tabs" };
    } browser{ this, "browser" };
};

/*
ndb_persistent_root(nxi
    , ndb_persistent_group(browser
        , ndb_persistent(home, std::string, "test")
        , ndb_persistent_group(tabs
            , ndb_persistent(max_width, std::string, "test")
        )
    )
)*/

int main()
{
    using ndb::sqlite;

    try
    {
        ndb::initializer<sqlite> init;

        ::nxi nxi{ "nxi" };

        std::cout << "\n" << nxi.browser.home.path() << " : " << nxi.browser.home.get();
        std::cout << "\n" << nxi.browser.tabs.max_width.path() << " : " << nxi.browser.tabs.max_width.get();
        std::cout << "\n" << nxi.browser.tabs.min_width.path() << " : " << nxi.browser.tabs.min_width.get();

        nxi.browser.home = std::string("neuroshok.com");
        nxi.browser.tabs.max_width = "50";
        nxi.browser.tabs.min_width = "99";


    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}