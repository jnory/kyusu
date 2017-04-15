

set(TEST_SOURCE
        test_config.cpp
        test_preprocess.cpp
        test_splitter.cpp
        test_concatter.cpp
        )

configure_file(test_env.h.in test_env.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(kyusu_test ${TEST_SOURCE})
target_link_libraries(kyusu_test
        gtest gtest_main)