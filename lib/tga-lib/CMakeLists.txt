set(SOURCES tgaimage.cpp)
set(HEADERS tgaimage.hpp)
add_library(tga STATIC ${SOURCES})
target_compile_options(tga PRIVATE ${COMPILER_FLAGS})
