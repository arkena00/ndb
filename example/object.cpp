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
    }
    catch (const std::exception& e) { std::cout << e.what(); }

    return 0;
}


namespace ndb::objects
{
    struct movie_obj;
}
namespace ndb::tables
{
    template<class Model>
    struct movie_obj : ::ndb::table<Model>
    {
        static constexpr struct oid_
        : ::ndb::field<movie_obj, ::ndb::type_get<::ndb::oid_>, ::ndb::option<ndb::field_option::oid>>
        {
            ndb_internal_field_op(oid)
        } oid{};

        static constexpr struct name_ : ::ndb::field<movie_obj, std::string>
        {
            ndb_internal_field_op(name)
        } name{};

        static constexpr struct duration_ : ::ndb::field<movie_obj, int>
        {
            ndb_internal_field_op(duration)
        } duration{};

        static constexpr auto all()
        {
            return ndb::expression<ndb::expressions::table_all_, movie_obj>{ movie_obj{}};
        }; using Detail_ = ::ndb::table_detail<::ndb::entity<oid_, name_, duration_>, ndb::parent<void>, ::ndb::objects::movie_obj, ::ndb::option<::ndb::void_, ndb::table_option::unique<name_> > >;
    };
}
namespace ndb::objects
{
    struct movie_obj
    {
        typename ::ndb::tables::movie_obj<void>::oid_::value_type      oid;
        typename ::ndb::tables::movie_obj<void>::name_::value_type     name;
        typename ::ndb::tables::movie_obj<void>::duration_::value_type duration;
    };
}
namespace ndb
{
    template<class Engine>
    struct result_encoder<::ndb::objects::movie_obj, Engine>
    {
        static auto decode(const ::ndb::line<Engine>& line)
        {
            ::ndb::objects::movie_obj object;
            object.oid = line[model.movie_obj.oid];
            object.name     = line[model.movie_obj.name];
            object.duration = line[model.movie_obj.duration];
            return object;
        }
    };
}
namespace ndb::internal
{
    template<class Database>
    struct object_get<Database, ndb::objects::movie_obj>
    {
        static auto process(ndb::cpp_type_t<oid_, Database> oid, ndb::objects::movie_obj& obj)
        {
            constexpr typename Database::model model{};
            auto                               res = ndb::query<Database>()
            << (ndb::get() << ndb::source(model.movie_obj) << ndb::filter(model.movie_obj.oid == oid));
            if (res.has_result())
            { obj.oid        = res[0][model.movie_obj.oid];
                obj.name     = res[0][model.movie_obj.name];
                obj.duration = res[0][model.movie_obj.duration];
            }
            else
                ndb_error("object_get error with oid : " + std::to_string(oid))
            return obj;
        }

        static auto process(ndb::cpp_type_t<oid_, Database> oid)
        {
            ndb::objects::movie_obj obj;
            return process(oid, obj);
        }
    };

    template<class Database>
    struct object_set<Database, ndb::objects::movie_obj>
    {
        static void process(ndb::objects::movie_obj& obj)
        {
            constexpr typename Database::model model{};
            ndb::query<Database>()
            << (ndb::set(model.movie_obj.name = obj.name, model.movie_obj.duration = obj.duration)
            << ndb::filter(model.movie_obj.oid == obj.oid));
        }
    };

    template<class Database>
    struct object_add<Database, ndb::objects::movie_obj>
    {
        static auto process(ndb::objects::movie_obj& obj)
        {
            constexpr typename Database::model model{};
            ndb::query<Database>()
            << ndb::add(model.movie_obj.name = obj.name, model.movie_obj.duration = obj.duration);
            obj.oid                          = ndb::last_id<Database>();
            return obj.oid;
        }
    };

    template<class Database>
    struct object_del<Database, ndb::objects::movie_obj>
    {
        static void process(ndb::objects::movie_obj& obj)
        {
            constexpr typename Database::model model{};
            ndb::query<Database>()
            << (ndb::del << ndb::source(model.movie_obj) << ndb::filter(model.movie_obj.oid == obj.oid));
        }
    };
}
#line 322 "D:/project/nk/ndb/experimental/preprocessor.hpp"