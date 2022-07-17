cmake_minimum_required(VERSION 3.12)

project(BlueDjinn)

set(GAME_TYPE "BINGO" CACHE STRING "Modo de Jogo. BINGO/SLOT/OTHER")

message("GAME_TYPE: ${GAME_TYPE}")

if(GAME_TYPE STREQUAL "BINGO")
    add_definitions(-D__GAME_TYPE__=1)
elseif(GAME_TYPE STREQUAL "SLOT")
    add_definitions(-D__GAME_TYPE__=2)
else()
    add_definitions(-D__GAME_TYPE__=0)
endif()




file(GLOB CORE_SOURCES ${CMAKE_SOURCE_DIR}/core/src/*.c ${CMAKE_SOURCE_DIR}/core/src/*.cpp)
file(GLOB CORE_HEADERS ${CMAKE_SOURCE_DIR}/core/inc/*.h ${CMAKE_SOURCE_DIR}/core/inc/*.hpp)

file(GLOB GRAPHICS_SOURCES ${CMAKE_SOURCE_DIR}/graphics/src/*.c ${CMAKE_SOURCE_DIR}/graphics/src/*.cpp)
file(GLOB GRAPHICS_HEADERS ${CMAKE_SOURCE_DIR}/graphics/inc/*.h ${CMAKE_SOURCE_DIR}/graphics/inc/*.hpp)

add_executable(BlueDjinn
                main.cpp
                ${CORE_SOURCES}
                ${CORE_HEADERS}
                ${GRAPHICS_SOURCES}
                ${GRAPHICS_HEADERS}
                #shaders/triangle_vert.glsl
                #shaders/triangle_frag.glsl
                )

#set_target_properties(BlueDjinn PROPERTIES LINKER_LANGUAGE CXX)


if(WIN32)
    target_include_directories(BlueDjinn PRIVATE #deps/glm
                                                #deps/stb
                                                #deps/glfw/include
                                                #deps/glew/include
                                                core/inc/
                                                graphics/inc/
                                                )

    #target_link_directories(BlueDjinn PRIVATE deps/glfw/lib-mingw-w64
    #                                        deps/glew/lib/Release/x64)

    #target_link_libraries(BlueDjinn PRIVATE glfw3.lib
    #                                        glew32.lib
    #                                        opengl32.lib)

    #add_custom_command(TARGET BlueDjinn POST_BUILD
    #                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/glew32.dll"
    #                COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
    #                COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/textures" "${CMAKE_BINARY_DIR}/textures"
#    )
else()
    find_package(glfw3 REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(OpenGL REQUIRED)

    target_include_directories(BlueDjinn PRIVATE deps/stb deps/glm)
    target_link_libraries(BlueDjinn GLEW::glew
                                     glfw
                                     OpenGL::OpenGL)
endif()
