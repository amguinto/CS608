cmake_minimum_required(VERSION 3.0.0)
project(CS608 VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

find_package(GMP REQUIRED)

add_executable(CS608 
    main.cpp
    Math_utils.hpp
    Message_utils.hpp
)

target_link_libraries(CS608 gmp gmpxx)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
