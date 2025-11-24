cmake_minimum_required(VERSION 3.22)

include(GNUInstallDirs)
set(CMAKE_C++_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME ssd1306)
project(${CMAKE_PROJECT_NAME} LANGUAGES CXX)
message("Build type: " ${CMAKE_BUILD_TYPE})


enable_language(CXX)

add_library(${CMAKE_PROJECT_NAME}_LIB INTERFACE)


target_sources(${CMAKE_PROJECT_NAME}_LIB PRIVATE 
    
)

target_include_directories(${CMAKE_PROJECT_NAME}_LIB INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME}_LIB PRIVATE
)

target_link_libraries(${CMAKE_PROJECT_NAME}_LIB INTERFACE i2c)


add_subdirectory(tests)

add_executable(${CMAKE_PROJECT_NAME}   
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_LIB)
install(TARGETS ${CMAKE_PROJECT_NAME} TYPE RUNTIME)
