#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp>
#include <ndb/preprocessor.hpp>
#include <ndb/function.hpp>
#include <ndb/query.hpp>

/// cf. example/basic.cpp

ndb_object(movie,
          ndb_field(name, std::string, ndb::size<255>),
          ndb_field(duration, std::chrono::duration<double, std::ratio<3600>>)
)
/// ndb_object generate the code
/*
namespace ndb::objects
{
    struct movie
    {
        ndb::type_get<::ndb::oid_> oid; /// defined by ndb_internal_type_map(oid_, int64_t); (cf. example/type.cpp)
        std::string name;
        std::chrono::duration<double, std::ratio<3600>> duration;
    };
} // ndb::objects
*/


ndb_model(library, movie)

ndb_project(example_object,
            ndb_database(library, library, ndb::sqlite)
)

// alias
namespace dbs
{
    using library = ndb::databases::example_object::library_;
}

/// user type movie inherit from ndb::object using dbs::library database and ndb::objects::movie data
/// ndb::objects::movie data are accessible according to inheritance visibility (oid, name and duration)
class movie : public ndb::object<dbs::library, ndb::objects::movie>
{
    ndb_access; // give access to ndb

public:
    // custom constructor
    movie(std::string image_path) {}

    void display()
    {
        std::cout << oid << " | " << name << " " << duration.count() << " state : " << (int)ndb::object_state(*this) << std::endl;
    }

    void edit()
    {
        // edit ndb::objects::movie data
        name += "_edited";
    }
};

/// functions used for database objects IO operations : load, unload, store
/// all those functions throw exceptions on errors and are immediate

/// ndb::object_state(T&) is used to know the state in database of the object

int main()
{
    using namespace std::chrono_literals;
    const auto& library = ndb::models::library;

    try
    {
        ndb::initializer<ndb::sqlite> init;
        ndb::connect<dbs::library>();
        ndb::clear<dbs::library>(library.movie);

        /// create a movie object with uninitialized object_state (interstellar.oid is invalid)
        ::movie interstellar{ "img_path" };
        interstellar.name = "Interstellar";
        interstellar.duration = 2.49h;
        // store the object into the database (interstellar.oid is valid)
        ndb::store(interstellar); // object_state : stored

        /// create a movie object with uninitialized object_state (movie.oid is invalid)
        ::movie movie{"img_path"};
        // load data from database
        ndb::load(movie, interstellar.oid); // object_state : loaded
        movie.display();
        movie.edit();
        // movie is loaded, store will update movie data into the database
        ndb::store(movie); // object_state : updated

        /// make an updated_movie object (create instance then ndb::load the instance)
        auto updated_movie = ndb::make<::movie>(interstellar.oid, "img_path");
        updated_movie.display();

        /// delete interstellar data from database, object remains valid but state is set to unloaded
        ndb::unload(interstellar); // object state : unloaded
        interstellar.display();

        /// exception : object with object_id interstellar.oid does not exist
        // auto unloaded_movie = ndb::make<::movie>(interstellar.id, "params");

        /// /!\  if multiple objects share the same oid, only the object making the last operation will have a valid state

        /// bulk operation
        std::cout << std::endl;
        ndb::query<dbs::library>() << ndb::add(library.movie.name = std::string("movie1"), library.movie.duration = 2.1h);
        ndb::query<dbs::library>() << ndb::add(library.movie.name = std::string("movie2"), library.movie.duration = 2.2h);
        ndb::query<dbs::library>() << ndb::add(library.movie.name = std::string("movie3"), library.movie.duration = 2.3h);

        std::vector<::movie> movies;
        ndb::load_bulk(movies, "ctor_param_for_all_records");
        for (auto& m : movies) m.display();
    }
    catch (const std::exception& e) { std::cout << e.what(); }

    return 0;
}