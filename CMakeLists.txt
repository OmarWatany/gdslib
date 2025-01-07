# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name and version
project(GDSLib VERSION 0.3)

# Directories for source files, include files, etc.
set(SRC_D "${CMAKE_SOURCE_DIR}/src")
set(INC_D "${CMAKE_SOURCE_DIR}/include")
set(OBJ_D "${CMAKE_BINARY_DIR}/objects")
set(LIB_D "${CMAKE_BINARY_DIR}/lib")
set(EXAMPLE_D "${CMAKE_SOURCE_DIR}/example_files")
set(BIN_D "${CMAKE_BINARY_DIR}/bin")


# Compiler flags
if(CMAKE_C_COMPILER_ID EQUAL "Clang" OR CMAKE_C_COMPILER_ID EQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g -fPIC")
endif()

include_directories(${INC_D})

# Source files
file(GLOB C_FILES "${SRC_D}/*.c")

# Example source files
file(GLOB EXAMPLE_FILES "${EXAMPLE_D}/*.c")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_D})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_D})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_D})

# Add an option to build a shared library
option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" OFF)

# Create the library from the object files
add_library(gdslib STATIC ${C_FILES})
if(BUILD_SHARED_LIBS)
    add_library(gdslib_s SHARED ${C_FILES})
    set_target_properties(gdslib_s PROPERTIES OUTPUT_NAME gdslib)
endif()


# List of example programs
set(EXAMPLES stack queue pqueue linkedlist arraylist arraystack circular_array tree ringbuffer)

# Set library version
# set_target_properties(gdslib PROPERTIES VERSION ${PROJECT_VERSION})

# Macro to simplify adding executables and linking them to the library
foreach(example IN LISTS EXAMPLES)
    add_executable(${example} ${EXAMPLE_D}/example_g${example}.c)
    target_link_libraries(${example} gdslib m)
    if(BUILD_SHARED_LIBS)
        target_link_libraries(${example} gdslib_s m)
    endif()
endforeach()

# Custom target for sanitizers (test equivalent in CMake)
add_custom_target(test
    COMMAND ${CMAKE_COMMAND} -E env CFLAGS="-fsanitize=address -g3" ${CMAKE_COMMAND} --build .
)

# Valgrind run target
add_custom_target(run
    COMMAND valgrind ${BIN_D}/ring
)

add_custom_target(malloc
    COMMAND ${CMAKE_COMMAND} -E env CFLAGS="-DBUFFER_BACKEND=BUFFER_BACKEND_LIBC_MALLOC" ${CMAKE_COMMAND} --build .
)

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${OBJ_D} ${LIB_D} ${BIN_D}
)
