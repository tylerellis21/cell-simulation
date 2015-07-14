project (cell-common)

set (COMMON_SRC
    util/log.h
    util/log.cpp

    util/picojson.h

    util/timehelper.h
    util/timehelper.cpp
)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
add_library (cell-common STATIC ${COMMON_SRC})