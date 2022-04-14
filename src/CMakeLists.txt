set(SOURCES tiny-render.cpp model.cpp render-methods.cpp)
set(HEADERS tiny-render.hpp model.hpp geometry.hpp common.hpp render-methods.hpp)
add_library(${CMAKE_PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${COMPILER_FLAGS})

set(BINARY_RUN ${CMAKE_PROJECT_NAME}_RUN)
set(RUN_SOURCES main.cpp ${SOURCES})
add_executable(${BINARY_RUN} ${RUN_SOURCES} ${HEADERS})
target_link_libraries(${BINARY_RUN} PRIVATE ${CMAKE_PROJECT_NAME} tga)
target_link_libraries(${BINARY_RUN} PRIVATE range-v3 range-v3-meta range-v3::meta range-v3-concepts)
target_compile_options(${BINARY_RUN} PRIVATE ${COMPILER_FLAGS})

install(TARGETS ${BINARY_RUN})
