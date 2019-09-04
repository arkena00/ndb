cmake_minimum_required(VERSION 3.10)

project(ndb_bench C CXX)

set(CMAKE_CXX_STANDARD 17)


#target_precompiled_header(ndb_pch ${NDB_ROOT}/include/ndb/line.hpp)

function(benchmark_make target_file)
    set(targetname ${target_file})

    add_executable(bench_${targetname} ${targetname}.cpp instance.cpp)
    target_link_libraries(bench_${targetname} lib_ndb)
    add_dependencies(bench_${targetname} lib_ndb)

    #target_sources(${targetname} PUBLIC instance.cpp)
endfunction()


benchmark_make(ndb_query)

add_executable(bench_sqlite sqlite.cpp)
target_link_libraries(bench_sqlite lib_ndb)
