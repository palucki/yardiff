# GTest include code
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(test_checksums test_strongChecksum.cpp test_rollingChecksum.cpp)

target_link_libraries(test_checksums
 PRIVATE
  GTest::GTest
  yardifflib)

add_test(yardiff_gtests test_checksums)
