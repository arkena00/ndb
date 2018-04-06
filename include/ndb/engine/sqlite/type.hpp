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

    // associate engine type id to cpp type
    template<> struct cpp_type<sqlite, SQLITE_INTEGER> { using type = int; };
    template<> struct cpp_type<sqlite, SQLITE_FLOAT> { using type = double; };
    template<> struct cpp_type<sqlite, SQLITE3_TEXT> { using type = std::string; };
    template<> struct cpp_type<sqlite, SQLITE_BLOB> { using type = std::vector<char>; };

    // associate native type to engine type id
    template<> struct type_id<sqlite, int> { static constexpr auto value = SQLITE_INTEGER; };
    template<> struct type_id<sqlite, double> { static constexpr auto value = SQLITE_FLOAT; };
    template<> struct type_id<sqlite, std::string> { static constexpr auto value = SQLITE3_TEXT; };
    template<> struct type_id<sqlite, std::vector<char>> { static constexpr auto value = SQLITE_BLOB; };

    // specify native type for custom type
    template<> struct native_type<sqlite, std::chrono::time_point<std::chrono::system_clock>> { using type = int; };

    template<> template<>
    inline auto type<sqlite>::encode(const std::chrono::time_point<std::chrono::system_clock>& v) -> native<std::decay_t<decltype(v)>>
    {
        auto epoch = v.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::seconds>(epoch);
        return static_cast<int>(value.count());
    }

/*
    template<>
    auto type<sqlite>::encode(const std::chrono::time_point<std::chrono::system_clock>& v) -> native<std::decay_t<decltype(v)>>
    {
        auto epoch = v.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::seconds>(epoch);
        return static_cast<int>(value.count());
    }*/

} // ndb


#endif // ENGINE_SQLITE_TYPE_H_NDB
