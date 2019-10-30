SET(TEST_SRC test_main.cpp
             common.cpp
             graph_test.cpp
             one_bridge_tests.cpp
             two_bridge_tests.cpp
             UtilsTests/sort_test.cpp
        )

add_executable(tests ${TEST_SRC})
target_link_libraries(tests PRIVATE Catch Graph Utils)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
