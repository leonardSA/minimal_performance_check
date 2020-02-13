#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

void measure_time(char** argv);

void* function(void* vargp);

int main(int argc, char** argv) {
    measure_time(argv + 1);
    return EXIT_SUCCESS;
}

void measure_time(char** argv) {
    pthread_t tid;
    struct timeval before, after, elapsed;

    gettimeofday(&before, NULL);
    
    pthread_create(&tid, NULL, function, argv);
    pthread_join(tid, NULL);

    gettimeofday(&after, NULL);
    timersub(&after, &before, &elapsed);
    printf("Time elapsed: %ld.%06ld\n", 
            (long int)elapsed.tv_sec, 
            (long int)elapsed.tv_usec);
}

void* function(void* vargp) {
    char** argv = (char**) vargp;
    execv(argv[0], argv + 1);
}
