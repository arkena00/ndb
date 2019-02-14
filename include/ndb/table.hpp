#ifndef TABLE_H_NDB
#define TABLE_H_NDB

//! \brief table for database model

#include <cstddef>

namespace ndb
{
    class table_base {};

    template<class Table>
    struct parent
    {
        using table = Table;
    };

    template<class Model>
    struct table : table_base
    {
        using model = Model;

        //using type = ndb::table<Model, Table>;
        //using Detail_ = typename Table::Detail_;

        //static constexpr Detail_ detail_ {};
    };

    struct table_option
    {
        template<class... Tables>
        struct unique {};
    };


    /*!
     * @tparam Entity ndb::entity<field1, field2, ...>
     * @tparam Parent table for relations
     * @tparam Object_type type for orm
     * @tparam Options table options
     */

    template<class Entity, class Parent = ndb::parent<void>, class Object_type = void, class Options = ndb::option<>>
    struct table_detail
    {
        using entity = Entity;
        using parent_table = typename Parent::table;
        using object_type = Object_type;
        using option = Options;

        static constexpr std::size_t size = Entity::size();
    };
} // ndb

#endif // TABLE_H_NDB