#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "parse_args.h"

void print_help(char* exec) {
    fprintf(stdout, USAGE, exec);
}

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

    if (args.status != ARGS_OK 
            || args.x_axis_column == 0 
            || args.entries == NULL
            || args.executable == NULL) {
        fprintf(stderr, "Error in arguments\n");
        exit(EXIT_FAILURE);
    }

    return args;
}
