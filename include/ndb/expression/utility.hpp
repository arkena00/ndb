#ifndef EXPRESSION_UTILITY_H_NDB
#define EXPRESSION_UTILITY_H_NDB

#include <ndb/utility.hpp>
#include <ndb/expression/type.hpp>

namespace ndb
{
    struct statement_;


    template<class L, class R>
    using enable_expression = std::enable_if_t<(ndb::is_expression<L> || ndb::is_expression<R>
                                               || ndb::is_field<L> || ndb::is_field<R>
                                               || ndb::is_table<L> || ndb::is_table<R>) && !std::is_same_v<L, ndb::statement_>
    >;



} // ndb

#endif // EXPRESSION_UTILITY_H_NDB