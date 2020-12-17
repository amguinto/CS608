cmake_minimum_required(VERSION 3.0.0)
project(test_CS608 VERSION 0.1.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# Google Test
find_package(GTest REQUIRED)

# Local FindGMP.cmake file
find_package(GMP REQUIRED)


add_executable(run_tests
    test_Crypto_utils.cpp
    test_Diffie_Hellman_KE.cpp
    test_ElGamal.cpp
    test_RSA.cpp
    test_Message_utils.cpp
    test_Math_utils.cpp
    test_Zero_Knowledge_Proof.cpp
)

target_link_libraries(run_tests 
    gmp 
    gmpxx 
    gtest 
    gtest_main 
    pthread
)
