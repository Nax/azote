find_package(OpenAL REQUIRED)

file(GLOB_RECURSE SOURCES "*.c")
add_library(libazote STATIC ${SOURCES})
set_target_properties(libazote PROPERTIES OUTPUT_NAME "azote")
target_link_libraries(libazote ${OPENAL_LIBRARY})
target_include_directories(libazote PUBLIC ${OPENAL_INCLUDE_DIR})
