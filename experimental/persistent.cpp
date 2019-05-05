


//#define NDB_DEBUG_QUERY

#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>
#include <ndb/query.hpp>
#include <ndb/statement.hpp>
#include <iostream>
#include <sstream>

#include <ndb/preprocessor.hpp>


// database

/*
ndb_table(
movie
, ndb_field_id
, ndb_field(f1, char)
, ndb_field(f2, int8_t, ndb::size<255>)
, ndb_field(f3, int16_t, ndb::size<255>)
, ndb_field(f3, int32_t, ndb::size<255>)
)*/


ndb_table(cat
, ndb_field_id
, ndb_field(name, std::string, ndb::size<255>)
)

ndb_table(movie
, ndb_field_id
, ndb_field(name, std::string, ndb::size<255>)
, ndb_field(director, std::string, ndb::size<255>)
)
ndb_table(user
, ndb_field(id, int)
, ndb_field(name, std::string, ndb::size<255>)
, ndb_field(image, std::string)
// ndb_field(cat, ndb::tables::cat<>)
)

ndb_table(property
, ndb_field(key, std::string, ndb::size<255>, ndb::option<ndb::field_option::unique>)
, ndb_field(value, std::string, ndb::size<255>)
)

ndb_model(library, movie, user)
ndb_model(internal, property)

//ndb_edge(user, movie, has)
//ndb_edge(user, movie, like)

ndb_project(exp_main,
            ndb_database(library, library, ndb::sqlite)
, ndb_database(property, internal, ndb::sqlite)
)

using db = ndb::databases::exp_main::library_;

struct server_
{
    int port;
    std::string name;
};


struct config_
{
    server_ server;
};

/*
ndb_property(server,
    , ndb_field(name, std::string)
    , ndb_field(port, uint16_t)
)
ndb_property(config,
    , ndb_field(ndb::properties::server)
    , ndb_field(version, int)
)*/


//ndb::load<dbs::property>();


namespace ndb
{
    template<class T, class Group>
    class persistent
    {
      public:
        persistent(const T& default_value = T{})
        : id_{ id++ }
        , value_{}
        , default_value_{ default_value }
        {
            auto res = ndb::query<ndb::databases::exp_main::property_>()
            << (ndb::get(ndb::models::internal.property.value)
            << ndb::source(ndb::models::internal.property)
            << ndb::filter(ndb::models::internal.property.key == id_));
            if (res.has_result())
            {
                value_ = res[0][ndb::models::internal.property.value];
            }
            else value_ = default_value_;
        }

        persistent(const std::string& name, const T& default_value = T{})
        : persistent(default_value)
        , name_{ name }
        {}

        operator T() const { return value_;}
        const T& get() const { return value_; }

        const T& default_value() const { return default_value_; }

        void operator=(T value)
        {
            // ignore update if value didn't change
            if (value_ == value) return;

            value_ = std::move(value);
            ndb::query<ndb::databases::exp_main::property_>()
            << (ndb::add_ignore
            (
            ndb::models::internal.property.value = value_
            , ndb::models::internal.property.key = id_
            ));
        }

      private:
        inline static int id = 0;

        int id_;
        std::string name_;
        T value_;
        T default_value_;
    };
}

struct nxi
{
    struct browser_
    {
        ndb::persistent<std::string, nxi> home{ "test" };

        struct
        {
            ndb::persistent<std::string> max_width;
            ndb::persistent<std::string> min_width;
        } tabs;
    } browser;
};

/*
ndb_struct(nxi,
    ndb_struct(browser,
        ndb_persist(home, "test", "desc"),
        ndb_struct(tabs,
            ndb_persist(max_width, "test", "desc")
        )
    )
)*/

int main()
{
    using ndb::sqlite;
    // using db = dbs::zeta;
    constexpr auto& library = ndb::models::library;
    constexpr auto& user = ndb::models::library.user;
    constexpr auto& movie = ndb::models::library.movie;

    std::stringstream result;

    try
    {
        ndb::initializer<sqlite> init;
        ndb::connect<db>();
        ndb::connect<ndb::databases::exp_main::property_>();

        ::nxi nxi;
        std::cout << nxi.browser.home.get();
        std::cout << "\n" << nxi.browser.tabs.max_width.get();

        nxi.browser.home = std::string("neuroshok.com");
        nxi.browser.tabs.max_width = "50";


        //nxi.browser.tabs.current_id


        //ndb::query() << (user.name, movie.name) << (user - edges.like -> movie)
        //std::cout << (ndb::query_str<db>() << (ndb::get(movie.name) << ndb::edge(user, movie)));

        //ndb::relation(user.user_id == movie.id)

        // user has one movie // ndb_movie_id
        // FROM user INNER JOIN movie ON user.movie_id = movie.id
        // LEFT JOIN music ON user_music.user_id == user.id
        // ndb_edge(user.movie_id, movie.id)
        // ndb::get(user.name, movie.name) << ndb::relation(user.movie_id == movie.id)

        // user has many movie

        // user -(z.like || z.has)-> movie

        //ndb::get(movie.name, user.name) << ndb::source(user) << ndb::edge(user, movie)
        //ndb::get(movie.name, user.name) << ndb::edge(user, movie)

        //FROM user INNER JOIN movie ON user.movie_id = movie.id

        //ndb::arc(user, movie, edges.like)


        //ndb::query<db>() << (ndb::add(movie.name = std::string("eryz"), movie.director = std::string("director")));

    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}