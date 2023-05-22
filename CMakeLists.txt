cmake_minimum_required(VERSION 3.5)

set(CMAKE_C_COMPILER gcc)

# Definitions
set(PROJECT_NAME stxetx_protocol)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

#project(cmake_c_template)
project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wpedantic -ggdb")

# Include paths
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

# Assign the include directories
include_directories(${INCLUDE_DIRS})

file(GLOB SOURCE_FILES "src/*.c")

add_executable(stxetx_protocol ${SOURCE_FILES})