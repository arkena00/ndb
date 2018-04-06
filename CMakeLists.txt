cmake_minimum_required(VERSION 3.7)

project(ndb C CXX)

message("--------------------------------------------------------
                         NDB
--------------------------------------------------------")

# roots
set(NDB_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(NDB_BIN_ROOT ${NDB_ROOT}/bin)
set(THIRD_PARTY_ROOT ${NDB_ROOT}/third_party)

# config file for custom variables
include(config.cmake OPTIONAL)

option(ENGINE_SQLITE "Build sqlite" ON)
option(ENGINE_MONGO "Build mongo" OFF)
option(ENGINE_MYSQL "Build mysql" OFF)
option(BUILD_EXP "Build experimentals" ON)

# display config
message("SQLITE : ${ENGINE_SQLITE}")
message("MONGO : ${ENGINE_MONGO}")
message("MYSQL : ${ENGINE_MYSQL}")
message("")

# boost_pp
set(BOOST_PP_INCLUDE ${THIRD_PARTY_ROOT}/boost)

#-------------------------------------------------------
#                      COMPILER
#-------------------------------------------------------
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(COMPILER_DIRNAME msvc)
    add_compile_options(/wd4710 /wd4251 /std:c++17 /we4309)
endif()

if (MINGW)
    set(COMPILER_DIRNAME mingw)
    set(NDB_LIB stdc++fs)
endif()

if (UNIX)
    add_compile_options(-std=c++17)
    set(NDB_LIB stdc++fs pthread dl)
endif()

#-------------------------------------------------------
#                       ENGINE
#-------------------------------------------------------
if (ENGINE_MONGO)
    include(third_party/mongo.cmake)
endif()

if (ENGINE_MYSQL)
    message("ENGINE_MYSQL : ON")
    set(MYSQL_ROOT C:/lib/mysql-connector)
    set(MYSQL_LIB mysqlcppconn)
endif()

if (ENGINE_SQLITE)
    add_definitions(-DNDB_ENGINE_SQLITE)
    add_library(lib_sqlite OBJECT ${THIRD_PARTY_ROOT}/sqlite/source/sqlite3.c)
    set(SQLITE_LIB $<TARGET_OBJECTS:lib_sqlite>)
    list(APPEND ENGINE_INCLUDE "${THIRD_PARTY_ROOT}/sqlite/include ")
    list(APPEND ENGINE_LIB ${SQLITE_LIB})
endif()

# include
include_directories(include ${BOOST_PP_INCLUDE} ${ENGINE_INCLUDE})

# subdirs
add_subdirectory(test)

if (BUILD_EXP)
    add_subdirectory(experimental)
endif()

# delete some cache variables
unset(ENGINE_MONGO CACHE)
unset(ENGINE_MYSQL CACHE)
unset(BUILD_EXP CACHE)