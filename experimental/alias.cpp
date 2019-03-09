#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp> // engine
#include <ndb/preprocessor.hpp> // database macros
#include <ndb/function.hpp> // ndb::clear
#include <ndb/query.hpp> // query and expression

#define NDB_QUERY_DEBUG

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

int main()
{
    using namespace std::chrono_literals;
    const auto& movie = ndb::models::library.movie;

    try
    {
        ndb::initializer<ndb::sqlite> init;
        ndb::connect<dbs::library>();
        ndb::clear<dbs::library>(movie);

        //! add records
        ndb::query<dbs::library>() + (movie.name = std::string("Interstellar"), movie.duration = 2.49h);
        ndb::query<dbs::library>() + (movie.name = std::string("Watchmen"), movie.duration = 3.30h);

/*
        constexpr const struct scoped_alias_
        {
            enum class alias_id : unsigned int { count_id, count_name };

            decltype(ndb::as(std::integral_constant<unsigned int, (unsigned int)alias_id::count_id>{}, ndb::count(movie.id))) count_id;
            decltype(ndb::as(std::integral_constant<unsigned int, (unsigned int)alias_id::count_name>{}, ndb::count(movie.name))) count_name;

            constexpr scoped_alias_() :
                count_id{ ndb::as(std::integral_constant<unsigned int, (unsigned int)alias_id::count_id>{}, ndb::count(movie.id)) }
                , count_name{ ndb::as(std::integral_constant<unsigned int, (unsigned int)alias_id::count_name>{}, ndb::count(movie.name)) }
            {}
        } alias;*/

        auto count_id = ndb::as(std::integral_constant<unsigned int , 0>{}, ndb::count(movie.id));
        auto count_name = ndb::as(std::integral_constant<unsigned int, 1>{}, ndb::count(movie.name));

        //ndb_aliasn(0, count_id, ndb::count(movie.id));
        //ndb_aliasn(1, count_id, ndb::count(movie.id));
        // ndb_alias(name, expr, name expr)

        auto q = (
        ndb::get(
            movie.id, movie.name, movie.duration
            , count_id
            , count_name)
            << ndb::source(movie)
        );
        auto result = ndb::query<dbs::library>() << q;

        std::cout << (ndb::query_str<dbs::library>() << q) << std::endl;

        for (auto&& line : result)
        {
            std::cout << "movie.id : " << line[movie.id] << std::endl;
            std::cout << "movie.name : " << line[movie.name] << std::endl;
            std::cout << "movie.duration : " << line[movie.duration].count() << " Hours" << std::endl;
            std::cout << "alias.count : " << line[count_name] << " Count" << std::endl;
            //std::cout << "alias.count_2 : " << line[alias.count_2] << " Count" << std::endl;
        }
    }
    catch (const std::exception& e) { std::cout << e.what(); }

    return 0;
}