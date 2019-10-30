add_library(Utils bencmark.cpp
                  random_generator.cpp)

target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
