cmake_minimum_required(VERSION 3.28)


add_executable(logger_tests
    test_file_logger.cpp
        test_logger_common.cpp
    test_main.cpp
)

target_link_libraries(logger_tests PRIVATE
    logger
    CppUTest
    CppUTestExt
)

enable_testing()
add_test(NAME logger_tests COMMAND logger_tests)
