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
 * @brief Measures the time for threads entries.
 *
 * @param argvs     parameters with which to execute the programs (threads)
 * @param x_axis    column of entry file to be considered as x axis
 * @param threads   number of threads/programs to execute
 */
static void measure_entries(char*** argvs, int x_axis, int threads);

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
    char* lines[args->threads];
    char** argvs[args->threads];
    size_t len = 0;

    fp = fopen(args->entries, "r");
    if (fp == NULL) {
        fprintf(stderr, "FAILURE: Could not open %s\n", args->entries);
        exit(EXIT_FAILURE);
    }

    int done_reading = 0;
    while (!done_reading) {
        for (int i = 0 ; i < args->threads ; i++) {
            if (getline(&lines[i], &len, fp) == EOF)
                done_reading = 1;
            if (!done_reading)
                argvs[i] = read_entry(args->executable, lines[i]);
            else
                argvs[i] = NULL;
        }
        measure_entries(argvs, args->x_axis_column, args->threads);
        for (int i = 0 ; i < args->threads ; i++) {
            if (argvs[i]) free(argvs[i]);
        }
    }

    fclose(fp);
    for (int i = 0 ; i < args->threads ; i++)
        free(lines[i]);
}

/* =================
 * PRIVATE FUNCTIONS
 * ================= */

static void measure_entries(char*** argvs, int x_axis, int threads) {
    FILE* fp = fopen(RES_FILE, "a");
    if (fp == NULL) {
        fprintf(stderr, "FAILURE: Could not open %s\n", RES_FILE);
        exit(EXIT_FAILURE);
    }

    pthread_t tids[threads];
    void* status[threads]; 

    /* create threads */
    for (int i = 0 ; i < threads ; i++) {
        if (argvs[i] == NULL) continue;
        pthread_create(&(tids[i]), NULL, execute_program, argvs[i]);
    }

    /* join threads */
    for (int i = 0 ; i < threads ; i++) {
        if (argvs[i] == NULL) continue; 

        struct timeval elapsed;
        pthread_join(tids[i], &(status[i]));
        elapsed = *(struct timeval*) status[i];
        fprintf(fp, "%s\t%ld.%06ld\n", 
                argvs[i][x_axis],
                (long int)elapsed.tv_sec, 
                (long int)elapsed.tv_usec);
    }

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
        waitpid(pid, &status, WUNTRACED | WCONTINUED);
        gettimeofday(&after, NULL);
        read(fd[0], &before, sizeof(struct timeval));
        close(fd[0]);
        timersub(&after, &before, elapsed);
        return (void *) elapsed;
    }
}
