file(GLOB_RECURSE SOURCES "*.cpp")
add_executable(azote ${SOURCES})
target_link_libraries(azote libazote)
