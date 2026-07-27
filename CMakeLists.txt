cmake_minimum_required(VERSION 3.28)
project(logger_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

add_subdirectory(logger)
add_subdirectory(app_file)
add_subdirectory(tests)
