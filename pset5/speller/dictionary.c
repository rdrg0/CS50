// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <strings.h>

#include "dictionary.h"


// Copyright (c) 2011 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
// compute FNV1a hash
// originally developed by Fowler, Noll and Vo
// http://isthe.com/chongo/tech/comp/fnv/
unsigned int fnv1a(const unsigned char *text, unsigned int hash, int Prime)
{
    assert(text);
    const unsigned char *ptr = (const unsigned char *)text;
    
    while (*ptr)
    {
        hash = (*ptr++ ^ hash) * Prime;
    }
    return hash;
}

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 300000;

// Hash table
node *table[N];

//Number of words in dictionary

int words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hashnum = hash(word);
    
    int counter = 0;
    
    node *tmp = table[hashnum];
    

    while (tmp != NULL)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
        else
        {
            tmp = tmp->next;
        }
    }
    

    return false;




}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int Seed = 0x811C9DC5;
    int Prime = 0x01000193;

    unsigned char loword[strlen(word) + 1];


    for (int i = 0, j = strlen(word) ; i <= j; i++)
    {
        loword[i] = tolower(word[i]);
    }
    unsigned int x = fnv1a(loword, Seed, Prime) % N;

    return x;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    unsigned int hashnum;

    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        hashnum = hash(word);
        strcpy(n->word, word);
        words++;

        n->next = NULL;

        if (table[hashnum] == NULL)
        {
            table[hashnum] = n;

        }
        else
        {
            n->next = table[hashnum];
            table[hashnum] = n;

        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{

    return words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = table[i];
        
        while (tmp != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
