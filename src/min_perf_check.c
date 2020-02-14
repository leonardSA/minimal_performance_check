#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define MAX_ARGS 16
#define DEL_ARGS ","

void measure_time(char** argv);

void measure_entry(char* executable, char* entry_file);

char** read_entry(char* executable, char* entry);

void* function(void* vargp);

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "min_perf_check PATH_TO_EXECUTABLE ENTRIES\n");
        exit(EXIT_FAILURE);
    }
    measure_entry(argv[1], argv[2]);
    return EXIT_SUCCESS;
}


void measure_entry(char* executable, char* entry_file) {
    FILE* fp = NULL;
    char* line = NULL; 
    size_t len = 0;

    fp = fopen(entry_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "FAILURE: Could not open %s\n", entry_file);
        exit(EXIT_FAILURE);
    }

    while (getline(&line, &len, fp) != EOF) {
        char** argv = read_entry(executable, line);
        measure_time(argv);
        free(argv);
    }

    fclose(fp);
    if (line) free(line);
}

char** read_entry(char* executable, char* entry) {
    char* arg = NULL;
    char** entries = (char**) malloc(sizeof(char*) * (MAX_ARGS + 1));
    entries[0] = executable;

    strtok(entry, "\n");
    int i = 1;
    for (arg = strtok(entry, DEL_ARGS) ; arg != NULL ; 
            arg = strtok(NULL, DEL_ARGS), i++) {
        entries[i] = arg;
    }

    entries[i] = NULL;
    return entries;
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
