#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char** argv) {
    struct timeval before, after, elapsed;
    gettimeofday(&before, NULL);
    sleep(1);
    gettimeofday(&after, NULL);
    timersub(&after, &before, &elapsed);
    printf("Time elapsed: %ld.%06ld\n", 
            (long int)elapsed.tv_sec, 
            (long int)elapsed.tv_usec);
    return EXIT_SUCCESS;
}
