cmake_minimum_required(VERSION 2.8.11)
project(passing)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG ")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")
endif ()


add_executable(passing-server helper.cpp helper.h Socket.cpp Socket.h main-server.cpp Fifo.cpp Fifo.h Pipe.cpp Pipe.h)

add_executable(passing-client helper.cpp helper.h Socket.cpp Socket.h main-client.cpp Fifo.cpp Fifo.h Pipe.cpp Pipe.h)
