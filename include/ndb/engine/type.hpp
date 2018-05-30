//! \brief types conversions for engines

#ifndef ENGINE_TYPE_H_NDB
#define ENGINE_TYPE_H_NDB


namespace ndb
{
    // engine ndb_type / type_id
    template<class Engine, class T>
    struct engine_type_id;

    template<class T, int Type_id>
    struct engine_ndb_type;
} // ndb

#endif // ENGINE_TYPE_H_NDB