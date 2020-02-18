#ifndef _READ_DATA_H_
#define _READ_DATA_H_

/**
 * @brief Reads integers from a file.
 *
 * @param nb_data       number of integers (= number of lines) in the file  
 * @param data_file     path to file 
 *
 * @return a list containing every integer in the file
 */
int* readNums(int nb_data, char* data_file) {
    int* nums = (int*) malloc(sizeof(int) * nb_data);

    FILE* fp = NULL;
    char* line = NULL; 
    size_t len = 0;
    fp = fopen(data_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "FAILURE: Could not open %s\n", data_file);
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (getline(&line, &len, fp) != EOF) {
        nums[i] = atoi(line);
        i++;
    }

    fclose(fp);
    if (line) free(line);

    return nums;
}

#endif
