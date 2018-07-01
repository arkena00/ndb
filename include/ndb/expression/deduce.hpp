#ifndef EXPRESSION_DEDUCE_H_NDB
#define EXPRESSION_DEDUCE_H_NDB

#pragma warning(push, 0)

#include <kvasir/mpl/algorithm/filter.hpp>
#include <kvasir/mpl/algorithm/transform.hpp>
#include <kvasir/mpl/algorithm/all.hpp>
#include <kvasir/mpl/sequence/join.hpp>
#include <kvasir/mpl/sequence/front.hpp>
#include <kvasir/mpl/sequence/size.hpp>
#include <kvasir/mpl/types/int.hpp>
#include <kvasir/mpl/types/list.hpp>
#include <kvasir/mpl/functional/call.hpp>
#include <kvasir/mpl/functional/fork.hpp>
#include <kvasir/mpl/functions/comparison/equal.hpp>
#include <kvasir/mpl/functions/logical/logical_or.hpp>

#pragma warning(pop)

#include <ndb/expression.hpp>
#include <ndb/cx_error.hpp>

namespace mpl = kvasir::mpl;

namespace ndb::internal
{

} // ndb::internal

struct error
{
    using table = int;
};

template<class T>
struct table_bot_found
{
    using value_type = error;
    static constexpr auto value = 0;
};

namespace ndb
{
    template<class Expr>
    struct deduce
    {

        //using main_table = typename

    };
} // ndb

#endif // EXPRESSION_DEDUCE_H_NDB

