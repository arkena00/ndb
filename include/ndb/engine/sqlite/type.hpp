#ifndef ENGINE_SQLITE_TYPE_H_NDB
#define ENGINE_SQLITE_TYPE_H_NDB

#pragma once

#include <ndb/engine/type.hpp>
#include "sqlite3.h"
#include <chrono>
#include <string>
#include <vector>
#include <variant>

namespace ndb
{
    #define ndb_type_map(NDB_TYPE, CPP_TYPE) \
    template<class Engine> \
    struct cpp_type<NDB_TYPE, Engine> { using type = CPP_TYPE; }; \
    template<class Engine> \
    struct ndb_type<CPP_TYPE, Engine> { using type = NDB_TYPE; };

    class sqlite;

    // ndb types are proxy types
    namespace types
    {
        struct int_;
        struct float_;
        struct double_;
        struct string_;
        struct byte_array_;
    } // types
    using namespace ::ndb::types;

    // associate engine type id to ndb type
    template<> struct engine_ndb_type<sqlite, SQLITE_INTEGER> { using type = int_; };
    template<> struct engine_ndb_type<sqlite, SQLITE_FLOAT> { using type = float_; };
    template<> struct engine_ndb_type<sqlite, SQLITE3_TEXT> { using type = string_; };
    template<> struct engine_ndb_type<sqlite, SQLITE_BLOB> { using type = byte_array_; };

    // associate ndb type to engine type id
    template<> struct engine_type_id<sqlite, int_> { static constexpr auto value = SQLITE_INTEGER; };
    template<> struct engine_type_id<sqlite, double_> { static constexpr auto value = SQLITE_FLOAT; };
    template<> struct engine_type_id<sqlite, string_> { static constexpr auto value = SQLITE3_TEXT; };
    template<> struct engine_type_id<sqlite, byte_array_> { static constexpr auto value = SQLITE_BLOB; };

    // associate ndb_type -> cpp_type, cpp_type -> ndb_type
    ndb_type_map(int_, int);
    ndb_type_map(float_, float);
    ndb_type_map(double_, double);
    ndb_type_map(string_, std::string);


    // custom type (non-native)
    template<class T, class U>
    struct ndb_type<std::chrono::duration<T, U>, sqlite> { using type = double_; };


/*
    // associate custom type to native type
    template<> struct native_type<sqlite, const char*> { using type = std::string; };
    template<class T, class U>
    struct native_type<sqlite, std::chrono::duration<T, U>> { using type = double; };

    // custom type conversions
    template<>
    struct type<sqlite> : basic_type<sqlite>
    {
        inline static auto encode(const char* v) -> native<std::decay_t<decltype(v)>>
        {
            return v;
        }

        template<class T, class U>
        inline static auto encode(const std::chrono::duration<T, U>& v) -> native<std::decay_t<decltype(v)>>
        {
            return v.count();
        }
        template<class T>
        inline static T decode(const native<T>& v)
        {
            return T{ v };
        };
    };*/
} // ndb


#endif // ENGINE_SQLITE_TYPE_H_NDB
