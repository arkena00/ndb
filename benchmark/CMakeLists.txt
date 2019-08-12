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

set(BENCH 0)

if(BENCH)
    include(${THIRD_PARTY_ROOT}/metabench/metabench.cmake)


    metabench_add_dataset(bench_native_query "native_query.cpp" "(0...2).step(1)" NAME native_query)
    benchmark_make(bench_native_query)

    metabench_add_dataset(bench_ndb_query "ndb_query.cpp" "(0...2).step(1)" NAME ndb_query)
    benchmark_make(bench_ndb_query)

    metabench_add_chart(benchmark_ndb_query
            TITLE "benchmark_ndb_query"
            SUBTITLE ""
            XLABEL ""
            YLABEL "Time"
            DATASETS bench_native_query bench_ndb_query)
endif()



