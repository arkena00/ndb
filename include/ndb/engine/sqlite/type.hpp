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
    class sqlite;

    // associate engine type id to native type
    template<> struct cpp_type<sqlite, SQLITE_INTEGER> { using type = int; };
    template<> struct cpp_type<sqlite, SQLITE_FLOAT> { using type = double; };
    template<> struct cpp_type<sqlite, SQLITE3_TEXT> { using type = std::string; };
    template<> struct cpp_type<sqlite, SQLITE_BLOB> { using type = std::vector<char>; };

    // associate native type to engine type id
    template<> struct type_id<sqlite, int> { static constexpr auto value = SQLITE_INTEGER; };
    template<> struct type_id<sqlite, double> { static constexpr auto value = SQLITE_FLOAT; };
    template<> struct type_id<sqlite, std::string> { static constexpr auto value = SQLITE3_TEXT; };
    template<> struct type_id<sqlite, std::vector<char>> { static constexpr auto value = SQLITE_BLOB; };

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
            return std::string(v);
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
    };
} // ndb


#endif // ENGINE_SQLITE_TYPE_H_NDB
