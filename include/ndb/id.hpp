#ifndef ID_H_NDB
#define ID_H_NDB

#include <ndb/database.hpp>
#include <ndb/table.hpp>
#include <ndb/field.hpp>

#include <ndb/utility.hpp>
#include <ndb/utility/cx_string.hpp>

namespace ndb
{
    namespace internal
    {
        template<template<class...> class T>
        struct id;

        template<> struct id<ndb::database> { static constexpr auto value = 'D'; };
        template<> struct id<ndb::table> { static constexpr auto value = 'T'; };
        template<> struct id<ndb::field> { static constexpr auto value = 'F'; };
    } // internal

    template<template<class...> class T>
    static constexpr auto id = internal::id<T>::value;

    template<class Database>
    constexpr unsigned char database_id = index_of<std::decay_t<Database>, typename std::decay_t<Database>::group::Detail_::entity>::value;
    template<class Table>
    constexpr unsigned char table_id = index_of<std::decay_t<Table>, typename std::decay_t<Table>::model::Detail_::entity>::value;
    template<class Field>
    constexpr unsigned char field_id = index_of<std::decay_t<Field>, typename std::decay_t<Field>::table::Detail_::entity>::value;

////////////////////////////////////////////////////////////////////////////////
////////////////////////             NAMES              ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    template<class Database>
    constexpr const char* database_name = ndb::cx_string<ndb::id<ndb::database>, ndb::database_id<std::decay_t<Database>>>;
    template<class Table>
    constexpr const char* table_name = ndb::cx_string<ndb::id<ndb::table>, ndb::table_id<std::decay_t<Table>>>;
    template<class Field>
    constexpr const char* field_name = ndb::cx_string<ndb::id<ndb::table>, ndb::table_id<typename std::decay_t<Field>::table>, ndb::id<ndb::field>, ndb::field_id<std::decay_t<Field>>>;
    template<class T, class Edge = std::decay_t<T>>
    constexpr const char* edge_name = ndb::cx_string<ndb::id<ndb::table>, ndb::table_id<Edge::source_table>, ndb::id<ndb::table>, ndb::table_id<Edge::target_table>>;

    template<class T>
    auto name()
    {
        if constexpr (ndb::is_field<T>) return ndb::field_name<T>;
        else if constexpr (ndb::is_table<T>) return ndb::table_name<T>;
        else if constexpr (ndb::is_database<T>) return std::string(T::group::name) + ndb::database_name<T>;
    }

    template<class T>
    auto name(const T&)
    {
        return name<T>();
    }
} // ndb

#endif // ID_H_NDB
