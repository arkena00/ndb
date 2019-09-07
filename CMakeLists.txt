cmake_minimum_required(VERSION 3.7)
if (POLICY CMP0077)
    cmake_policy(SET CMP0077 OLD) # remove cmake option warning
endif()

project(ndb C CXX)
enable_testing()

message("--------------------------------------------------------
                         NDB
--------------------------------------------------------")

# config file for custom variables
include(config.cmake OPTIONAL)

#-------------------------------------------------------
#                        VARS
#-------------------------------------------------------
# roots
set(NDB_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(NDB_BIN_ROOT ${NDB_ROOT}/bin)
set(THIRD_PARTY_ROOT ${NDB_ROOT}/third_party)

option(NDB_ENGINE_SQLITE "Build sqlite" ${NDB_ENGINE_SQLITE})
option(NDB_ENGINE_MONGO "Build mongo" ${NDB_ENGINE_MONGO})
option(NDB_ENGINE_POSTGRE "Build postgresql" ${NDB_ENGINE_POSTGRE})
option(NDB_BUILD_EXP "Build experimentals" ${NDB_BUILD_EXP})
option(NDB_BUILD_TEST "Build tests" ${NDB_BUILD_TEST})
option(NDB_BUILD_EXAMPLE "Build examples" ${NDB_BUILD_EXAMPLE})
option(NDB_BUILD_BENCHMARK "Build benchmarks" ${NDB_BUILD_BENCHMARK})

# display config
if(STZ_FILESYSTEM)
    message("STZ_FILESYSTEM : ${STZ_FILESYSTEM}")
endif()
message("SQLITE : ${NDB_ENGINE_SQLITE}")
message("MONGO : ${NDB_ENGINE_MONGO}")
message("POSTGRESQL : ${NDB_ENGINE_POSTGRE}")
message("BUILD_EXP : ${NDB_BUILD_EXP}")
message("BUILD_TEST : ${NDB_BUILD_TEST}")
message("BUILD_EXAMPLE : ${NDB_BUILD_EXAMPLE}")

add_library(lib_ndb INTERFACE)

#-------------------------------------------------------
#                       ENGINE
#-------------------------------------------------------
include(third_party/sqlite.cmake)
include(third_party/postgre.cmake)
include(third_party/mongo.cmake)

#-------------------------------------------------------
#                      COMPILER
#-------------------------------------------------------
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/wd4710 /wd4251 /we4309)
endif()

if (MINGW)
    set(NDB_LIB stdc++fs)
endif()

if (UNIX AND NOT ANDROID)
    set(NDB_LIB pthread dl)
    if (NOT STZ_FILESYSTEM)
        set(NDB_LIB "${NDB_LIB} stdc++fs")
    endif()
endif()

# generic
# include
set(NDB_INCLUDE include
        ${THIRD_PARTY_ROOT}/boost
        ${NDB_ENGINE_INCLUDE})

# use third_party filesystem
if(STZ_FILESYSTEM)
    set(NDB_DEFINITION -DSTZ_FILESYSTEM)
    list(APPEND NDB_INCLUDE ${THIRD_PARTY_ROOT}/stz)
endif()


#-------------------------------------------------------
#                     NDB INTERFACE
#-------------------------------------------------------
target_include_directories(lib_ndb INTERFACE ${NDB_INCLUDE})
target_link_libraries(lib_ndb INTERFACE ${NDB_ENGINE_LIB} ${NDB_LIB})
target_compile_definitions(lib_ndb INTERFACE ${NDB_DEFINITION})

#-------------------------------------------------------
#                        SUBDIRS
#-------------------------------------------------------
if (NDB_BUILD_EXP)
    add_subdirectory(experimental)
endif()
if (NDB_BUILD_TEST)
    add_subdirectory(test)
endif()
if (NDB_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
if (NDB_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()