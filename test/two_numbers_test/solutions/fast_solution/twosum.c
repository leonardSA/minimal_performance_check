#include <stdio.h>
#include <stdlib.h>
#include "../read_data.h"

/**
 * @brief Return indices of two ints adding up to a target in an array.
 *
 * Given an array of integers, return indices of the two numbers 
 * such that they add up to a specific target.
 *
 * @param nums              array
 * @param numsSize          size of array
 * @param target            sum target
 * @param returnSize        size of returned array
 *
 * @return  array containing indexes of ints adding up to target 
 *          if found else NULL
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize);


struct Node 
{
    int key;
    int val;
    struct Node* next;
};

struct HashMap
{
    struct Node** list;
    int length; 
    int* occupied;
};


/**
 * @brief Instantiates a node.
 *
 * @param key   value of member key
 * @param val   value of member val
 *
 * @return  new allocated node
 */
struct Node* Node_new(int key, int val);


/**
 * @brief Adds a node to a node.
 *
 * Adds a node to the last node of the chain
 * of nodes starting with the given node.
 *
 * @param node  first node
 * @param key   value of member key for new node
 * @param val   value of member val for new node
 */
void Node_add(struct Node* node, int key, int val);


/**
 * @brief Seeks a node with for the given key.
 *
 * Parses through the chain of nodes to find the node 
 * corresponding to the given key but not to the given 
 * value.
 *
 * @param node      first node of chain of node
 * @param key       key of node
 * @param not_val   value avoided
 *
 * @return  node pointer if found else NULL
 */
struct Node* Node_seek(struct Node* node, int key, int not_val);


/**
 * @brief Frees a chain of nodes.
 *
 * @param node  first node of the chain
 */
void Node_destruct(struct Node* node);


/**
 * @brief Prints a chain of nodes.
 *
 * @param node  first node of the chain
 */
void Node_print(struct Node* node);


/**
 * @brief Instantiates a hash map.
 *
 * @param length    length of hash map member list 
 *
 * @return  new allocated hash map
 */
struct HashMap* HashMap_new(int length);


/**
 * @brief Hashing function for hash map.  
 *
 * @param hm    hash map for which we hash the key
 * @param key   key to hash
 *
 * @return  hashed key 
 */
int HashMap_keygen(struct HashMap* hm, int key);


/**
 * @brief Add a new key-value pair to the hash map.
 *
 * @param hm    hash map for which we add a new pair
 * @param key   
 * @param val   
 */
void HashMap_add(struct HashMap* hm, int key, int val);


/**
 * @brief Find the value for the given key in the hash map.
 *
 * Avoid the key-value pair given by key-not_val.
 *
 * @param hm        searched hash map
 * @param key       searched key
 * @param not_val   value for which the key is invalid
 *
 * @return value for given key if found else -1
 */
int HashMap_value(struct HashMap* hm, int key, int not_val);


/**
 * @brief Frees a hash map.
 *
 * @param hm   
 */
void HashMap_destruct(struct HashMap* hm);


/**
 * @brief Print a hash map.
 *
 * @param hm    hash map to print
 */
void HashMap_print(struct HashMap* hm);


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
    struct HashMap* hm = HashMap_new(numsSize);
    int* result = malloc(2 * sizeof(int));

    for (int i = 0 ; i < numsSize ; i++)
    {
        HashMap_add(hm, nums[i], i);
    }

    for (int i = 0 ; i < numsSize ; i++)
    {
        int complement = target - nums[i];
        int j = HashMap_value(hm, complement, i);
        if (j != -1 && j != i)
        {
            HashMap_destruct(hm);
            result[0] = i;
            result[1] = j;
            *returnSize = 2;
            return result;
        }
    }

    HashMap_destruct(hm);
    free(result);
    *returnSize = 0;
    return NULL;
}


struct Node* Node_new(int key, int val)
{
    struct Node* node = malloc(sizeof(struct Node));
    node->key = key;
    node->val = val;
    node->next = NULL;
    return node;
}


void Node_add(struct Node* node, int key, int val)
{
    struct Node* n = node;
    struct Node* new_node = Node_new(key, val);
    while (n->next != NULL)
        n = n->next;
    n->next = new_node;
}


struct Node* Node_seek(struct Node* node, int key, int not_val)
{
    struct Node* n = node;
    while (n != NULL) 
    {
        if (key == n->key && n->val != not_val)
            return n;
        n = n->next;
    }
    return n;
}


void Node_destruct(struct Node* node)
{
    struct Node* n1 = node;
    struct Node* n2 = node->next;
    while (n2 != NULL)
    {
       free(n1);
       n1 = n2;
       n2 = n2->next;
    }
    free(n1);
}


void Node_print(struct Node* node)
{
    struct Node* n = node;
    while (n != NULL)
    {
        printf("%d : %d -> ", n->key, n->val);
        n = n->next;
    }
    printf("NULL\n");
}


struct HashMap* HashMap_new(int length)
{
    struct HashMap* hm = malloc(sizeof(struct HashMap));
    hm->length = length;
    hm->occupied = calloc(length, sizeof(int));
    hm->list = malloc(length * sizeof(struct Node*));
    return hm;
}


int HashMap_keygen(struct HashMap* hm, int key)
{
    int sign = 1;
    if (key < 0) sign = -1;
    return (key % hm->length) * sign;
}


void HashMap_add(struct HashMap* hm, int key, int val)
{
    int i = HashMap_keygen(hm, key);
    if (hm->occupied[i])
    {
        Node_add(hm->list[i], key, val);
    }
    else
    {
        struct Node* node = Node_new(key, val);
        hm->list[i] = node;
        hm->occupied[i] = 1;
    }
}


int HashMap_value(struct HashMap* hm, int key, int not_val)
{
    int i = HashMap_keygen(hm, key);

    if (hm->occupied[i])
    {
        struct Node* node = Node_seek(hm->list[i], key, not_val);
        if (node != NULL)
            return node->val;
    }

    return -1;
}


void HashMap_destruct(struct HashMap* hm)
{
    for (int i = 0 ; i < hm->length ; i++)
        if (hm->occupied[i])
            Node_destruct(hm->list[i]);
    free(hm->list);
    free(hm->occupied);
    free(hm);
}


void HashMap_print(struct HashMap* hm)
{
    for (int i = 0 ; i < hm->length ; i++)
        Node_print(hm->list[i]);
}
