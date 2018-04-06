#ifndef TABLE_H_NDB
#define TABLE_H_NDB

//! \brief table for database model

#include <cstddef>

namespace ndb
{
    class table_base {};

    template<class Model>
    struct table : table_base
    {
        using model = Model;
        //using type = ndb::table<Model, Table>;
        //using Detail_ = typename Table::Detail_;

        //static constexpr Detail_ detail_ {};
    };

    template<class Entity, class... Options>
    struct table_detail
    {
        using entity = Entity;

        static constexpr std::size_t size = Entity::size() + sizeof...(Options);
    };
} // ndb

#endif // TABLE_H_NDB
