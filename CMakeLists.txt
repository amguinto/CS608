cmake_minimum_required(VERSION 3.0.0)
project(CS608 VERSION 0.1.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# Google Test
find_package(GTest REQUIRED)

# Local FindGMP.cmake file
find_package(GMP REQUIRED)

# Required to create a test directory with executable.
add_subdirectory(test)

include(CTest)

add_executable(CS608 
    main.cpp
    utils/Math_utils.hpp
    utils/Message_utils.hpp
    utils/Crypto_utils.hpp
)

target_link_libraries(CS608 gmp gmpxx gtest)
