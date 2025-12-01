cmake_minimum_required(VERSION 3.22)




if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME ssd1306)
project(${CMAKE_PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_C++_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Build type: " ${CMAKE_BUILD_TYPE})


enable_language(CXX)
include(GNUInstallDirs)

add_library(${CMAKE_PROJECT_NAME}_LIB STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/i2c_driver.cpp
)


target_include_directories(${CMAKE_PROJECT_NAME}_LIB PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME}_LIB PRIVATE
)

target_link_libraries(${CMAKE_PROJECT_NAME}_LIB PRIVATE 
    # i2c
)


# add_subdirectory(tests)

add_executable(${CMAKE_PROJECT_NAME}    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_LIB)
install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
