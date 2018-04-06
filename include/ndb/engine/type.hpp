//! \brief types conversions for engines

#ifndef ENGINE_TYPE_H_NDB
#define ENGINE_TYPE_H_NDB

#include <cstddef>
#include <ndb/value.hpp>

namespace ndb
{

    template<class Engine, class... Ts>
    struct type_list;

    template<class Engine, class T>
    struct type_id { static constexpr int value = 0; } ;

    template<class Engine, int>
    struct cpp_type;

    template<class Engine, class T>
    struct native_type
    {
        using type = T;
    };

    template<class Engine>
    struct type
    {
        template<class T> using id = type_id<Engine, T>;
        template<class T> using native = typename native_type<Engine, T>::type;
        template<int ID> using get = typename cpp_type<Engine, ID>::type;

        template<class T> static auto encode(const T& v) -> native<T>;
        template<class T> static T decode(const ndb::value&);
    };
} // ndb

#endif // ENGINE_TYPE_H_NDB