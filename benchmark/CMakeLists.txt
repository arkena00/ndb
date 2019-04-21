cmake_minimum_required(VERSION 3.10)

project(ndb_bench C CXX)

set(CMAKE_CXX_STANDARD 17)


#target_precompiled_header(ndb_pch ${NDB_ROOT}/include/ndb/line.hpp)

function(benchmark_make target_file)
    set(targetname ${target_file})



    #add_executable(${targetname} ${target_file}.cpp)
    target_link_libraries(${targetname} lib_ndb)
    #target_precompiled_header(${targetname}  include/ndb/line.hpp REUSE ndb_pch)
    add_dependencies(${targetname} lib_ndb)
endfunction()


set(BENCH 1)


if(BENCH)
    include(${THIRD_PARTY_ROOT}/metabench/metabench.cmake)


    metabench_add_dataset(bench_base "base.cpp" "(0...10).step(1)" NAME Macros DB)
    benchmark_make(bench_base)

    metabench_add_dataset(bench_main "main.cpp" "(0...10).step(1)" NAME C++ DB)
    benchmark_make(bench_main)

    metabench_add_chart(benchmark_pch
            TITLE "Macros test"
            SUBTITLE ""
            XLABEL ""
            YLABEL "Time"
            DATASETS bench_base bench_main)
endif()



