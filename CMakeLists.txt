cmake_minimum_required(VERSION 3.10)

project(TestProject VERSION 1.0)

# Add source files
file(GLOB_RECURSE IMGUI_SOURCES ${CMAKE_SOURCE_DIR}/include/imgui/*.cpp)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp) 

# Add header files
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)  

# Add lib files
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Configure assets header file
include_directories(${CMAKE_BINARY_DIR}/src)

# define the executeable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${IMGUI_SOURCES})

# Cmake Dir with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#------------------
# external packages
#------------------
#OpenGL
find_package(OpenGL REQUIRED)

# Put all libraries into a variable
set(LIBS glfw3 glew32 OpenGL::GL fmod_vc)

# Define the include DIRs
include_directories(
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/include"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Default Project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TestProject)
# Setze das Arbeitsverzeichnis für Visual Studio (Debug-Modus)
set_target_properties(TestProject PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")

# Setze das Arbeitsverzeichnis für Visual Studio (Release-Modus)
set_target_properties(TestProject PROPERTIES VS_RELEASE_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Release")

# Copy dlls
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dll"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/res"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()