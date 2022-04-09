cmake_minimum_required(VERSION 3.20)
project(formula)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_SYSTEM_NAME STREQUAL "GNU"           OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Clang"     OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  list(APPEND COMPILER_FLAGS -Wall -Wextra -pedantic
                     -pedantic-errors
                     -Werror -Wno-c++98-compat
                     -Wno-c++98-compat-pedantic
                     -Wno-padded)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"     OR
      CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    list(APPEND COMPILER_FLAGS -Weverything -Wno-global-constructors)
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  list(APPEND COMPILER_FLAGS /Wall /WX /wd4668 /wd4514 /wd5045 /wd5026)
endif()

find_package(GTest CONFIG REQUIRED)

enable_testing()

include_directories(src)

add_subdirectory(src)
add_subdirectory(tst)
