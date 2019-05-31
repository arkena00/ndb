#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp> // engine
#include <ndb/preprocessor.hpp> // database macros
#include <ndb/function.hpp> // ndb::clear
#include <ndb/query.hpp> // query and expression

// oid field is generated
ndb_object(movie,
          ndb_field(name, std::string, ndb::size<255>),
          ndb_field(duration, std::chrono::duration<double, std::ratio<3600>>)
)

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
/// ndb::objects::movie data are accessible according to inheritance visibility (id, name and duration)
class movie : public ndb::object<dbs::library, ndb::objects::movie>
{
    ndb_access; // give access to ndb

public:
    movie(std::string display_mode){}

    void display()
    {
        std::cout << oid << " | " << name << " " << duration.count() << " state : " << (int)ndb::object_state(*this) << std::endl;
    }

    void edit()
    {
        name += "_edited";
    }
};

int main()
{
    using namespace std::chrono_literals;
    const auto& library = ndb::models::library;

    try
    {
        ndb::initializer<ndb::sqlite> init;
        ndb::connect<dbs::library>();
        ndb::clear<dbs::library>(library.movie);

        ::movie interstellar{ "params" }; // object state : uninitialized
        interstellar.name = "Interstellar";
        interstellar.duration = 2.49h;
        ndb::store(interstellar); // object state : stored

        ::movie movie{"params"};
        ndb::load(movie, interstellar.oid); // object state : loaded
        movie.display();
        movie.edit();
        ndb::store(movie); // object state : updated

        auto edited_movie = ndb::make<::movie>(movie.oid, "params"); // make ::movie and load it
        edited_movie.display();

        /// unload the object from database, object remains valid but state is set to unloaded
        ndb::unload(interstellar); // object state : unloaded
        interstellar.display();

        /// error : object with object_id intersteallar.id does not exist
        // auto unloaded_movie = ndb::make<::movie>(interstellar.id, "params");
    }
    catch (const std::exception& e) { std::cout << e.what(); }

    return 0;
}