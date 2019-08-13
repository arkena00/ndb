#ifndef NDB_PREPROCESSOR_HPP
#define NDB_PREPROCESSOR_HPP

#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/list/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>

#include <ndb/expression.hpp>
#include <ndb/expression/basic.hpp>
#include <ndb/database.hpp>
#include <ndb/model.hpp>
#include <ndb/table.hpp>
#include <ndb/field.hpp>
#include <ndb/entity.hpp>
#include <ndb/result.hpp>
#include <ndb/object.hpp>

// operator= for fields
#define ndb_internal_field_op(NAME) \
template<class R, class E = std::enable_if_t<!std::is_same_v<std::decay_t<R>, NAME##_>>> \
constexpr const auto operator=(R&& r) const \
{ \
    auto expr_l = ndb::expr_make(*this); \
    auto expr_r = ndb::expr_make(std::forward<R>(r)); \
    return ::ndb::expression< ::ndb::expressions::assign_, decltype(expr_l), decltype(expr_r)> { std::move(expr_l), std::move(expr_r) }; \
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////           GENERATION           ////////////////////////
////////////////////////////////////////////////////////////////////////////////
// declare namespaces
namespace ndb::databases{}
namespace ndb::models{}
namespace ndb::tables{}
namespace ndb::objects{}

// TODO remove macros in 2023

// common
// unpack macro
#define ndb_internal_unpack(ITEM) BOOST_PP_CAT(ndb_internal_unpack_, ITEM)

////////////////////////////////////////////////////////////////////////////////
////////////////////////             FIELD              ////////////////////////
////////////////////////////////////////////////////////////////////////////////
// apply func on each ndb_field(...)
#define ndb_internal_for_each_fields(TABLE_NAME, FUNC,...) \
BOOST_PP_LIST_FOR_EACH_I( \
        FUNC, \
        TABLE_NAME, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
)

// make field
#define ndb_internal_unpack_ndb_field(...)  __VA_ARGS__
#define ndb_internal_unpack_ndb_option(...) __VA_ARGS__

#define ndb_internal_make_field_impl(PARENT, NAME, ...) static constexpr struct NAME##_ : \
    ::ndb::field<PARENT, __VA_ARGS__> { ndb_internal_field_op(NAME) } NAME {};
#define ndb_internal_make_field_impl_args(TABLE, PARAM) (TABLE, ndb_internal_unpack(PARAM))

#define ndb_internal_make_field_filter(PARAM, FUNCTION) BOOST_PP_CAT(ndb_internal_make_field_filter_, PARAM)(FUNCTION)
#define ndb_internal_make_field_filter_ndb_field(...) BOOST_PP_REM
#define ndb_internal_make_field_filter_ndb_option(...) BOOST_PP_TUPLE_EAT


#ifdef _MSC_VER
    #define ndb_internal_make_field(r, TABLE, i, PARAM) BOOST_PP_CAT(ndb_internal_make_field_filter(PARAM, ndb_internal_make_field_impl), (TABLE, ndb_internal_unpack(PARAM)))
#else
    #define ndb_internal_make_field(r, TABLE, i, PARAM) BOOST_PP_EXPAND( ndb_internal_make_field_filter(PARAM, ndb_internal_make_field_impl) ndb_internal_make_field_impl_args(TABLE, PARAM) )
#endif

// make object
#define ndb_internal_make_object_field_impl(TABLE_NAME, FIELD_NAME, ...) \
typename ::ndb::tables::TABLE_NAME<void>::FIELD_NAME##_::value_type FIELD_NAME;
#define ndb_internal_make_object_field_impl_args(TABLE, PARAM) (TABLE, ndb_internal_unpack(PARAM))


#ifdef _MSC_VER
    #define ndb_internal_make_object_field(r, TABLE, i, PARAM) BOOST_PP_CAT(ndb_internal_make_field_filter(PARAM, ndb_internal_make_object_field_impl), (TABLE, ndb_internal_unpack(PARAM)))
#else
    #define ndb_internal_make_object_field(r, TABLE, i, PARAM) BOOST_PP_EXPAND( ndb_internal_make_field_filter(PARAM, ndb_internal_make_object_field_impl) ndb_internal_make_object_field_impl_args(TABLE, PARAM) )
#endif


////////////////////////////////////////////////////////////////////////////////
////////////////////////             OBJECT              ////////////////////////
////////////////////////////////////////////////////////////////////////////////


// make object result encoder
#define ndb_internal_make_object_result_encoder_impl(TABLE_NAME, FIELD_NAME, ...) \
object.FIELD_NAME = line[model.TABLE_NAME.FIELD_NAME];

#define ndb_internal_make_object_member_assign_impl(TABLE_NAME, FIELD_NAME, ...) obj.FIELD_NAME = res[0][model.TABLE_NAME.FIELD_NAME];

#define ndb_internal_empty(a, b, c)
// ignore the first comma
#define ndb_internal_make_object_expr_assign_fields_impl(i, TABLE_NAME, FIELD_NAME) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 1)) model.TABLE_NAME.FIELD_NAME = obj.FIELD_NAME
// ignore the first field (oid field) in object set/add
#define ndb_internal_make_object_expr_assign_impl(i, TABLE_NAME, FIELD_NAME, ...)  BOOST_PP_IF(BOOST_PP_EQUAL(i, 0), ndb_internal_empty, ndb_internal_make_object_expr_assign_fields_impl) (i, TABLE_NAME, FIELD_NAME)

#define ndb_internal_make_object_result_encoder(r, TABLE, i, PARAM) ndb_internal_make_field_filter(PARAM, ndb_internal_make_object_result_encoder_impl) (TABLE, BOOST_PP_VARIADIC_ELEM(0, ndb_internal_unpack(PARAM)))
#define ndb_internal_make_object_member_assign(r, TABLE, i, PARAM) ndb_internal_make_field_filter(PARAM, ndb_internal_make_object_member_assign_impl) (TABLE, BOOST_PP_VARIADIC_ELEM(0, ndb_internal_unpack(PARAM)))
#define ndb_internal_make_object_expr_assign(r, TABLE, i, PARAM) ndb_internal_make_field_filter(PARAM, ndb_internal_make_object_expr_assign_impl) (i, TABLE, BOOST_PP_VARIADIC_ELEM(0, ndb_internal_unpack(PARAM)))

////////////////////////////////////////////////////////////////////////////////
////////////////////////             TABLE              ////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ndb_internal_make_option_filter(PARAM, FUNCTION) BOOST_PP_CAT(ndb_internal_make_option_filter_, PARAM)(FUNCTION)
#define ndb_internal_make_option_filter_ndb_field(...) BOOST_PP_TUPLE_EAT
#define ndb_internal_make_option_filter_ndb_option(...) BOOST_PP_REM

// table detail
#define ndb_internal_make_table_detail_entity_impl(i, PARAM) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) BOOST_PP_CAT(BOOST_PP_VARIADIC_ELEM(0, ndb_internal_unpack(PARAM)), _)
// entity
#define ndb_internal_make_table_detail_entity(r, data, i, PARAM) ndb_internal_make_field_filter(PARAM, ndb_internal_make_table_detail_entity_impl)(i, PARAM)
// option
#define ndb_internal_make_table_detail_option_impl(i, PARAM) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) ndb_internal_unpack(PARAM)
#define ndb_internal_make_table_detail_option(r, data, i, PARAM) ndb_internal_make_option_filter(PARAM, ndb_internal_make_table_detail_option_impl)(i, PARAM)

#define ndb_table(TABLE_NAME, ...) namespace ndb::objects { struct TABLE_NAME; } namespace ndb::tables { \
    template<class Model> struct TABLE_NAME : ::ndb::table<Model> { \
        ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_field, __VA_ARGS__) \
         static constexpr auto all() { return ndb::expression<ndb::expressions::table_all_, TABLE_NAME>{ TABLE_NAME{} }; }; \
     \
    using Detail_ = ::ndb::table_detail \
    < \
        ::ndb::entity< \
            ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_table_detail_entity, __VA_ARGS__) \
        >, \
        ndb::parent<void>, /* table relation */ \
        ::ndb::objects::TABLE_NAME \
        /* table option */ \
        , ::ndb::option< ::ndb::void_ ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_table_detail_option, __VA_ARGS__) > \
    >; \
}; \
} /* ndb::tables */ \
namespace ndb::objects \
{ \
    struct TABLE_NAME \
    { \
        ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_object_field, __VA_ARGS__) \
    }; \
} \
namespace ndb { \
    template<class Database> \
    struct result_encoder< ::ndb::objects::TABLE_NAME, Database > \
    { \
        static auto decode(const ::ndb::line<Database>& line) \
        { \
            constexpr typename Database::model model{}; \
            ::ndb::objects::TABLE_NAME object; \
                ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_object_result_encoder, __VA_ARGS__) \
            return object; \
        } \
    }; \
}

