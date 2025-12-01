cmake_minimum_required(VERSION 3.22)

set(CMAKE_C++_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
set(FETCHCONTENT_FULLY_DISCONNECTED false)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(${CMAKE_PROJECT_NAME}_tests 
   test_ssd1306.cpp
)
target_include_directories(${CMAKE_PROJECT_NAME}_tests PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
)
target_link_libraries(${CMAKE_PROJECT_NAME}_tests  PUBLIC
  ${CMAKE_PROJECT_NAME}_LIB
  GTest::gtest GTest::gtest_main)





include(GoogleTest)
gtest_discover_tests(${CMAKE_PROJECT_NAME}_tests)