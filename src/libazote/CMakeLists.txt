file(GLOB_RECURSE SOURCES "*.c")
add_library(libazote STATIC ${SOURCES})
set_target_properties(libazote PROPERTIES OUTPUT_NAME "azote")