#define ndb_object(TABLE_NAME, ...) ndb_internal_object_impl(TABLE_NAME, ndb_field_oid, __VA_ARGS__)

#define ndb_internal_object_impl(TABLE_NAME, ...) namespace ndb::objects { struct TABLE_NAME; } namespace ndb::tables { \
    template<class Model> struct TABLE_NAME : ::ndb::table<Model> { \
        ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_field, __VA_ARGS__) \
         static constexpr auto all() { return ndb::expression<ndb::expressions::table_all_, TABLE_NAME>{ TABLE_NAME{} }; }; \
     \
    using Detail_ = ::ndb::table_detail \
    < \
        ::ndb::entity< \
            ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_table_detail_entity, __VA_ARGS__) \
        >, \
        ndb::parent<void>, /* table relation */ \
        ::ndb::objects::TABLE_NAME \
        /* table option */ \
        , ::ndb::option< ::ndb::void_ ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_table_detail_option, __VA_ARGS__) > \
    >; \
}; \
} /* ndb::tables */ \
namespace ndb::objects \
{ \
    struct TABLE_NAME \
    { \
        ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_object_field, __VA_ARGS__) \
    }; \
} \
namespace ndb { \
    template<class Engine> \
    struct result_encoder< ::ndb::objects::TABLE_NAME, Engine > \
    { \
        static auto decode(const ::ndb::line<Engine>& line) \
        { \
            ::ndb::objects::TABLE_NAME object; \
                ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_object_result_encoder, __VA_ARGS__) \
            return object; \
        } \
    }; \
} \
 /*////////////////////////////////////////// objects IO */ \
