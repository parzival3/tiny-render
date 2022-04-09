set(SOURCES formula.cpp)
set(HEADERS formula.hpp)
add_library(${CMAKE_PROJECT_NAME} STATIC ${SOURCES})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${COMPILER_FLAGS})

set(BINARY_RUN ${CMAKE_PROJECT_NAME}_RUN)
set(RUN_SOURCES main.cpp ${SOURCES})
add_executable(${BINARY_RUN} ${RUN_SOURCES} ${HEADERS})
target_link_libraries(${BINARY_RUN} PRIVATE ${CMAKE_PROJECT_NAME})
target_compile_options(${BINARY_RUN} PRIVATE ${COMPILER_FLAGS})

install(TARGETS ${BINARY_RUN})
