#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp> // engine
#include <ndb/preprocessor.hpp> // database macros
#include <ndb/function.hpp> // ndb::clear
#include <ndb/query.hpp> // query and expression
#include <ndb/prepared_query.hpp>

// database
ndb_table(movie,
          ndb_field_id,
          ndb_field(name, std::string, ndb::size<255>),
          ndb_field(duration, std::chrono::duration<double, std::ratio<3600>>)
)
ndb_table(music,
          ndb_field(id, int),
          ndb_field(image, std::string)
)
ndb_model(library, movie, music)

ndb_project(my_project,
            ndb_database(library, library, ndb::sqlite)
)

// alias
namespace dbs
{
    using library = ndb::databases::my_project::library_;
}

using namespace std::chrono_literals;



namespace queries
{
    const auto& movie = ndb::models::library.movie;

    ndb_prepare(dbs::library, get_movie, movie.duration) { ndb_pquery << (ndb::get() << ndb::source(movie) << ndb::filter(movie.duration == _0)); }
    ndb_prepare(dbs::library, get_movie, movie.name) { ndb_pquery << (ndb::get() << ndb::source(movie) << ndb::filter(movie.name == _0)); }
    ndb_prepare(dbs::library, get_movie, movie.id, movie.name) { ndb_pquery << (ndb::get() << ndb::source(movie) << ndb::filter(movie.name == _1 && movie.id == _0)); }
}

/*
namespace queries
{
    template<class... Ts>
    auto get_movie(const Ts&... ts)
    {
        auto& q = ndb::prepared_query<dbs::library>::get("get_movie");
        (q.bind(ts), ...);
        return q.exec();
    }
    namespace internal::get_movie
    {
        constexpr dbs::library::model model{};
        ndb_internal_for_each_fields(get_movie, ndb_internal_make_prepared_placeholder, model.movie.id, model.movie.duration)

        auto query = [](){ return ndb::prepare_proxy<dbs::library>{ "get_movie" }; };
    } // namespace
    namespace internal::get_movie
    {
        auto z = query() << (ndb::get() << ndb::source(model.movie) << ndb::filter(model.movie.duration == _0 || model.movie.name == _1));
    }
}*/


int main()
{
    const auto& movie = ndb::models::library.movie;

    try
    {
        ndb::initializer<ndb::sqlite> init;
        ndb::connect<dbs::library>();
        ndb::clear<dbs::library>(movie);

        ndb::query<dbs::library>() + (movie.name = std::string("Interstellar"), movie.duration = 2.49h);
        ndb::query<dbs::library>() + (movie.name = std::string("Watchmen"), movie.duration = 3.30h);

        for (auto& line : queries::get_movie(2.49h))
        {
            std::cout << "movie.id : " << line[movie.id] << std::endl;
            std::cout << "movie.name : " << line[movie.name] << std::endl;
            std::cout << "movie.duration : " << line[movie.duration].count() << " Hours" << std::endl;
        }


    }
    catch (const std::exception& e) { std::cout << e.what(); }

    return 0;
}







