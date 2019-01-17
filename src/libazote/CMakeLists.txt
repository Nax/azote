find_package(OpenAL REQUIRED)

file(GLOB_RECURSE SOURCES "*.c")
file(GLOB_RECURSE SOURCES_WIN32 "sys/win32/*.c")

list(REMOVE_ITEM SOURCES ${SOURCES_WIN32})

if(WIN32)
    list(APPEND SOURCES ${SOURCES_WIN32})
endif()

add_library(libazote STATIC ${SOURCES})
set_target_properties(libazote PROPERTIES OUTPUT_NAME "azote")
target_link_libraries(libazote ${OPENAL_LIBRARY})
target_include_directories(libazote PUBLIC ${OPENAL_INCLUDE_DIR})