namespace ndb::internal \
{ \
    template<class Database> \
    struct object_get<Database, ndb::objects::TABLE_NAME> \
    { \
        static auto process(ndb::cpp_type_t<oid_, Database> oid, ndb::objects::TABLE_NAME& obj) \
        { \
            constexpr typename Database::model model{}; \
            auto res = ndb::query<Database>() << (ndb::get() << ndb::source(model.TABLE_NAME) << ndb::filter(model.TABLE_NAME.oid == oid)); \
            if (res.has_result()) { ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_object_member_assign, __VA_ARGS__) } \
            else ndb_error("object_get error with oid : " + std::to_string(oid)) \
            return obj; \
        } \
        static auto process(ndb::cpp_type_t<oid_, Database> oid) \
        { \
            ndb::objects::TABLE_NAME obj; \
            return process(oid, obj); \
        } \
    }; \
 \
    template<class Database> \
    struct object_set<Database, ndb::objects::TABLE_NAME> \
    { \
        static void process(ndb::objects::TABLE_NAME& obj) \
        { \
            constexpr typename Database::model model{}; \
            ndb::query<Database>() << (ndb::set( \
            ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_object_expr_assign, __VA_ARGS__) \
            ) << ndb::filter(model.TABLE_NAME.oid == obj.oid)); \
        } \
    }; \
 \
    template<class Database> \
    struct object_add<Database, ndb::objects::TABLE_NAME> \
    { \
        static auto process(ndb::objects::TABLE_NAME& obj) \
        { \
            constexpr typename Database::model model{}; \
            ndb::query<Database>() << ndb::add( ndb_internal_for_each_fields(TABLE_NAME, ndb_internal_make_object_expr_assign, __VA_ARGS__) ); \
            obj.oid = ndb::last_id<Database>(); \
            return obj.oid; \
        } \
    }; \
 \
    template<class Database> \
    struct object_del<Database, ndb::objects::TABLE_NAME> \
    { \
        static void process(ndb::objects::TABLE_NAME& obj) \
        { \
            constexpr typename Database::model model{}; \
            ndb::query<Database>() << (ndb::del << ndb::source(model.TABLE_NAME) << ndb::filter(model.TABLE_NAME.oid == obj.oid)); \
        } \
    }; \
} // ndb::internal


