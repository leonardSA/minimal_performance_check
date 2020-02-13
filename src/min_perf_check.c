#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

void measure_time();

void* thread_mock_function(void* vargp) { sleep(3); };

int main(int argc, char** argv) {
    measure_time();
    return EXIT_SUCCESS;
}

void measure_time() {
    pthread_t tid;
    struct timeval before, after, elapsed;

    gettimeofday(&before, NULL);
    
    pthread_create(&tid, NULL, thread_mock_function, NULL);
    pthread_join(tid, NULL);

    gettimeofday(&after, NULL);
    timersub(&after, &before, &elapsed);
    printf("Time elapsed: %ld.%06ld\n", 
            (long int)elapsed.tv_sec, 
            (long int)elapsed.tv_usec);
}
