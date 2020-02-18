#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "parse_args.h"

/* =============================
 * PRIVATE FUNCTIONS PROTOTYPES
 * ============================= */

/**
 * @brief Validates arguments.
 *
 * Set args->status to ARGS_NOK if an error was found.
 *
 * @param args
 */
static void check_arguments(struct Arguments* args);

/**
 * @brief Verifies the entries argument.
 *
 * @param entries
 *
 * @return ARGS_NOK if error found else ARGS_OK
 */
static int check_entries(char* entries);

/**
 * @brief Verifies the executable argument.
 *
 * @param executable
 *
 * @return ARGS_NOK if error found else ARGS_OK
 */
static int check_executable(char* executable);

/**
 * @brief Verifies the x axis column argument.
 *
 * Uses the entries file to verify if given column is not out of bounds.
 *
 * @param column    given column by user
 * @param entries   entries file
 *
 * @return ARGS_NOK if error found else ARGS_OK
 */
static int check_x_axis_column(int column, char* entries);

/**
 * @brief Prints help message and exits.
 *
 * @param exec  name of executable
 */
static void print_help(char* exec);

/* ================
 * PUBLIC FUNCTIONS
 * ================ */

struct Arguments parse_arguments(int argc, char** argv) {
    struct Arguments args;

    // default
    args.executable = NULL;
    args.entries = NULL;
    args.x_axis_name = NULL;
    args.x_axis_column = 0;
    args.title = NULL;
    args.status = ARGS_OK;

    struct option options[] = {
        {"help",            no_argument,        0, 'h'},
        {"executable",      required_argument,  0, 'e'},
        {"entries",         required_argument,  0, 'v'},
        {"x-axis-column",   required_argument,  0, 'x'},
        {"x-axis-name",     required_argument,  0, 'n'},
        {"title",           required_argument,  0, 't'}
    };

    int c = 0;
    while (c != -1) {
        int option_index = 0;
        c = getopt_long(argc, argv, "he:v:x:n:t:", options, &option_index);
        switch (c) {
            case 'h':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
                break;

            case 'e':
                args.executable = (char*) malloc(sizeof(char) * MAX_ARG_LEN);
                strncpy(args.executable, optarg, MAX_ARG_LEN);
                break;

            case 'v':
                args.entries = (char*) malloc(sizeof(char) * MAX_ARG_LEN);
                strncpy(args.entries, optarg, MAX_ARG_LEN);
                break;

            case 'x':
                args.x_axis_column = atoi(optarg);
                break;

            case 'n':
                args.x_axis_name = (char*) malloc(sizeof(char) * MAX_ARG_LEN);
                strncpy(args.x_axis_name, optarg, MAX_ARG_LEN);
                break;

            case 't':
                args.title = (char*) malloc(sizeof(char) * MAX_ARG_LEN);
                strncpy(args.title, optarg, MAX_ARG_LEN);
                break;

            case '?':
                args.status = ARGS_NOK;
                break;
        }
    }

    check_arguments(&args);
    if (args.status != ARGS_OK) {
        fprintf(stderr, "ERRORS FOUND IN ARGUMENTS\n");
        exit(EXIT_FAILURE);
    }

    return args;
}

/* =================
 * PRIVATE FUNCTIONS
 * ================= */

static void check_arguments(struct Arguments* args) {
    if (check_x_axis_column(args->x_axis_column, args->entries) == ARGS_NOK) {
        fprintf(stderr, 
                "An error was found for the column number argument for one of the following reasons:\n\
                - the argument is missing\n\
                - the number given is out of bounds\n");
        args->status = ARGS_NOK;
    }
    if (check_entries(args->entries) == ARGS_NOK) {
        fprintf(stderr, 
                "An error was found for the entries argument for one the following reasons:\n\
                - the argument is missing\n\
                - the file cannot be accessed\n\
                - the file cannot be read\n");
        args->status = ARGS_NOK;
    }
    if (check_executable(args->executable) == ARGS_NOK) {
        fprintf(stderr, 
                "An error was found for the executable argument for one of the following reasons:\n\
                - the argument is missing\n\
                - the file cannot be accessed\n\
                - the file cannot be executed\n");
        args->status = ARGS_NOK;
    }
}


static int check_entries(char* entries) {
    if (entries == NULL)
        return ARGS_NOK;
    if (access(entries, R_OK) == -1)
        return ARGS_NOK;
    return ARGS_OK;
}


static int check_executable(char* executable) {
    if (executable == NULL)
        return ARGS_NOK;
    if (access(executable, X_OK) == -1)
        return ARGS_NOK;
    return ARGS_OK;
}


static int check_x_axis_column(int column, char* entries) {
    if (column == 0)
        return ARGS_NOK;

    FILE* fp = fopen(entries, "r");
    char* line = NULL;
    size_t len = 0;

    if (fp == NULL)
        return ARGS_NOK;
    if (getline(&line, &len, fp) == EOF || line == NULL)
        return ARGS_NOK;
    for (int i = 0 ; i < len ; i++) {
        if (line[i] == DEL_ARGS[0]) // Delimiter is a string
            column--;
    }
    if (column > 1)
        return ARGS_NOK;
    return ARGS_OK;
}


static void print_help(char* exec) {
    fprintf(stdout, USAGE, exec);
}
