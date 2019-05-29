    namespace ndb::internal
    {
        template<class Database, class T>
        struct object_get;
    } // internal

#include <ndb/initializer.hpp>
#include <ndb/engine/sqlite/sqlite.hpp> // engine
#include <ndb/preprocessor.hpp> // database macros
#include <ndb/function.hpp> // ndb::clear
#include <ndb/query.hpp> // query and expression
#include <ndb/object.hpp>

struct page { page(std::string n) : name{n}{} std::string name; virtual std::string info() {  return name + "\\n"; } public: page() = default; };
struct web_page : public page
{
    using page::page;
    std::string url;
    std::string info() override { return name + "\\n" + url; }

  public:
    web_page() = default;
};
struct explorer_page : public page { using page::page; std::string path; std::string info() override {  return name + "\\n" + path; } };

enum class page_type {web, explorer};

// ndb type_maps
namespace ndb
{
    ndb_type_map(page_type, int64_, scope::global);
}

// database
ndb_table(
    page
    , ndb_field_id
    , ndb_field(name, std::string)
    , ndb_field(type, page_type)
)


ndb_table(
    web_page
    , ndb_field(page_id, int)
    , ndb_field(url, std::string)
)

ndb_table(
    page_edge
    , ndb_field(source_id, int)
    , ndb_field(target_id, int)
)

ndb_table(movie,
  ndb_field_id,
  ndb_field(name, std::string, ndb::size<255>)
)

ndb_model(graph,
movie,
page,
web_page,
page_edge)

ndb_project(orm,
            ndb_database(graph, graph, ndb::sqlite)
)

// alias
namespace dbs
{
    using graph = ndb::databases::orm::graph_;
}





namespace ndb
{
    namespace internal
    {
        template<class Database>
        struct object_get<Database, ndb::objects::movie>
        {
            static auto process(int oid)
            {
                constexpr typename Database::model model{};
                auto res = ndb::query<Database>() << (ndb::get(model.movie.name) << ndb::source(model.movie) << ndb::filter(model.movie.id == oid));

                ndb::objects::movie obj;
                if (res.has_result())
                {
                    obj.name = res[0][model.movie.name];
                }
                return obj;
            }

            static auto process(int oid, ndb::objects::movie& obj)
            {
                constexpr typename Database::model model{};
                auto res = ndb::query<Database>() << (ndb::get() << ndb::source(model.movie) << ndb::filter(model.movie.id == oid));
                if (res.has_result())
                {
                    obj.id = res[0][model.movie.id];
                    obj.name = res[0][model.movie.name];
                }
                else ndb_error("object_get error");
                return obj;
            }
        };
    } // internal


    template<class Database>
    void object_add(const ndb::objects::movie& m)
    {
        constexpr typename Database::model model{};
        ndb::query<Database>() << ndb::add(model.movie.name = m.name);
        auto pid = ndb::last_id<Database>();
    }


/*
    template<class Database>
    void object_add(const page& wp)
    {
        const auto& g = ndb::models::graph;
        ndb::query<Database>() << ndb::add(g.page.name = wp.name, g.page.type = page_type::web);
        auto pid = ndb::last_id<Database>();
    }

    template<class Database>
    void object_add(const web_page& wp)
    {
        const auto& g = ndb::models::graph;
        ndb::query<Database>() << ndb::add(g.page.name = wp.name, g.page.type = page_type::web);
        auto pid = ndb::last_id<Database>();
        ndb::query<Database>() << ndb::add(g.web_page.page_id = pid, g.web_page.url = wp.url);
    }

    template<class Database, class T>
    auto object_get(int oid)
    {
        const auto& g = ndb::models::graph;
        auto res = ndb::query<Database>() << (ndb::get(g.page.name) << ndb::source(g.page) << ndb::filter(g.page.id == oid));
        auto res2 = ndb::query<Database>() << (ndb::get(g.web_page.url) << ndb::source(g.web_page) << ndb::filter(g.web_page.page_id == oid));
        web_page obj;
        if (res.has_result())
        {
            obj.name = res[0][g.page.name];
            obj.url = res2[0][g.web_page.url];
        }
        return obj;
    }*/


} // ndb

struct movie : ndb::object<dbs::graph, ndb::objects::movie>
{
    movie(std::string display_mode){}

    void display()
    {
        std::cout << id << "_" << name;
    }
};

int main()
{
    using namespace std::chrono_literals;
    const auto& p = ndb::models::graph.page;

    try
    {
        ndb::initializer<ndb::sqlite> init;
        ndb::connect<dbs::graph>();

        ndb::objects::movie m;
        m.name = "stargate";
        //ndb::object_add<dbs::graph>(m);
        //auto movie_data = ndb::object_get<dbs::graph, ndb::objects::movie>(2);

        auto movie = ndb::make<::movie>(2, "mode");
        movie.display();
        std::cout << ndb::is_loaded(movie);

/*
        web_page wp{"aze"};
        wp.name = "neuroshok.com";
        wp.url = "http://www.nk.com";

        //ndb::object_add<dbs::graph>(wp);
        web_page w = ndb::object_get<dbs::graph, web_page>( 2 );
        std::cout << w.name << " " << w.url;
*/


    }
    catch (const std::exception& e) { std::cout << e.what(); }

    return 0;
}