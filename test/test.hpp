#ifndef TEST_H_NDB
#define TEST_H_NDB

#include "gtest/gtest.h"

// engines
#include <ndb/engine/sqlite/sqlite.hpp>

// test database
#include "database.hpp"

#define TEST_SQLITE ndb::sqlite

#ifdef NDB_ENGINE_MONGO
//#include <ndb/engine/mongo/mongo.hpp>
    #define TEST_MONGO ,ndb::mongo
#else
    #define TEST_MONGO
#endif

#ifdef NDB_ENGINE_POSTGRE
    #include <ndb/engine/postgre/postgre.hpp>
    #define TEST_POSTGRE ,ndb::postgre
#else
    #define TEST_POSTGRE
#endif

#define TEST_ENGINE TEST_SQLITE TEST_MONGO TEST_POSTGRE



#endif // TEST_H_NDB