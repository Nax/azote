find_package(OpenGL REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SOURCES "*.cpp")
add_executable(azote ${SOURCES})
target_link_libraries(azote ${OPENGL_LIBRARY} libazote Qt5::Widgets)
