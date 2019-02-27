if (NDB_ENGINE_POSTGRE)

    set(NDB_PQ_LIB_NAMES libpq libpq5 pq pq5)

    # search in system path then specified path
    find_library(NDB_PQ_LIB NAMES ${NDB_PQ_LIB_NAMES} HINTS "${NDB_PQ_PATH}/lib")
    find_path(NDB_PQ_INCLUDE NAMES postgresql/libpq-fe.h HINTS ${NDB_PQ_PATH}/include)

    if (NOT NDB_PQ_LIB)
        message(FATAL_ERROR  "PostgreSQL lib not found")
    endif()

    if (NOT NDB_PQ_INCLUDE)
        message(FATAL_ERROR  "PostgreSQL include not found")
    endif()

    list(APPEND NDB_ENGINE_INCLUDE ${NDB_PQ_INCLUDE})
    list(APPEND NDB_ENGINE_LIB ${NDB_PQ_LIB})
endif()