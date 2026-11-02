cmake_minimum_required(VERSION 3.16)
project(Student-Processor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(Student-Processor
        main.cpp
        failai.cpp
        utils.cpp
        results.cpp
        results.h
        manual.cpp
        manual.h
        nustatymai.h
        testai.cpp
        testai.h
        studentas.cpp
)

if(WIN32)
    target_link_libraries(Student-Processor PRIVATE psapi)
endif()
