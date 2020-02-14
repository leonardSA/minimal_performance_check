#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define MAX_ARGS 16
#define DEL_ARGS ","
#define RES_FILE "./data.dat"

void measure_time(char** argv, int x_axis);

void measure_entry(char* executable, char* entry_file, int x_axis);

char** read_entry(char* executable, char* entry);

void* function(void* vargp);

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "min_perf_check PATH_TO_EXECUTABLE ENTRIES X_AXIS_NB\n");
        exit(EXIT_FAILURE);
    }
    measure_entry(argv[1], argv[2], atoi(argv[3]));
    return EXIT_SUCCESS;
}


void measure_entry(char* executable, char* entry_file, int x_axis) {
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
        measure_time(argv, x_axis);
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

void measure_time(char** argv, int x_axis) {
    FILE* fp = fopen(RES_FILE, "a");
    if (fp == NULL) {
        fprintf(stderr, "FAILURE: Could not open %s\n", RES_FILE);
        exit(EXIT_FAILURE);
    }

    pthread_t tid;
    struct timeval before, after, elapsed;

    gettimeofday(&before, NULL);
    
    pthread_create(&tid, NULL, function, argv);
    pthread_join(tid, NULL);

    gettimeofday(&after, NULL);
    timersub(&after, &before, &elapsed);
    fprintf(fp, "%s\t%ld.%06ld\n", 
            argv[x_axis],
            (long int)elapsed.tv_sec, 
            (long int)elapsed.tv_usec);
    fclose(fp);
}

void* function(void* vargp) {
    char** argv = (char**) vargp;
    execv(argv[0], argv + 1);
}
