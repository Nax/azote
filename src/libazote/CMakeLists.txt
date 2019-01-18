find_package(OpenAL REQUIRED)

file(GLOB_RECURSE SOURCES "*.c")
file(GLOB_RECURSE SOURCES_WIN32 "sys/win32/*.c")
file(GLOB_RECURSE SOURCES_UNIX "sys/unix/*.c")

list(REMOVE_ITEM SOURCES ${SOURCES_WIN32})
list(REMOVE_ITEM SOURCES ${SOURCES_UNIX})

if(WIN32)
    list(APPEND SOURCES ${SOURCES_WIN32})
elseif(APPLE)
    list(APPEND SOURCES ${SOURCES_UNIX})
endif()

add_library(libazote STATIC ${SOURCES})
set_target_properties(libazote PROPERTIES OUTPUT_NAME "azote")
target_link_libraries(libazote ${OPENAL_LIBRARY})
target_include_directories(libazote PUBLIC ${OPENAL_INCLUDE_DIR})
