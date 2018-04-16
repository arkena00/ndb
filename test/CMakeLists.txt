cmake_minimum_required(VERSION 3.7)

project(test_ndb)
enable_testing()

add_library(lib_gtest OBJECT ${THIRD_PARTY_ROOT}/gtest/source/gtest-all.cc)
set(GTEST_LIB $<TARGET_OBJECTS:lib_gtest>)

include_directories(${THIRD_PARTY_ROOT}/gtest/include ../)

add_executable(test_ndb source/main.cpp source/query.cpp source/expression.cpp)
add_dependencies(test_ndb lib_sqlite lib_gtest)
target_link_libraries(test_ndb ${ENGINE_LIB} ${NDB_LIB} ${GTEST_LIB})


set_target_properties(test_ndb PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${NDB_BIN_ROOT}/test"
        OUTPUT_NAME  "test_ndb")

add_test(NAME testing_ndb COMMAND "${NDB_BIN_ROOT}/experimental}/test_ndb")