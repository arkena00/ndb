#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/list/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>

#define ndb_unpack(ITEM) BOOST_PP_CAT(ndb_unpack_, ITEM)
#define ndb_unpack_ndb_field(...)  __VA_ARGS__

#define ndb_make_field_impl(PARENT, NAME, TYPE, SIZE) static constexpr struct NAME##_ : ndb::field<PARENT, TYPE, ndb::size<SIZE>> { ndb_field_op } NAME {};

#define ndb_make_field_impl_args(data, item) (data, item)

#define ndb_make_field(r, data, item) BOOST_PP_EXPAND(ndb_make_field_impl ndb_make_field_impl_args(data, ndb_unpack(item)) )

#define ndb_make_entity(r, data, i, item) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) BOOST_PP_CAT(BOOST_PP_VARIADIC_ELEM(0, ndb_unpack(item)), _)

#define ndb_table(TABLE_NAME, ...) template<class Model> struct TABLE_NAME : ndb::table<Model> { \
    BOOST_PP_LIST_FOR_EACH( \
        ndb_make_field, \
        TABLE_NAME, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
    ) \
    using Detail_ = ndb::table_detail \
    < \
        ndb::entity< \
            BOOST_PP_LIST_FOR_EACH_I( \
                ndb_make_entity, \
                BOOST_NIL, \
                BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
            ) \
        > \
    >; \
};

ndb_table(movie,
          ndb_field(id, int, 8),
          ndb_field(name, std::string, 255),
          ndb_field(image, std::string, 255)
)


/*
template<class Model>
struct movie : ndb::table<Model>
{
    using id_ = fields::id<movie>; static constexpr id_ id {};
    static constexpr struct name_ : ndb::field<movie, std::string, ndb::size<255>> { ndb_field_op } name {};
    static constexpr struct image_ : ndb::field<movie, std::string, ndb::size<255>> { ndb_field_op } image {};
    static constexpr struct user_ : ndb::field<movie, tables::user> {} user {};
    using Detail_ = ndb::table_detail
    <
    ndb::entity<id_, name_, image_>
    >;
};
*/