// ===========================================
// File: src/main.c
// Description: Contains the primary application logic and a simple function to be tested.
// ===========================================

#include <stdio.h>

// The function we want to unit test
int add(int a, int b) {
    return a + b;
}

int main(int argc, char *argv[]) {
    printf("Application running...\n");
    int result = add(5, 3);
    printf("Result of 5 + 3 is: %d\n", result);
    return 0;
}

