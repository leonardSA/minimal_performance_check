#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    long int x = atol(argv[1]);
    long int n = atol(argv[2]);
    unsigned int negative = 0;

    if (n == 0)
        return 1;

    if (n < 0) {
        negative = 1;
        n = -n;
    }

    long int result = x;
    for (int i = 1 ; i < n ; i++) {
        result *= x; 
    }

    if (negative)
        result = 1 / result;

    return EXIT_SUCCESS;
}
