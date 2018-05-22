//! \brief types conversions for engines

#ifndef ENGINE_TYPE_H_NDB
#define ENGINE_TYPE_H_NDB

#include <cstddef>

namespace ndb
{
    class value;
    struct type_undefined;

    template<class Engine, class... Ts>
    struct type_list;

    // engine ndb_type / type_id
    template<class Engine, class T>
    struct engine_type_id;

    template<class T, int Type_id>
    struct engine_ndb_type;

    // ndb_type / cpp_type
    template<class Ndb_type, class Engine>
    struct cpp_type { using type = type_undefined; };
    template<class Ndb_type, class Engine>
    using cpp_type_t = typename cpp_type<Ndb_type, Engine>::type;

    template<class Cpp_type, class Engine>
    struct ndb_type { using type = type_undefined; };
    template<class Cpp_type, class Engine>
    using ndb_type_t = typename ndb_type<Cpp_type, Engine>::type;

    // ndb_type from T, then cpp_type
    template<class T, class Engine>
    using native_type = cpp_type_t< ndb_type_t<T, Engine>, Engine >;

    template<class Engine>
    struct basic_type
    {
        template<class T> using id = engine_type_id<Engine, T>;
        template<class T> using native = native_type<T, Engine>;
    };

    template<class Engine>
    struct type : basic_type<Engine>
    {
        template<class T>
        static auto encode(const T& v) ->  typename basic_type<Engine>::template native<T>;
        template<class T>
        static T decode(const ndb::value&);
    };
} // ndb

#endif // ENGINE_TYPE_H_NDB