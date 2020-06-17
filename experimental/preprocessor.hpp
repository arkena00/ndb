#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/list/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>
#include <boost/preprocessor/stringize.hpp>


template<class T>
struct constant
{
        constant(T&&) {}
    using value_type = T;
};

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
#define ndb_internal_make_prepared_params(r, FUNCTION, i, FIELD) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) const typename decltype(FIELD)::value_type& p##i = {}
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


ndb_prepare(dbs::library, get_movies, constant<int>(1)) { ndb_pquery << (ndb::get() << ndb::source(movie) << ndb::filter(movie.id == _0)); }