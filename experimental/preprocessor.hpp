#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/list/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>

////////////////////////////////////////////////////////////////////////////////
////////////////////////           GENERATION           ////////////////////////
////////////////////////////////////////////////////////////////////////////////
// declare namespaces
namespace ndb::tables{}
namespace ndb::models{}
namespace ndb::databases{}


// common
// unpack macro
#define ndb_unpack(ITEM) BOOST_PP_CAT(ndb_unpack_, ITEM)


// table and fields
#define ndb_make_table_detail_entity(r, data, i, item) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) BOOST_PP_CAT(BOOST_PP_VARIADIC_ELEM(0, ndb_unpack(item)), _)

#define ndb_unpack_ndb_field(...)  __VA_ARGS__

#define ndb_make_field_impl(PARENT, NAME, TYPE, SIZE) static constexpr struct NAME##_ : ::ndb::field<PARENT, TYPE, ::ndb::size<SIZE>> { ndb_field_op } NAME {};
#define ndb_make_field_impl_args(data, item) (data, item)

#define ndb_make_field(r, data, item) BOOST_PP_EXPAND(ndb_make_field_impl ndb_make_field_impl_args(data, ndb_unpack(item)) )

#define ndb_table(TABLE_NAME, ...) namespace ndb::tables { \
    template<class Model> struct TABLE_NAME : ::ndb::table<Model> { \
    BOOST_PP_LIST_FOR_EACH( \
        ndb_make_field, \
        TABLE_NAME, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
    ) \
    using Detail_ = ::ndb::table_detail \
    < \
        ::ndb::entity< \
            BOOST_PP_LIST_FOR_EACH_I( \
                ndb_make_table_detail_entity, \
                BOOST_NIL, \
                BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
            ) \
        > \
    >; \
}; \
} // ::ndb::tables

// model
#define ndb_make_model_detail_entity(r, data, i, item) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) BOOST_PP_CAT(item, _)

#define ndb_make_table_impl(MODEL_NAME, TABLE_NAME) using TABLE_NAME##_ = ::ndb::tables::TABLE_NAME< ::ndb::models::MODEL_NAME##_ >; static constexpr TABLE_NAME##_ TABLE_NAME {};
#define ndb_make_table(r, data, item) ndb_make_table_impl(data, item)

#define ndb_model(MODEL_NAME, ...) namespace ndb::models { \
    struct MODEL_NAME##_ { \
    BOOST_PP_LIST_FOR_EACH( \
        ndb_make_table, \
        MODEL_NAME, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
    ) \
    using Detail_ = ::ndb::model_detail \
    < \
        ::ndb::entity< \
            BOOST_PP_LIST_FOR_EACH_I( \
                ndb_make_model_detail_entity, \
                BOOST_NIL, \
                BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
            ) \
        > \
    >; \
}; \
static constexpr const ::ndb::models::MODEL_NAME##_ MODEL_NAME = {}; \
} // ::ndb::models

// database
#define ndb_make_database_detail_entity(r, data, i, item) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) BOOST_PP_CAT(BOOST_PP_VARIADIC_ELEM(0, ndb_unpack(item)), _)

#define ndb_unpack_ndb_database(...)  __VA_ARGS__

#define ndb_make_database_impl(PROJECT_NAME, NAME, MODEL_NAME, DB_ENGINE) static constexpr struct NAME##_ : ::ndb::database< ::ndb::databases::PROJECT_NAME, ::ndb::models::MODEL_NAME##_, DB_ENGINE >{} NAME{};
#define ndb_make_database_impl_args(data, item) (data, item)

#define ndb_make_database(r, data, item) BOOST_PP_EXPAND(ndb_make_database_impl ndb_make_database_impl_args(data, ndb_unpack(item)) )

#define ndb_project(PROJECT_NAME, ...) namespace ndb::databases { \
    struct PROJECT_NAME { \
    static constexpr auto name = #PROJECT_NAME; \
    BOOST_PP_LIST_FOR_EACH( \
        ndb_make_database, \
        PROJECT_NAME, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
    ) \
    using Detail_ = ::ndb::database_detail \
    < \
        ::ndb::entity< \
            BOOST_PP_LIST_FOR_EACH_I( \
                ndb_make_database_detail_entity, \
                BOOST_NIL, \
                BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
            ) \
        > \
    >; \
}; \
} // ::ndb::databases



ndb_table(movie,
          ndb_field(id, int, 8),
          ndb_field(name, std::string, 255),
          ndb_field(image, std::string, 255)
)

ndb_table(music,
          ndb_field(id, int, 8)
)

ndb_model(library, movie, music);

ndb_project(nxi,
    ndb_database(alpha, library, ndb::sqlite),
    ndb_database(zeta, library, ndb::mongo)
)

