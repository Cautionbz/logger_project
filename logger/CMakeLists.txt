
add_library(logger
    src/logger.cpp
    src/file_logger.cpp
    src/socket_logger.cpp
)

target_include_directories(logger
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (WIN32)
    target_link_libraries(logger PRIVATE ws2_32)
endif()

set_target_properties(logger PROPERTIES
    OUTPUT_NAME "logger"
)
