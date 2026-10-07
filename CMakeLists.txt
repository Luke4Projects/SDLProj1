cmake_minimum_required(VERSION 3.10.0)
project(SDLProj1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_CXX_STANDARD 20)
set(GLM_INCLUDE_DIR "C:/Programming Libraries/glm")

add_subdirectory("C:/Programming Libraries/SDL" "C:/Programming Libraries/SDL-build")

add_executable(SDLProj1
main.cpp
util.cpp
camera.cpp
render.cpp
game.cpp
entity.cpp
town.cpp
runWorld.cpp
header/camera.hpp
header/util.hpp
header/render.hpp
header/game.hpp
header/entity.hpp
header/town.hpp
header/runWorld.hpp
)

target_link_libraries(SDLProj1 PRIVATE SDL3::SDL3)
target_include_directories(SDLProj1 PRIVATE ${GLM_INCLUDE_DIR})
target_include_directories(SDLProj1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/header)