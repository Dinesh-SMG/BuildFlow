# Enable testing
enable_testing()

# Unit test executable
add_executable(unit_tests
    Test/test_main.c
)

# Include Unity headers (adjust path if necessary)
target_include_directories(unit_tests PRIVATE tests)

# Register the test
add_test(NAME UnitTests COMMAND unit_tests)
