find_package(Qt5 REQUIRED COMPONENTS Core)

set(HEADERS
    include/fastsignature.h
    include/strongsignature.h)

set(SOURCES
    yardiff.cpp
    fastsignature.cpp
    strongsignature.cpp)

add_library(yardifflib ${HEADERS} ${SOURCES})

target_link_libraries(yardifflib PUBLIC Qt5::Core)

target_include_directories(yardifflib PUBLIC include)
