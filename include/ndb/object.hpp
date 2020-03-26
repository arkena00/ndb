#ifndef INCLUDE_NDB_OBJECT_HPP_NDB
#define INCLUDE_NDB_OBJECT_HPP_NDB

#include <ndb/type.hpp>

#define ndb_access template<class> friend struct ::ndb::internal::object_access

namespace ndb
{
    enum class object_states { uninitialized = 0, loaded = 1, unloaded = 2, stored = 4, updated = 8 };

    namespace internal
    {
        template<class Database, class T> struct object_get;
        template<class Database, class T> struct object_set;
        template<class Database, class T> struct object_add;
        template<class Database, class T> struct object_del;

        template<class T>
        struct object_access
        {
            using database = typename T::ndb_internal_database;
            using object_type = typename T::ndb_internal_object_type;

            template<class U>
            static U& cast(T& u) { return static_cast<U&>(u); }

            static object_states state(T& t) { return t.ndb_internal_state_; }
            static void set_state(T& t, object_states state) { t.ndb_internal_state_ = state; }
        };
    } // internal

    template<class Database, class Object>
    class object : public Object
    {
        template<class> friend struct internal::object_access;

        using ndb_internal_database = Database;
        using ndb_internal_object_type = Object;

        object_states ndb_internal_state_ = object_states::uninitialized;
    };

    template<class Object>
    using oid_t = ndb::cpp_type_t<oid_, typename internal::object_access<Object>::database>;

    /*
    template<class Database, class T>
    auto object_get(ndb::cpp_type_t<oid_, Database> oid)
    {
        return internal::object_get<Database, T>(oid);
    }*/

    template<class Database, class Object_type, class T>
    auto object_get(ndb::cpp_type_t<oid_, Database> oid, T& obj)
    {
        return internal::object_get<Database, Object_type>::process(oid, obj);
    }

    template<class Database, class T>
    auto object_del(T& obj)
    {
        return internal::object_del<Database, T>::process(obj);
    }

    template<class T>
    auto object_state(T& obj)
    {
        return internal::object_access<T>::state(obj);
    }

    template<class T>
    bool has_state(T& obj, ndb::object_states state)
    {
        return (int)internal::object_access<T>::state(obj) == (int)state;
    }

    template<class T>
    bool is_stored(const T& t)
    {
        return ndb::has_state(t, object_states::loaded) || ndb::has_state(t, object_states::stored) || ndb::has_state(t, object_states::updated);
    }

    template<class T>
    bool is_valid(const T& t)
    {
        return !ndb::has_state(t, ndb::object_states::uninitialized);
    }

    template<class T>
    bool is_saved(const T& t)
    {
        return !ndb::has_state(t, ndb::object_states::uninitialized) && !ndb::has_state(t, ndb::object_states::unloaded);
    }

    template<class T>
    void load(T& obj, ndb::oid_t<T> oid)
    {
        using Database = typename internal::object_access<T>::database;
        using Object_type = typename internal::object_access<T>::object_type;
        ndb::object_get<Database, Object_type>(oid, internal::object_access<T>::template cast<ndb::object<Database, Object_type>>(obj));
    }

    namespace internal
    {
        template<class Container>
        struct load;

        template<template<class...> class Container, class T, class... Ts>
        struct load<Container<T, Ts...>>
        {
            template<class... Args>
            static void process(Container<T, Ts...>& container, Args&&... args)
            {
                using Database = typename internal::object_access<T>::database;
                using Object_type = typename internal::object_access<T>::object_type;

                ::ndb::internal::object_get<Database, Object_type>::template process<T>(container, std::forward<Args>(args)...);
            }
        };
    } // internal

    template<class T, class... Args>
    void load_bulk(T& v, Args&&... args)
    {
        internal::load<T>::process(v, std::forward<Args>(args)...);
    }

    /// Remove the object data from database, object is in unloaded state
    template<class T>
    void unload(T& obj)
    {
        using Database = typename internal::object_access<T>::database;
        using Object_type = typename internal::object_access<T>::object_type;
        ndb::object_del<Database, Object_type>(internal::object_access<T>::template cast<ndb::object<Database, Object_type>>(obj));
        internal::object_access<T>::set_state(obj, object_states::unloaded);
    }

    template<class T>
    auto store(T& obj)
    {
        using Database = typename internal::object_access<T>::database;
        using Object_type = typename internal::object_access<T>::object_type;

        if (ndb::is_stored(obj))
        {
            internal::object_set<Database, Object_type>::process(internal::object_access<T>::template cast<Object_type>(obj));
            internal::object_access<T>::set_state(obj, object_states::updated);
            return obj.oid;
        }
        else
        {
            auto oid = internal::object_add<Database, Object_type>::process(internal::object_access<T>::template cast<Object_type>(obj));
            internal::object_access<T>::set_state(obj, object_states::stored);
            return oid;
        }
    }

    template<class T, class... Ts>
    auto make(ndb::oid_t<T> oid, Ts... args)
    {
        using Object_type = typename internal::object_access<T>::object_type;
        using Database = typename internal::object_access<T>::database;

        T obj{ std::move(args)... };
        ndb::load(obj, oid);
        return obj;
    }
} // ndb

#endif // INCLUDE_NDB_OBJECT_HPP_NDB
