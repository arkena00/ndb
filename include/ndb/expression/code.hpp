#ifndef EXPRESSION_CODE_NDB
#define EXPRESSION_CODE_NDB

#include <ndb/expression/category.hpp>

namespace ndb
{
    template<class Type, expression_categories Category>
    struct expression_code
    {
        static constexpr const char* value = " _expr_code_undefined_ ";
    };
} // ndb

#endif // EXPRESSION_CODE_NDB
