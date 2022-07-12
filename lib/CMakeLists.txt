#required for MD4 implementation
find_package(Qt5 REQUIRED COMPONENTS Core)

set(HEADERS
    include/utils.h
    include/rollingchecksum.h
    include/strongchecksum.h)

set(SOURCES
    rollingchecksum.cpp
    strongchecksum.cpp)

add_library(yardifflib ${HEADERS} ${SOURCES})

target_link_libraries(yardifflib PUBLIC Qt5::Core)

target_include_directories(yardifflib PUBLIC include)
