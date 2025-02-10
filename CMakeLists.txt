cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)


project(TestProject VERSION 1.0)

# Add source files
file(GLOB_RECURSE IMGUI_SOURCES ${CMAKE_SOURCE_DIR}/include/imgui/*.cpp)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) 

# Add header files
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)  

# Add lib files
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# define the executeable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${IMGUI_SOURCES} ${GLAD_SOURCES})

# Cmake Dir with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

#------------------
# external packages
#------------------
#OpenGL
find_package(OpenGL REQUIRED)

# glad
set(GLAD_DIR ${CMAKE_SOURCE_DIR}/include/glad)
add_library(glad ${CMAKE_SOURCE_DIR}/include/glad/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)

# Put all libraries into a variable
set(LIBS glfw3 glew32 OpenGL::GL fmod_vc glad)


# Define the include DIRs
target_include_directories( ${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
)

# assimp
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/assimp)
set(LIBS ${LIBS} assimp-vc143-mt)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/")

configure_file(
    "${CMAKE_SOURCE_DIR}/include/assimp/config.h.in"
    "${CMAKE_SOURCE_DIR}/include/assimp/config.h"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Default Project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
# Setze das Arbeitsverzeichnis für Visual Studio (Debug-Modus)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")

# Setze das Arbeitsverzeichnis für Visual Studio (Release-Modus)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_RELEASE_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Release")

# Copy dlls
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dll"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/ressource"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()