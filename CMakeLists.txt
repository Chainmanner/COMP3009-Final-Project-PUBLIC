# CMake file for our COMP3009A final project.
# TODO: See if this works on Windows. If not, make it.

cmake_minimum_required(VERSION 2.6)

if(WIN32)
	set(CMAKE_GENERATOR_PLATFORM win32)
endif(WIN32)

set(PROJ_NAME COMP3009A_Final_Project)
project(${PROJ_NAME})

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/headers)
include_directories(${PROJECT_SOURCE_DIR}/shaders)
include_directories(${PROJECT_SOURCE_DIR}/textures)

set(SRCS
	# Headers
	${PROJECT_SOURCE_DIR}/headers/constants.h
	${PROJECT_SOURCE_DIR}/headers/camera.h
	#${PROJECT_SOURCE_DIR}/headers/stb_image.h
	${PROJECT_SOURCE_DIR}/headers/utils.h
	${PROJECT_SOURCE_DIR}/headers/polyobject.h
	${PROJECT_SOURCE_DIR}/headers/baseobject.h
	# TODO: Add more here.

	# C++ Sources
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/camera.cpp
	#${PROJECT_SOURCE_DIR}/src/stb_image.cpp
	${PROJECT_SOURCE_DIR}/src/utils.cpp
	${PROJECT_SOURCE_DIR}/src/polyobject.cpp
	${PROJECT_SOURCE_DIR}/src/baseobject.cpp
	# TODO: Add more here.

	# Shaders
	${PROJECT_SOURCE_DIR}/shaders/unlitTex.vert
	${PROJECT_SOURCE_DIR}/shaders/unlitTex.frag
	${PROJECT_SOURCE_DIR}/shaders/phongTex.vert
	${PROJECT_SOURCE_DIR}/shaders/phongTex.frag
	${PROJECT_SOURCE_DIR}/shaders/phongTexShadow.vert
	${PROJECT_SOURCE_DIR}/shaders/phongTexShadow.frag
	${PROJECT_SOURCE_DIR}/shaders/particle.vert
	${PROJECT_SOURCE_DIR}/shaders/particle.geom
	${PROJECT_SOURCE_DIR}/shaders/particle.frag
	${PROJECT_SOURCE_DIR}/shaders/depth.vert
	${PROJECT_SOURCE_DIR}/shaders/depth.frag
	# TODO: Add more here.

	# Textures
	${PROJECT_SOURCE_DIR}/textures/container.jpg
	# TODO: Add more here.

	# Miscellaneous
	${PROJECT_SOURCE_DIR}/_RUN_PROJECT.bat
)

add_executable(${PROJ_NAME} ${SRCS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

set(WINLIBS ./Windows_Libs)

include_directories(${WINLIBS}/include)

if(NOT WIN32)
	find_library(GLEW_LIB GLEW)
	find_library(GLFW_LIB glfw)
	find_library(PTHREAD_LIB pthread)
elseif(WIN32)
	find_library(GLEW_LIB glew32s HINTS ${WINLIBS}/lib)
	find_library(GLFW_LIB glfw3 HINTS ${WINLIBS}/lib)
	find_library(SOIL_LIB SOIL HINTS ${WINLIBS}/lib)
endif(NOT WIN32)

target_link_libraries(${PROJ_NAME} ${GLEW_LIB})
target_link_libraries(${PROJ_NAME} ${GLFW_LIB})
if(NOT WIN32)
	target_link_libraries(${PROJ_NAME} ${PTHREAD_LIB})
elseif(WIN32)
	target_link_libraries(${PROJ_NAME} ${SOIL_LIB})
endif(NOT WIN32)

if(WIN32)
	set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
