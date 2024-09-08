# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name and version
project(GDSLib VERSION 1.0)

# Directories for source files, include files, etc.
set(SRC_D "${CMAKE_SOURCE_DIR}/src")
set(INC_D "${CMAKE_SOURCE_DIR}/include")
set(OBJ_D "${CMAKE_BINARY_DIR}/objects")
set(LIB_D "${CMAKE_BINARY_DIR}/lib")
set(EXAMPLE_D "${CMAKE_SOURCE_DIR}/example_files")
set(BIN_D "${CMAKE_BINARY_DIR}/bin")

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g -fPIC -Wno-unused-function")
include_directories(${INC_D})

# Source files
file(GLOB C_FILES "${SRC_D}/*.c")

# Example source files
file(GLOB EXAMPLE_FILES "${EXAMPLE_D}/*.c")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_D})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_D})

# Create the library from the object files
add_library(gdslib STATIC ${C_FILES})

# List of example programs
set(EXAMPLES stack queue pqueue linkedlist arraylist arraystack circular_array ringbuffer tree)

# Macro to simplify adding executables and linking them to the library
foreach(example IN LISTS EXAMPLES)
    add_executable(${example} ${EXAMPLE_D}/example_g${example}.c)
    target_link_libraries(${example} gdslib m)
endforeach()

# Custom target for sanitizers (test equivalent in CMake)
add_custom_target(test
    COMMAND ${CMAKE_COMMAND} -E env CFLAGS="-fsanitize=address -g3" ${CMAKE_COMMAND} --build .
)

# Valgrind run target
add_custom_target(run
    COMMAND valgrind ${BIN_D}/ring
)

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${OBJ_D} ${LIB_D} ${BIN_D}
)
