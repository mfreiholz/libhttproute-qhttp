cmake_minimum_required(VERSION 3.0)
project(libhttproute-qhttp)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_AUTOMOC ON)

find_package(LibHttpRoute REQUIRED)
include_directories(${LIBHTTPROUTE_INCLUDE_DIRS})

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

find_package(QHttp REQUIRED)
include_directories(${QHTTP_INCLUDE_DIRS})

if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

add_library(
	${PROJECT_NAME}
	STATIC
	${PROJECT_NAME}/libhttproute-qhttp.h
	${PROJECT_NAME}/libhttproute-qhttp.cpp
)
target_link_libraries(
	${PROJECT_NAME}
	${LIBHTTPROUTE_LIBRARY}
	${QHTTP_LIBRARY}
	Qt5::Core
	Qt5::Network
)

# demo binary
include_directories(${PROJECT_NAME})
add_executable(
	${PROJECT_NAME}_demo
	demo/server.cpp
)
target_link_libraries(
	${PROJECT_NAME}_demo
	${PROJECT_NAME}
	${LIBHTTPROUTE_LIBRARY}
	${QHTTP_LIBRARY}
	Qt5::Core
	Qt5::Network
)