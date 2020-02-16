#ifndef _MEASURE_TIME_H_
#define _MEASURE_TIME_H_

#include "parse_args.h"

#define MAX_ARGS        16
#define RES_FILE        "./data.dat"

/**
 * @brief Measures time for multiple executions of an executable.
 * 
 * Executes an executable for various parameters given by entries
 * (see Arguments structure).
 *
 * @param args
 */
void measure_time(struct Arguments* args);

#endif
