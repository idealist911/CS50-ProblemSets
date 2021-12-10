// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 2017;
unsigned int dict_size = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    unsigned int index = hash(word);

    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
/* *
 * Polynomial Hash
 * Credit: https://www.geeksforgeeks.org/
    string-hashing-using-polynomial-rolling-hash-function/
 */
unsigned int hash(const char *word)
{
    // TODO
    // Set polynomial.
    /* *
     * 31 recommended if string only contains lowercase
     * 53 recommended if string contains both cases.
     */
    int P = 31;
    unsigned int index = 0;

    for (int i = 0, len = strlen(word); i < len; i++)
    {
        index = index + (tolower(word[i]) - 65 + 1) * pow(P, i);
    }
    index = index % N;
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opens the dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Cannot open dictionary file.\n");
        return false;
    }

    // Scans dictionary and saves word into hash table
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) == 1)
    {
        unsigned int index = hash(word);
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Cannot create node.\n");
            return false;
        }
        strcpy(n->word, word);
        n->next = table[index];
        table[index] = n;
        dict_size += 1;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // // TODO
    // int count = 0;
    // // loop over dictionary array
    // for (int index = 0; index < N; index++)
    // {
    //     for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    //     {
    //         count++;
    //     }
    // }

    return dict_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // loop over dictionary array
    for (int index = 0; index < N; index++)
    {
        node *cursor = table[index];

        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}
