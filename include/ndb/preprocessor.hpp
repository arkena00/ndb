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
#include <ndb/database.hpp>
#include <ndb/model.hpp>
#include <ndb/table.hpp>
#include <ndb/field.hpp>
#include <ndb/entity.hpp>
#include <ndb/expression.hpp>

/*
#define ndb_field(TABLE, FIELD, TYPE, SIZE) \
struct FIELD##_  : ndb::field<TABLE, TYPE, ndb::size<SIZE>> { }; static constexpr const FIELD##_ FIELD {};
*/

#define ndb_field_op \
template<class R> \
constexpr const auto operator=(const R& rhs) const \
{ \
    using L = std::decay_t<decltype(*this)>; \
    constexpr auto lhs = ndb::expression<L, ndb::expr_type_code::field, void> {}; \
    auto expr_value = ndb::expression<R, ndb::expr_type_code::value, void> { rhs }; \
    return ndb::expression<decltype(lhs), ndb::expr_type_code::op_assign, decltype(expr_value), ndb::expr_clause_code::set> { lhs, std::move(expr_value) }; \
}



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

#endif // NDB_PREPROCESSOR_HPP
