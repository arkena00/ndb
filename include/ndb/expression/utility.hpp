#ifndef EXPRESSION_UTILITY_H_NDB
#define EXPRESSION_UTILITY_H_NDB

#include <ndb/utility.hpp>

namespace ndb
{
    template<class L, class R>
    using enable_expression = std::enable_if_t<ndb::is_expression<L> || ndb::is_expression<R>
                                               || ndb::is_field<L> || ndb::is_field<L>
                                               || ndb::is_table<L> || ndb::is_table<L>
    >;

    // deduce source
    template<class Expr>
    constexpr auto deduce_source()
    {
        // get source from first field
        bool field_found = false;
        int table_id = -1;
        Expr::static_eval([&](auto&& e)
                          {
                              if constexpr (e.type == expr_type_code::field)
                              {
                                  using Table = typename std::decay_t<decltype(e)>::value_type::table;
                                  if (!field_found)
                                  {
                                      field_found = true;
                                      table_id = ndb::table_id<Table>;
                                  }
                              }
                          });
        return table_id;
    }
} // ndb

#endif // EXPRESSION_UTILITY_H_NDB
