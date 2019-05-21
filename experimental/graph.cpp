


#define NDB_DEBUG_QUERY

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


// database
ndb_table(
    page
    , ndb_field_id
    , ndb_field(name, std::string)
    , ndb_field(type, int)
)

ndb_table(
    web_page
    , ndb_field(page_id, int)
    , ndb_field(url, std::string)
)


ndb_table(movie
, ndb_field_id
, ndb_field(name, std::string, ndb::size<255>)
, ndb_field(director, std::string, ndb::size<255>)
)
ndb_table(user
, ndb_field_id
, ndb_field(name, std::string, ndb::size<255>)
, ndb_field(image, std::string)
// ndb_field(cat, ndb::tables::cat<>)
)

//ndb_arc(user, movie, possess)
// ndb_edge(user, user, friend_with)
// table user_user_friend_with | source_id, target_id

ndb_model(library, movie, user, page, web_page)

ndb_project(exp_graph,
            ndb_database(library, library, ndb::sqlite)
)

using db = ndb::databases::exp_graph::library_;

int main()
{
    using ndb::sqlite;
    // using db = dbs::zeta;
    constexpr auto& md = ndb::models::library;
    constexpr auto& user = ndb::models::library.user;
    constexpr auto& movie = ndb::models::library.movie;

    std::stringstream result;

    try
    {
        ndb::initializer<sqlite> init;
        ndb::connect<db>();


        //ndb::relation(user.user_id == movie.id)

        //! user has one movie // ndb_movie_id
        // FROM user INNER JOIN movie ON user.movie_id = movie.id
        // LEFT JOIN music ON user_music.user_id == user.id
        // ndb_edge(user.movie_id, movie.id)
        // ndb::get(user.name, movie.name) << ndb::relation(user.movie_id == movie.id)

        //! user has many movies
        //ndb::get(movie.name, user.name) << ndb::source(user) << ndb::edge(user, movie, edges.has)
        //ndb::get(movie.name, user.name) << ndb::edge(user, movie)
        // SELECT * FROM user LEFT JOIN

        //! web_page inherit page
        constexpr auto inherit = md.web_page.page_id == md.page.id;
        auto q = ndb::query<db>() << (ndb::get(md.page.name, md.web_page.url) << ndb::edge(md.web_page, md.page, inherit));
        std::cout << q[0][md.page.name];
        std::cout << "\n" << q[0][md.web_page.url];
        // SELECT * FROM web_page INNER JOIN page ON web_page.page_id = page.id

        //FROM user INNER JOIN movie ON user.movie_id = movie.id


    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}