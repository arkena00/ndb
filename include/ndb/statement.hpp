#ifndef STATEMENT_NDB
#define STATEMENT_NDB

#include <ndb/expression.hpp>

namespace ndb
{
    static constexpr ndb::expression<expressions::statement_> statement;

    template<class Database>
    struct dynamic_statement
    {
        typename Database::engine::statement_type output_;

        template<class Expr>
        dynamic_statement& operator<<(const Expr& e)
        {
            auto s = ndb::native_expression<Expr, typename Database::engine>{};
            output_ += s.data();
            return *this;
        }

        const auto& native() const
        {
            return output_;
        }
    };
} // ndb


#endif // STATEMENT_NDB
