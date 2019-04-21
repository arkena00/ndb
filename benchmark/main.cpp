#include "ndb_pch.hpp"


// database
namespace ndb::databases{}
namespace ndb::models{}
namespace ndb::tables{}
namespace ndb::objects{}
namespace ndb::objects { struct movie; } namespace ndb::tables { template<class Model> struct movie : ::ndb::table<Model> { static constexpr struct id_ : ::ndb::field<movie, ::ndb::type_get< ::ndb::int64_>, ::ndb::option<ndb::field_option::oid> > { ndb_internal_field_op(id) } id {}; static constexpr struct name_ : ::ndb::field<movie, std::string, ndb::size<255> > { ndb_internal_field_op(name) } name {}; static constexpr struct duration_ : ::ndb::field<movie, std::chrono::duration<double, std::ratio<3600>>> { ndb_internal_field_op(duration) } duration {}; static constexpr auto all() { return ndb::expression<ndb::expressions::table_all_, movie>{ movie{} }; }; using Detail_ = ::ndb::table_detail < ::ndb::entity< id_ , name_ , duration_ >, ndb::parent<void>, ::ndb::objects::movie , ::ndb::option< ::ndb::void_ > >; }; } namespace ndb::objects { struct movie { typename ::ndb::tables::movie<void>::id_::value_type id; typename ::ndb::tables::movie<void>::name_::value_type name; typename ::ndb::tables::movie<void>::duration_::value_type duration; }; } namespace ndb { template<class Engine> struct result_encoder< ::ndb::objects::movie, Engine > { static auto decode(const ::ndb::line<Engine>& line) { ::ndb::objects::movie object; object.id = line[::ndb::tables::movie<void>::id]; object.name = line[::ndb::tables::movie<void>::name]; object.duration = line[::ndb::tables::movie<void>::duration]; return object; } }; }
namespace ndb::objects { struct music; } namespace ndb::tables { template<class Model> struct music : ::ndb::table<Model> { static constexpr struct id_ : ::ndb::field<music, int> { ndb_internal_field_op(id) } id {}; static constexpr struct image_ : ::ndb::field<music, std::string> { ndb_internal_field_op(image) } image {}; static constexpr auto all() { return ndb::expression<ndb::expressions::table_all_, music>{ music{} }; }; using Detail_ = ::ndb::table_detail < ::ndb::entity< id_ , image_ >, ndb::parent<void>, ::ndb::objects::music , ::ndb::option< ::ndb::void_ > >; }; } namespace ndb::objects { struct music { typename ::ndb::tables::music<void>::id_::value_type id; typename ::ndb::tables::music<void>::image_::value_type image; }; } namespace ndb { template<class Engine> struct result_encoder< ::ndb::objects::music, Engine > { static auto decode(const ::ndb::line<Engine>& line) { ::ndb::objects::music object; object.id = line[::ndb::tables::music<void>::id]; object.image = line[::ndb::tables::music<void>::image]; return object; } }; }
namespace ndb::models { struct library_ { using movie_ = ::ndb::tables::movie< ::ndb::models::library_ >; static constexpr movie_ movie {}; using music_ = ::ndb::tables::music< ::ndb::models::library_ >; static constexpr music_ music {}; using Detail_ = ::ndb::model_detail < ::ndb::entity< movie_ , music_ > >; }; static constexpr const ::ndb::models::library_ library = {}; }

namespace ndb::databases { struct my_project { static constexpr auto name = "my_project"; static constexpr struct library_ : ::ndb::database< ::ndb::databases::my_project, ::ndb::models::library_, ndb::sqlite >{} library{}; using Detail_ = ::ndb::database_detail < ::ndb::entity< library_ > >; }; }


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

        auto result = ndb::query<dbs::library>() << (ndb::get() << ndb::source(movie) << ndb::filter(movie.duration == 3.30h));


        for (auto&& line : result)
        {
            std::cout << "movie.id : " << line[movie.id] << std::endl;
            std::cout << "movie.name : " << line[movie.name] << std::endl;
            std::cout << "movie.duration : " << line[movie.duration].count() << " Hours" << std::endl;
        }
    }
    catch (const std::exception& e) { std::cout << e.what(); }


    return 0;
}