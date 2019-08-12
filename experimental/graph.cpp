


#define NDB_DEBUG_QUERY

#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>
#include <ndb/query.hpp>
#include <ndb/statement.hpp>
#include <iostream>
#include <sstream>

#include <ndb/preprocessor.hpp>



namespace ndb::objects
{
    struct page;
}
namespace ndb::tables
{
    template<class Model>
    struct page : ::ndb::table<Model>
    {
        static constexpr struct id_
            : ::ndb::field<page, ::ndb::type_get<::ndb::int64_>, ::ndb::option<ndb::field_option::oid>>{ ndb_internal_field_op(id) } id
        {};
        static constexpr struct name_ : ::ndb::field<page, std::string>{ ndb_internal_field_op(name) } name
        {};
        static constexpr struct type_ : ::ndb::field<page, int>{ ndb_internal_field_op(type) } type
        {};
        static constexpr auto all() { return ndb::expression<ndb::expressions::table_all_, page>{ page{} }; };
        using Detail_ = ::ndb::table_detail<::ndb::entity<id_, name_, type_>, ndb::parent<void>, ::ndb::objects::page, ::ndb::option<::ndb::void_>>;
    };
} // namespace ndb::tables
namespace ndb::objects
{
    struct page
    {
        typename ::ndb::tables::page<void>::id_::value_type id;
        typename ::ndb::tables::page<void>::name_::value_type name;
        typename ::ndb::tables::page<void>::type_::value_type type;
    };
} // namespace ndb::objects
namespace ndb
{
    template<class Engine>
    struct result_encoder<::ndb::objects::page, Engine>
    {
        static auto decode(const ::ndb::line<Engine>& line)
        {
            ::ndb::objects::page object;
            object.id = line[::ndb::tables::page<void>::id];
            object.name = line[::ndb::tables::page<void>::name];
            object.type = line[::ndb::tables::page<void>::type];
            return object;
        }
    };
} // namespace ndb


namespace ndb::objects
{
    struct web_page;
}
namespace ndb::tables
{
    template<class Model>
    struct web_page : public ::ndb::tables::page<Model>
    {
        static constexpr struct page_id_ : ::ndb::field<web_page, int>{ ndb_internal_field_op(page_id) } page_id
        {};
        static constexpr struct url_ : ::ndb::field<web_page, std::string>{ ndb_internal_field_op(url) } url
        {};
        static constexpr auto all() { return ndb::expression<ndb::expressions::table_all_, web_page>{ web_page{} }; };
        using Detail_ = ::ndb::table_detail<::ndb::entity<page_id_, url_>, ndb::parent<void>, ::ndb::objects::web_page, ::ndb::option<::ndb::void_>>;
    };
} // namespace ndb::tables
namespace ndb::objects
{
    struct web_page : public page
    {
        typename ::ndb::tables::web_page<void>::page_id_::value_type page_id;
        typename ::ndb::tables::web_page<void>::url_::value_type url;
    };
} // namespace ndb::objects
namespace ndb
{
    template<class Engine>
    struct result_encoder<::ndb::objects::web_page, Engine>
    {
        static auto decode(const ::ndb::line<Engine>& line)
        {
            ::ndb::objects::web_page object;
            object.page_id = line[::ndb::tables::web_page<void>::page_id];
            object.url = line[::ndb::tables::web_page<void>::url];
            return object;
        }
    };
} // namespace ndb




// database
/*
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
)*/


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


template<class Source, class Target, class Type = void>
struct edge
{
    using source_table = typename Source::table;
    using target_table = typename Target::table;
    using source_field = Source;
    using target_field = Target;
    using type = Type;

    static constexpr Source source{};
    static constexpr Target target{};
};

template<class Model>
struct has : edge<typename ::ndb::tables::user<Model>::id_, typename ::ndb::tables::movie<Model>::id_>
{

};
//ndb_edge(has, user, movie)

ndb_table(user_movie
, ndb_field(source_id, int)
, ndb_field(target_id, int)
)

// ndb_edge(user, movie, has)
// ndb_edge_n(1, user, movie, has)

//ndb_arc(user, movie, possess)
// ndb_edge(user, user, friend_with)
// table user_user_friend_with | source_id, target_id

namespace ndb::models
{
    struct library_
    {
        using movie_ = ::ndb::tables::movie<::ndb::models::library_>;
        static constexpr movie_ movie{};
        using user_ = ::ndb::tables::user<::ndb::models::library_>;
        static constexpr user_ user{};

        using has_ = has<::ndb::models::library_>;
        static constexpr has_ has{};

        using Detail_ = ::ndb::model_detail<::ndb::entity<movie_, user_>, ::ndb::entity<has_>>;
    };
    static constexpr const ::ndb::models::library_ library = {};
} // namespace ndb::models

ndb_project(exp_graph,
            ndb_database(library, library, ndb::sqlite)
)

using db = ndb::databases::exp_graph::library_;


namespace ndb
{
    void object_add()
    {

    }
}



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
        //ndb::object_add<db>();
        //ndb::query<db>() << (ndb::add(user.name = std::string("ads")));
        //ndb::query<db>() << (ndb::add(movie.name = std::string("interstellar")));
        //ndb::add_edge<db>(1, has, 1);
        for (auto&& res : ndb::query<db>() << (ndb::get(movie.name, user.name) << ndb::arc(user, md.has, movie)))
        {
            std::cout << "\n" << res[user.name] << " " << res[movie.name];
        }

        /*
        auto qm = ndb::query<db>() << (
        ndb::get(movie.name, user.name)
        << ndb::edge(user, movie, md.user_movie.source_id == user.id)
        << ndb::edge(user, movie, md.user_movie.target_id == movie.id)
        );
        std::cout << qm[0][user.name];
        std::cout << "\n" << qm[0][movie.name];*/

        //! web_page inherit page
        /*
        ndb::query<db>() << (ndb::add(md.page.name = std::string("NeuroshoK")));
        ndb::query<db>() << (ndb::add(md.web_page.url = std::string("http://www.neuroshok.com")));
        constexpr auto inherit = md.web_page.page_id == md.page.id;
        auto q = ndb::query<db>() << (ndb::get(md.web_page.name, md.web_page.url) << ndb::edge(md.web_page, md.page, inherit));
        std::cout << q[0][md.page.name];
        std::cout << "\n" << q[0][md.web_page.url];
         */
        // SELECT * FROM web_page INNER JOIN page ON web_page.page_id = page.id



        //FROM user INNER JOIN movie ON user.movie_id = movie.id


    } catch(const std::exception& e)
    {
        std::cout << e.what();
    }

    return 0;

}
