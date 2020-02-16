#ifndef _PARSE_ARGS_H
#define _PARSE_ARGS_H

#define MAX_ARG_LEN 255
#define ARGS_OK     0
#define ARGS_NOK    1
#define DEL_ARGS    ","

#define USAGE "\
Usage: %s [OPTION] EXECUTABLE ENTRIES X_AXIS_COLUMN\n\
\n\
Required arguments:\n\
--executable, -e STRING         Path to executable file.\n\
--entries, -v STRING            File containing the values for the executable (cvs format).\n\
--x-axis-column, -x STRING      Column number of entries (starting with 1) to be used as the x axis values.\n\
\n\
Optional arguments:\n\
--help, -h                      Shows this help message and exits.\n\
--x-axis-name, -n STRING        Names the x axis.\n\
--title, -t STRING              Gives a title to the graph.\n\
"

struct Arguments {
    char* executable;
    char* entries;
    char* x_axis_name;
    int x_axis_column;
    char* title;
    int status;
};

/**
 * @brief Parses arguments and returns them as a struct Arguments.
 *
 * Exits if arguments are misgiven.
 *
 * @param argc
 * @param argv
 *
 * @return a struct Arguments containing argument's values
 */
struct Arguments parse_arguments(int argc, char** argv);


#endif
