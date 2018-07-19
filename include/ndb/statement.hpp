#ifndef STATEMENT_NDB
#define STATEMENT_NDB

#include <ndb/expression.hpp>
#include <iostream>

namespace ndb
{
    static constexpr ndb::expression<expressions::statement_> statement;

    struct dynamic_statement
    {
        std::string output_;

        template<class Expr>
        dynamic_statement& operator<<(const Expr& e)
        {
            auto s = ndb::native_expression<Expr, ndb::sqlite>{};
            output_ += s.c_str();
            return *this;
        }

        const auto& native()
        {
            return output_;
        }
    };
} // ndb


#endif // STATEMENT_NDB
