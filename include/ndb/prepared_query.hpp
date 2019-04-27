#ifndef PREPARED_QUERY_H_NDB
#define PREPARED_QUERY_H_NDB

#include <ndb/engine/native_query.hpp>
#include <ndb/statement.hpp>

#include <string>
#include <unordered_map>

// TODO: add compile time type check on function parameters
//! prepare macros, Database type must be defined
//! usage : ndb_prepare(get_movie) << (ndb::get() << ndb::source(movie) << ndb::filter(movie.duration == _));
#define ndb_prepare(FUNCTION) \
template<class... Ts> \
auto FUNCTION(const Ts&... ts) \
{ \
    auto& q = ndb::prepared_query<Database>::get(#FUNCTION); \
    (q.bind(ts), ...); \
 \
    return q.exec(); \
} \
namespace internal { extern int FUNCTION##_init; } \
int internal::FUNCTION##_init = ndb::prepare_proxy<Database>{#FUNCTION}


namespace ndb
{
    template<class Database>
    class prepared_query
    {
    public:
        using Engine = typename Database::engine;
        using statement_type = typename Engine::statement_type;
        using native_query_type = native_query<Database, Engine>;

        static native_query_type& get(const std::string& id)
        {
            return queries_[query_indexes()[id]];
        }

        static void add_statement(const std::string& id, statement_type statement)
        {
            query_indexes()[id] = statements().size();
            statements().emplace_back(std::move(statement));
        }

        static const statement_type& get_statement(const std::string& id)
        {
            return statements(query_indexes()[id]);
        }

        //! make prepared queries from statements on first connection
        static void make()
        {
            // return if queries are already prepared
            if (queries_.size() == statements().size()) return;

            for (const auto& native_statement : statements())
            {
                queries_.emplace_back(native_query<Database>{ native_statement });
            }
        }

        //! define statics  before usage
        static std::vector<statement_type>& statements()
        {
            static std::vector<statement_type> v;
            return v;
        }

        //! define statics  before usage
        static std::unordered_map<std::string, std::size_t>& query_indexes()
        {
            static std::unordered_map<std::string, std::size_t> v;
            return v;
        }

    private:
        static inline std::vector<native_query_type> queries_;
    };

    //! utility class to construct prepared statements
    template<class Database>
    struct prepare_proxy
    {
        constexpr prepare_proxy(const char* name) : name_{ name } {}

        template<class E>
        int operator<<(E&& e)
        {
            auto expr = ndb::statement << e;
            constexpr auto str_query = ndb::native_expression<decltype(expr), typename Database::engine>{};
            using native_statement_type = typename Database::engine::statement_type;
            native_statement_type native_statement{ str_query.data(), str_query.size() };
            ndb::prepared_query<Database>::add_statement(name_, native_statement);
            return 0;
        }

        const char* name_;
    };
} // ndb

#endif // PREPARED_QUERY_H_NDB
