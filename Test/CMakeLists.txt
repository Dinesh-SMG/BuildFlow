cmake_minimum_required(VERSION 3.10)
project(FirmwareUnitTests C)

# Enable CTest
enable_testing()

# Add the unit test executable
add_executable(unit_Test Test/test_main.c)

# Include Test folder (for headers if you add Unity later)
target_include_directories(unit_Test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Testing/Temporary
)

# Register the test with full path
add_test(NAME unit_Test COMMAND $<TARGET_FILE:unit_Test>)
