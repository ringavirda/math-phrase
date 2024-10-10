cmake_minimum_required(VERSION 3.10)
project(math-interpreter)

set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}\\bin")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
if (!MSVC)
    set(CMAKE_GENERATOR "Ninja")
endif()

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)
if (WIN32)
    ADD_DEFINITIONS(-DWIN32)
endif()

add_executable(math-int
    src/main.cpp
    src/per-operation/interprete.cpp
    src/per-operation/per-operation.cpp
    src/per-phrase/per-phrase.cpp
    src/per-phrase/lexical.cpp
    src/per-phrase/syntax.cpp
    src/per-phrase/semantic.cpp
    src/per-phrase/echo.cpp
    src/type-list/meta.cpp
    )
target_include_directories(math-int PRIVATE inc/ inc/per-phrase)

if(MSVC)
    target_compile_options(math-int PRIVATE /W4)
else()
    target_compile_options(math-int PRIVATE -Wall -Wextra -Wpedantic)
endif()