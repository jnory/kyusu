cmake_minimum_required(VERSION 3.6)
project(kyusu2)

add_subdirectory(externals/googletest)
find_package(Boost REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(tests)

set(HEADERS
        src/preprocess.h
        src/config.h
        src/splitter.h
        src/concatter.h
        src/utf8.h
        )

set(SOURCE_FILES main.cpp ${HEADERS})
add_executable(kyusu2 ${SOURCE_FILES})