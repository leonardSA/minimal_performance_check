#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "measure_time.h"

#define GPLOT_CMD_FILE  "./gnuplot_cmd"
#define GPLOT_RES_FILE  "./graph.svg"
#define GPLOT_CMD_LEN   255

void gnuplot_print(struct Arguments* args);

void clean_dir();

int main(int argc, char** argv) {
    struct Arguments args = parse_arguments(argc, argv);
    clean_dir();
    measure_time(&args);
    gnuplot_print(&args);
    return EXIT_SUCCESS;
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
