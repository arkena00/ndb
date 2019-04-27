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

const auto& movie = ndb::models::library.movie;
using namespace std::chrono_literals;



namespace queries
{
    using Database = dbs::library;

}

/*
namespace queries
{
    using Database = dbs::library;

    auto get_movie(decltype(movie.duration)::value_type t)
    {
        auto& q = ndb::prepared_query<Database>::get("get_movie");
        q.bind(t);

        return q.exec();
    }

    namespace internal { extern int get_movie_init; }
    int internal::get_movie_init = ndb::prepare_proxy<Database>{"get_movie"} << (ndb::get() << ndb::source(movie) << ndb::filter(movie.duration == decltype(movie.duration)::value_type{}));
}
*/

int main()
{
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







