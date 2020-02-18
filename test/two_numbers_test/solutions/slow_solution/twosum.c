#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../read_data.h"

/**
 * Given an array of integers, return indices of the two numbers 
 * such that they add up to a specific target.
 *
 * Calls both ntwoSum and qtwoSum.
 * ntwoSum being faster if numsSize < 10, else qtwoSum is faster.
 *
 *
 * @param nums          the array
 * @param numsSize      size of the array
 * @param target        targetted sum
 * @param returnSize    size of returned array
 * @return array containing the indexes of the numbers adding up to
 *         the target sum
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize);


/**
 * Given an array of integers, return indices of the two numbers 
 * such that they add up to a specific target.
 *
 * @param nums          the array
 * @param numsSize      size of the array
 * @param target        targetted sum
 * @param returnSize    size of returned array
 * @return array containing the indexes of the numbers adding up to
 *         the target sum
 */
int* qtwoSum(int* nums, int numsSize, int target, int* returnSize);


/**
 * Given an array of integers, return indices of the two numbers 
 * such that they add up to a specific target.
 *
 * @param nums          the array
 * @param numsSize      size of the array
 * @param target        targetted sum
 * @param returnSize    size of returned array
 * @return array containing the indexes of the numbers adding up to
 *         the target sum
 */
int* ntwoSum(int* nums, int numsSize, int target, int* returnSize);


/**
 * Recursively searches an array for a target number.
 * 
 * Very similar to quicksort.
 *
 * @param tab       the array being inspected 
 * @param lo        lower bound index
 * @param hi        higher bound index
 * @param found     set to true if target is found
 * @param target    integer that is searched for
 * @return index of target if found else -1
 */
int qfind(int* tab, int lo, int hi, int* found, int target);


/**
 * Partitions an array using a pivot.
 *
 * Searches for a target integer and returns its index and 
 * changes found to true if found.
 *
 * @param tab       the array being inspected and partitioned
 * @param lo        lower bound index
 * @param hi        higher bound index
 * @param found     set to true if target is found
 * @param target    integer that is searched for
 * @return index of target if found else index for next partition
 */
int partition(int* tab, int lo, int hi, int* found, int target);


/**
 * Swaps two integers.
 * 
 * @param a integer to swap
 * @param b integer to swap
 */
void swap(int* a, int* b);


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
    if (numsSize < 10)
        return ntwoSum(nums, numsSize, target, returnSize);
    else
        return ntwoSum(nums, numsSize, target, returnSize);
}


int* ntwoSum(int* nums, int numsSize, int target, int* returnSize)
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


int* qtwoSum(int* nums, int numsSize, int target, int* returnSize)
{
    int* result = malloc(2 * sizeof(int));
    int* mock_nums = malloc(numsSize * sizeof(int));

    int found = 0;
    int i = 0;
    while (i < numsSize && !found)
    {
        int locked = nums[i];
        int target_num = target - locked;
        int found_num = 0;
        memcpy(mock_nums, nums, numsSize * sizeof(int));

        int j = qfind(mock_nums, i, numsSize-1, &found_num, target_num);
        printf("j: %d\n", j);
        if (i == j)
        {
            printf("i: %d j: %d\n", i, j);
            i++;
            continue;
        }

        if (j != -1)
        {
            result[0] = i;
            result[1] = j;
            found = 1;
            printf("%d what about j?", j);
        }

        i++;
    }

    free(mock_nums);

    if (!found)
    {
        *returnSize = 0;
        return NULL;
    }

    *returnSize = 2;
    return result;
}


void swap(int* a, int* b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}


int qfind(int* tab, int lo, int hi, int* found, int target)
{
    if (lo < hi) 
    {
        int p = partition(tab, lo, hi, found, target);
        if (*found)
            return p;
        if (target < tab[p])
            qfind(tab, lo, p, found, target);
        else
            qfind(tab, p + 1, hi, found, target);
    }

    return -1;
}


int partition(int* tab, int lo, int hi, int* found, int target)
{
    int pivot = tab[lo + (hi - lo) / 2];
    if (pivot == target)
    {
        *found = 1;
        return (lo + (hi - lo) / 2);
    }
    while(1)
    {
        while (tab[lo] < pivot)
        {
            if (tab[lo] == target)
            {
                *found = 1;
                return lo;
            }
            ++lo;
        }

        while (tab[hi] > pivot)
        {
            if (tab[hi] == target)
            {
                *found = 1;
                return hi;
            }
            --hi;   
        }

        if (lo >= hi)
            return hi;
        
        swap(&tab[lo], &tab[hi]);
        
        ++lo;
        --hi;
    }
}

