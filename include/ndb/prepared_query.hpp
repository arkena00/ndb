#ifndef PREPARED_QUERY_H_NDB
#define PREPARED_QUERY_H_NDB

#include <ndb/engine/native_query.hpp>
#include <ndb/statement.hpp>

#include <string>
#include <unordered_map>

//! prepare macros, Database type must be defined
//! require one macro per line to enable overloads
//! usage : ndb_prepare(dbs::library, get_movie, movie.duration) { ndb_pquery << (ndb::get() << ndb::source(movie) << ndb::filter(movie.duration == _0)); }

//! empty packs size returns 1 so we add a parameter and compare the pack size to 1
#define ndb_internal_make_prepared_loop_void(...)

#define ndb_internal_make_prepared_loop_extract(INDEX_0, ...) __VA_ARGS__

#define ndb_internal_make_prepared_loop(FUNCTION, FUNC, ...)  \
    BOOST_PP_LIST_FOR_EACH_I( \
            FUNC, \
            FUNCTION, \
            BOOST_PP_VARIADIC_TO_LIST(BOOST_PP_EXPAND(ndb_internal_make_prepared_loop_extract (__VA_ARGS__))) \
    )

#define ndb_internal_prepared_loop(FUNCTION, FUNC, ...) \
BOOST_PP_CAT(ndb_internal_make_prepared_loop, BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 1), _void, BOOST_PP_EMPTY())) (FUNCTION, FUNC, __VA_ARGS__)

#define ndb_internal_make_prepared_placeholder(r, FUNCTION, i, FIELD) [[maybe_unused]] decltype(FIELD)::value_type _##i{};
#define ndb_internal_make_prepared_params(r, FUNCTION, i, FIELD) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) const typename decltype(FIELD)::value_type& p##i = 1
#define ndb_internal_make_prepared_bind(r, FUNCTION, i, FIELD) q.bind(p##i);
#define ndb_pquery [[maybe_unused]] auto ndb_unused = query()


#define ndb_prepare(DATABASE, FUNCTION, ...) ndb_prepare_impl(BOOST_PP_CAT(FUNCTION, __LINE__), DATABASE, FUNCTION, INDEX_0, __VA_ARGS__)

#define ndb_prepare_impl(ID, DATABASE, FUNCTION, ...) \
auto FUNCTION( ndb_internal_prepared_loop(FUNCTION, ndb_internal_make_prepared_params, __VA_ARGS__) ) \
{ \
    auto& q = ndb::prepared_query<DATABASE>::get( BOOST_PP_STRINGIZE(ID) ); \
    ndb_internal_prepared_loop(FUNCTION, ndb_internal_make_prepared_bind, __VA_ARGS__) \
 \
    return q.exec(); \
} \
namespace internal::ID { \
ndb_internal_prepared_loop(FUNCTION, ndb_internal_make_prepared_placeholder, __VA_ARGS__) \
auto query = []() noexcept { return ndb::prepare_proxy<DATABASE>{ BOOST_PP_STRINGIZE(ID) }; }; \
} namespace internal::ID

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
        constexpr explicit prepare_proxy(const char* name) : name_{ name } {}

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
