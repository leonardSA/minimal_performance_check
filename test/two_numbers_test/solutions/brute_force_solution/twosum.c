#include <stdio.h>
#include <stdlib.h>
#include "../read_data.h"

int* twoSum(int* nums, int numsSize, int target, int* returnSize);

int main(int argc, char** argv)
{
    int numsSize = atoi(argv[1]);
    int* nums = readNums(numsSize, argv[2]);
    int target = atoi(argv[3]);

    int resultSize = 0;
    int *result = twoSum(nums, numsSize, target, &resultSize);
    /* if (result == NULL) */
    /*     fprintf(stdout, "No solution found\n"); */
    /* else */
    /*     fprintf(stdout, "[%d,%d]\n", result[0], result[1]); */
    free(result);
    return 0;
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
    int *result = malloc(2*sizeof(int));

    int found = 0;
    int i = 0;
    while (i < numsSize && !found)
    {
        int locked = nums[i];
        int j = 0;
        while (j < numsSize && !found)
        {
            if (i == j)
            {
                j++;
                continue;
            }

            if (locked + nums[j] == target)
            {
                result[0] = i;
                result[1] = j;
                found = 1;
            }
            j++;
        }
        i++;
    }

    if (!found)
    {
        *returnSize = 0;
        return NULL;
    }

    *returnSize = 2;
    return result;
}
