#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_CAPACITY 512
#define STRING_CAPACITY 255

void write_buffer(FILE* fp, char** buffer, unsigned int* size);

int main(int argc, char** argv) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s NB_DATA STEP LOW HIGH\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));

    long int nb_data = atol(argv[1]);
    long int step = atol(argv[2]);
    long int low = atol(argv[3]);
    long int high = atol(argv[4]);

    fprintf(stderr, "nb_data: %s %ld\n", argv[1], nb_data);

    char* buffer[BUFFER_CAPACITY];
    for (int i = 0 ; i < BUFFER_CAPACITY ; i++)
        buffer[i] = (char*) malloc(sizeof(STRING_CAPACITY));
    unsigned int buffer_size = 0;

    for ( ; nb_data - step > 0 ; nb_data -= step){
        FILE* fp;
        char fname[STRING_CAPACITY];

        sprintf(fname, "./%ld.dat", nb_data);
        fp = fopen(fname, "w");

        for (int j = 0 ; j < nb_data ; j++) {
            sprintf(buffer[buffer_size], "%ld", rand() % (high - low + 1) + low);
            buffer_size++;
            if (buffer_size >= BUFFER_CAPACITY - 1) {
                write_buffer(fp, buffer, &buffer_size);
            }
        }
        write_buffer(fp, buffer, &buffer_size);
    }

    return EXIT_SUCCESS;
}


void write_buffer(FILE* fp, char** buffer, unsigned int* size) {
    for (int i = 0 ; i < *size ; i++)
        fprintf(fp, "%s\n", buffer[i]);
    *size = 0;
}
