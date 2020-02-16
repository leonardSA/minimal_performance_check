#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "parse_args.h"

#define MAX_ARGS 16
#define DEL_ARGS ","
#define RES_FILE "./data.dat"
#define GPLOT_CMD_FILE "./gnuplot_cmd"
#define GPLOT_RES_FILE "./graph.svg"
#define GPLOT_CMD_LEN   255

void measure_time(char** argv, int x_axis);

void measure_entry(struct Arguments* args);

char** read_entry(char* executable, char* entry);

void* function(void* vargp);

void gnuplot_print(struct Arguments* args);

void clean_dir();

int main(int argc, char** argv) {
    struct Arguments args = parse_arguments(argc, argv);
    clean_dir();
    measure_entry(&args);
    gnuplot_print(&args);
    return EXIT_SUCCESS;
}


void measure_entry(struct Arguments* args) {
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
        measure_time(argv, args->x_axis_column);
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
    void* status;
    struct timeval elapsed;

    pthread_create(&tid, NULL, function, argv);
    pthread_join(tid, &status);
    elapsed = *(struct timeval*) status;

    fprintf(fp, "%s\t%ld.%06ld\n", 
            argv[x_axis],
            (long int)elapsed.tv_sec, 
            (long int)elapsed.tv_usec);
    fclose(fp);
}

void* function(void* vargp) {
    struct timeval before, after;
    int fd[2];
    int status = 0;
    char** argv;

    argv = (char**) vargp;
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


void gnuplot_print(struct Arguments* args) {
    FILE* fp = fopen(GPLOT_CMD_FILE, "w");
    if (fp == NULL) {
        fprintf(stderr, "FAILURE: Could not open %s\n", RES_FILE);
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "set terminal svg\n");
    fprintf(fp, "set output \"%s\"\n", GPLOT_RES_FILE);
    fprintf(fp, "unset key\n");
    fprintf(fp, "set grid\n");
    fprintf(fp, "set ylabel \"Time in seconds\"\n");

    if (args->title != NULL)
        fprintf(fp, "set title \"%s\"\n", args->title);
    if (args->x_axis_name != NULL)
        fprintf(fp, "set xlabel \"%s\"\n", args->x_axis_name);

    fprintf(fp, "plot \"%s\" lt 7 lc 0 w l\n", RES_FILE);

    fclose(fp);

    char cmd[GPLOT_CMD_LEN] = "gnuplot ";
    strcat(cmd, GPLOT_CMD_FILE);
    system(cmd);
}

void clean_dir() {
    remove(RES_FILE);
    remove(GPLOT_CMD_FILE);
    remove(GPLOT_RES_FILE);
}
