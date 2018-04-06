#ifndef FIELD_H_NDB
#define FIELD_H_NDB

/*! \struct field can be static or dynamic, data will be stored in static or dynamic section of a table
 * Size is the space required for a field, it will be dynamic for dynamic fields
 *
 * Example :
 * field<int>
 * field<char, size<255>>
 * field<dynamic_size_object*>
 * field<dynamic_size_object_with_static_capacity*, 5000>
 */

#include <ndb/fwd.hpp>
#include <ndb/utility.hpp>
#include <ndb/option.hpp>

namespace ndb
{
    template<size_t Size>
    struct size
    {
        static constexpr auto value = Size;
    };

    // default field value : sizeof(T) for static_field, 0 for field_entity
    template<class T>
    constexpr size_t default_size()
    {
        if constexpr (ndb::is_table<T>) return 0;
        else return sizeof(T);
    }

    struct field_base {};

////////////////////////////////////////////////////////////////////////////////
////////////////////////          COMMON_FIELD          ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    template<class Table, class Value_type, class Size, class Option>
    struct common_field : field_base
    {
        using table = Table;
        using value_type = Value_type;

        static constexpr struct Detail_
        {
            static constexpr bool is_primary = has_option<field_option::primary, Option>::value;
            static constexpr bool is_autoincrement = has_option<field_option::autoincrement, Option>::value;
            static constexpr bool is_unique = has_option<field_option::unique, Option>::value;
            static constexpr bool is_not_null = has_option<field_option::not_null, Option>::value;
            static constexpr size_t size = Size::value;
        } detail_ {};
    };

////////////////////////////////////////////////////////////////////////////////
////////////////////////             FIELD              ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    // field<T>
    template<class Table, class T>
    struct field<Table, T, typename std::enable_if_t<!ndb::is_table<T>>> :
        common_field<Table, T, ndb::size<default_size<T>()>, ndb::option<>>
    {};

    // field<T, Size>
    template<class Table, class T, size_t Size>
    struct field<Table, T, ndb::size<Size>> :
        common_field<Table, T, ndb::size<Size>, option<field_option::none>>
    {};

    // field<T, option>
    template<class Table, class T, class... Option_code>
    struct field<Table, T, option<Option_code...>> :
        common_field<Table, T, ndb::size<default_size<T>()>, option<Option_code...>>
    {};

    // field<T, Size, Option>
    template<class Table, class T, size_t Size, class... Option_code>
    struct field<Table, T, ndb::size<Size>, option<Option_code...>> :
        common_field<Table, T, ndb::size<Size>, option<Option_code...>>
    {};

    // field<table>
    template<class Table, class T>
    struct field<Table, T, typename std::enable_if_t<ndb::is_table<T>>> :
        common_field<Table, T, ndb::size<default_size<T>()>, option<field_option::none>>
    {};

    // field<table, size>
    template<class Table, class T, size_t Size>
    struct field<Table, T, typename std::enable_if_t<ndb::is_table<T>>, ndb::size<Size>> :
        public common_field<Table, T, ndb::size<Size>, option<field_option::none>>
    {};
} // ndb

#endif // FIELD_H_NDB