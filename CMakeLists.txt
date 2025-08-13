cmake_minimum_required(VERSION 3.10)
project(ParkingManagement VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Source files
set(SOURCES
    src/car.cpp
    src/parking_lot.cpp
    src/main.cpp
)

# Test files
set(TEST_SOURCES
    src/car.cpp
    src/parking_lot.cpp
    src/parking_lot_test.cpp
)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)

# Main executable
add_executable(parking-system ${SOURCES})

# Test executable
add_executable(parking-test ${TEST_SOURCES})

# Compiler warnings
if(MSVC)
    target_compile_options(parking-system PRIVATE /W4)
    target_compile_options(parking-test PRIVATE /W4)
else()
    target_compile_options(parking-system PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(parking-test PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation rules
install(TARGETS parking-system parking-test
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Testing
enable_testing()
add_test(NAME ParkingTests COMMAND parking-test)

# Status messages
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Project version: ${PROJECT_VERSION}")

# Build instructions
message(STATUS "\nBuild Instructions:")
message(STATUS "1. Create build directory: mkdir build && cd build")
message(STATUS "2. Configure project: cmake ..")
message(STATUS "3. Build project: cmake --build .")
message(STATUS "4. Run main program: ./bin/parking-system")
message(STATUS "5. Run tests: ./bin/parking-test\n")