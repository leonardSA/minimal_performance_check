#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "measure_time.h"

/* =============================
 * PRIVATE FUNCTIONS PROTOTYPES
 * ============================= */

/**
 * @brief Measures the time for one entry.
 * 
 * Measures time for an entry and writes the resulting info in a file.
 *
 * @param argv      program to execute with parameters
 * @param x_axis    column of entry file to be considered as x axis
 */
static void measure_entry(char** argv, int x_axis);

/**
 * @brief Reads an entry.
 *
 * Takes a given entry line and adds each parameter to a list.
 * This list is strings starting with the executable's path followed by 
 * the parameters and terminated with a NULL.
 *
 * @param executable    path to executable
 * @param entry         an entry line 
 *
 * @return list of strings to be executed
 */
static char** read_entry(char* executable, char* entry);

/**
 * @brief Executes a program and returns the time it took.
 *
 * Function to be executed in a thread. 
 * It will fork on itself in order to execute the program 
 * using its argument vargp and measure the time its child takes 
 * to then return it.
 *
 * @param vargp expects an argv to execute with execv.
 *
 * @return struct timeval of elapsed time
 */
static void* execute_program(void* vargp);


/* ================
 * PUBLIC FUNCTIONS
 * ================ */

void measure_time(struct Arguments* args) {
    FILE* fp = NULL;
    char* line = NULL; 
    size_t len = 0;

    fp = fopen(args->entries, "r");
    if (fp == NULL) {
        fprintf(stderr, "FAILURE: Could not open %s\n", args->entries);
        exit(EXIT_FAILURE);
    }

    while (getline(&line, &len, fp) != EOF) {
        char** argv = read_entry(args->executable, line);
        measure_entry(argv, args->x_axis_column);
        free(argv);
    }

    fclose(fp);
    if (line) free(line);
}

/* =================
 * PRIVATE FUNCTIONS
 * ================= */

static void measure_entry(char** argv, int x_axis) {
    FILE* fp = fopen(RES_FILE, "a");
    if (fp == NULL) {
        fprintf(stderr, "FAILURE: Could not open %s\n", RES_FILE);
        exit(EXIT_FAILURE);
    }

    pthread_t tid;
    void* status;
    struct timeval elapsed;

    pthread_create(&tid, NULL, execute_program, argv);
    pthread_join(tid, &status);
    elapsed = *(struct timeval*) status;

    fprintf(fp, "%s\t%ld.%06ld\n", 
            argv[x_axis],
            (long int)elapsed.tv_sec, 
            (long int)elapsed.tv_usec);
    fclose(fp);

}


static char** read_entry(char* executable, char* entry) {
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


static void* execute_program(void* vargp) {
    struct timeval before, after;
    int fd[2];
    int status = 0;
    char** argv = (char**) vargp;

    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        gettimeofday(&before, NULL);
        write(fd[1], &before, sizeof(struct timeval));
        close(fd[1]);
        execv(argv[0], argv);
        fprintf(stderr, "execv for %s failed\n", argv[0]);
    } else {
        struct timeval* elapsed = (struct timeval*) malloc(sizeof(struct timeval));
        close(fd[1]);
        wait(&status);
        gettimeofday(&after, NULL);
        read(fd[0], &before, sizeof(struct timeval));
        close(fd[0]);
        timersub(&after, &before, elapsed);
        return (void *) elapsed;
    }
}