////////////////////////////////////////////////////////////////////////////////
////////////////////////             MODEL              ////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ndb_internal_make_model_detail_entity(r, data, i, item) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) BOOST_PP_CAT(item, _)

#define ndb_internal_make_table_impl(MODEL_NAME, TABLE_NAME) using TABLE_NAME##_ = ::ndb::tables::TABLE_NAME< ::ndb::models::MODEL_NAME##_ >; static constexpr TABLE_NAME##_ TABLE_NAME {};
#define ndb_internal_make_table(r, data, item) ndb_internal_make_table_impl(data, item)

#define ndb_model(MODEL_NAME, ...) namespace ndb::models { \
    struct MODEL_NAME##_ { \
    BOOST_PP_LIST_FOR_EACH( \
        ndb_internal_make_table, \
        MODEL_NAME, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
    ) \
    using Detail_ = ::ndb::model_detail \
    < \
        ::ndb::entity< \
            BOOST_PP_LIST_FOR_EACH_I( \
                ndb_internal_make_model_detail_entity, \
                BOOST_NIL, \
                BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
            ) \
        > \
    >; \
}; \
static constexpr const ::ndb::models::MODEL_NAME##_ MODEL_NAME = {}; \
} // ::ndb::models

////////////////////////////////////////////////////////////////////////////////
////////////////////////            DATABASE            ////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ndb_internal_make_database_detail_entity(r, data, i, item) BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0)) BOOST_PP_CAT(BOOST_PP_VARIADIC_ELEM(0, ndb_internal_unpack(item)), _)

#define ndb_internal_unpack_ndb_database(...)  __VA_ARGS__

#define ndb_internal_make_database_impl(PROJECT_NAME, NAME, MODEL_NAME, DB_ENGINE) static constexpr struct NAME##_ : ::ndb::database< ::ndb::databases::PROJECT_NAME, ::ndb::models::MODEL_NAME##_, DB_ENGINE >{} NAME{};
#define ndb_internal_make_database_impl_args(data, item) (data, item)

#define ndb_internal_make_database(r, data, item) BOOST_PP_EXPAND(ndb_internal_make_database_impl ndb_internal_make_database_impl_args(data, ndb_internal_unpack(item)) )

#define ndb_project(PROJECT_NAME, ...) namespace ndb::databases { \
    struct PROJECT_NAME { \
    static constexpr auto name = #PROJECT_NAME; \
    BOOST_PP_LIST_FOR_EACH( \
        ndb_internal_make_database, \
        PROJECT_NAME, \
        BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
    ) \
    using Detail_ = ::ndb::database_detail \
    < \
        ::ndb::entity< \
            BOOST_PP_LIST_FOR_EACH_I( \
                ndb_internal_make_database_detail_entity, \
                BOOST_NIL, \
                BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__) \
            ) \
        > \
    >; \
}; \
} // ::ndb::databases

////////////////////////////////////////////////////////////////////////////////
////////////////////////        EXPRESSION ALIASES      ////////////////////////
////////////////////////////////////////////////////////////////////////////////
//! aliases are scoped

//! alias using current line
#define ndb_alias(VAR_NAME, EXPR) auto VAR_NAME = ndb::as(std::integral_constant<unsigned int , __LINE__>{}, EXPR)

//! alias with specified index
#define ndb_aliasn(ID, VAR_NAME, EXPR) auto VAR_NAME = ndb::as(std::integral_constant<unsigned int , ID>{}, EXPR)

////////////////////////////////////////////////////////////////////////////////
////////////////////////          MACRO ALIASES         ////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define ndb_field_id ndb_field(id, ::ndb::type_get<::ndb::int64_>, ::ndb::option<ndb::field_option::oid>)
#define ndb_field_oid ndb_field(oid, ::ndb::type_get<::ndb::oid_>, ::ndb::option<ndb::field_option::oid>) // new macro for object id

#endif // NDB_PREPROCESSOR_HPP
