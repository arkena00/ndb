#ifndef INCLUDE_NDB_OBJECT_HPP_NDB
#define INCLUDE_NDB_OBJECT_HPP_NDB

namespace ndb
{
    template<class T>
    struct object_access : public T
    {
        using typename T::database;
        using object_type = typename T::object_type; // fix msvc

        static bool is_loaded(const T& t) { return t.is_loaded_; }
    };

    template<class Database, class Object>
    class object : public Object
    {
        template<class> friend struct object_access;

        using database = Database;
        using object_type = Object;

        bool is_loaded_ = false;
    };



    template<class Database, class T>
    auto object_get(int oid)
    {
        return internal::object_get<Database, T>(oid);
    }

    template<class Database, class T>
    auto object_get(int oid, T& obj)
    {
        return internal::object_get<Database, T>::process(oid, obj);
    }

    template<class T>
    bool is_loaded(const T& t)
    {
        return object_access<T>::is_loaded(t);
    }

    template<class T, class... Ts>
    auto make(int oid, Ts... args)
    {
        using Database = typename ndb::object_access<T>::database;
        using Object_type = typename ndb::object_access<T>::object_type;
        T obj{ std::move(args)... };
        ndb::object_get<Database, Object_type>(oid, obj);
        return obj;
    }
} // ndb

#endif // INCLUDE_NDB_OBJECT_HPP_NDB
