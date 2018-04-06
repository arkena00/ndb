#ifndef CONFIG_H_NDB
#define CONFIG_H_NDB

#include <ndb/engine/sqlite/sqlite.hpp>

#define TEST_SQLITE ndb::sqlite

#ifdef NDB_ENGINE_MONGO
#include <ndb/engine/mongo/mongo.hpp>
    #define TEST_MONGO ,ndb::mongo
#else
    #define TEST_MONGO
#endif

#define TEST_ENGINE TEST_SQLITE TEST_MONGO

#endif // CONFIG_H_NDB