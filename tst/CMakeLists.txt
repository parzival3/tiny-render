set(BINARY ${CMAKE_PROJECT_NAME}_tst)
set(BIN_UASAN  ${BINARY}_UASAN)
set(BIN_THREAD ${BINARY}_THREAD)

set(TEST_SOURCES main.cpp formula-tests.cpp)
set(LIBS ${CMAKE_PROJECT_NAME} GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  if (NOT MINGW)
    list(APPEND UASAN_LINK_FLAGS -fsanitize=undefined,address)
    list(APPEND UASAN_FLAGS -fsanitize=undefined,address
                          -fno-optimize-sibling-calls
                          -fsanitize-address-use-after-scope
                          -fno-omit-frame-pointer
                          -g -O1)
    list(APPEND THREAD_FLAGS -fsanitize=thread)
    list(APPEND THREAD_LINK_FLAGS -fsanitize=thread)
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # TODO this doesn't work on github actions but works on my windows machine
  # list(APPEND UASAN_FLAGS /fsanitize=address /Zi)
  list(APPEND UASAN_FLAGS "")
  list(APPEND UASAN_LINK_FLAGS "")
  list(APPEND THREAD_FLAGS "")
  list(APPEND THREAD_LINK_FLAGS "")
endif()

add_executable(${BIN_UASAN} ${SOURCES} ${TEST_SOURCES} ${HEADERS})
target_compile_options(${BIN_UASAN} PRIVATE ${UASAN_FLAGS} ${COMPILER_FLAGS})
target_link_options(${BIN_UASAN} PRIVATE ${UASAN_LINK_FLAGS})
target_link_libraries(${BIN_UASAN} PRIVATE ${LIBS})

add_executable(${BIN_THREAD} ${SOURCES} ${TEST_SOURCES} ${HEADERS})
target_compile_options(${BIN_THREAD} PRIVATE ${THREAD_FLAGS} ${COMPILER_FLAGS})
target_link_options(${BIN_THREAD} PRIVATE ${THREAD_LINK_FLAGS})
target_link_libraries(${BIN_THREAD} PRIVATE ${LIBS})


enable_testing()

# Add Undefined behaviour, Address sanitizer and Thread sanitizer
add_test(NAME ${BINARY}_UASAN COMMAND ${BINARY}_UASAN)
add_test(NAME ${BINARY}_THREAD COMMAND ${BINARY}_THREAD)
