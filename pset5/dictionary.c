// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = (LENGTH * 90) + 1;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Case insensitive

    int hash_value = hash(word);
    node *current_node = table[hash_value];

    while (current_node != NULL)
    {
        if (strcasecmp(current_node->word, word) == 0)
        {
            return true;
        }
        current_node = current_node->next;
    }
    // Hash word to obtain a hash value using hash function
    // Access linked list at that index in the hash table
    // Traverse linked list, looking for the word(strcasecmp: This method is going to compare words with case insensitive)
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // Input: word, with alphabetical characters and (possibly) apostrophes
    // Output: numerical index between 0 and N - 1, inclusive
    // Deterministic
    // You'll need to decide on a value of N, the number of buckets that you hash table is going to have, in other words, the length of your array, and also the possible values that your hash funcion can return
    // A larger N value means you have more buckets and your hash table and therefore, data is spread apart more and could potentially mean faster search times.
    // But you'll need to make sure that the output of your hash function is some value between 0 and n minus 1 inclusive because those will be the only valid indices into your hash table.
    // If your function ends up with a value greater than N, you can take the value % N (Mod N) to get a value in the apppropriate range

    int ascii_sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        ascii_sum += tolower(word[i]);
    }

    return ascii_sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
int total_words = 0;
bool load(const char *dictionary)
{
    // TODO
    // Open up and read from dictionary file
    // Load all this data into your hash table
    // Return true if you successfully load all the data, False if there was some sort of memory error

    // - Hash Table: array of linked lists


    // 1) Open Dictionary File using fopen and check for NULL
    FILE *dictionary_set = fopen(dictionary, "r");

    if (dictionary_set == NULL)
    {
        printf("Error occured while opening the file: %s\n", dictionary);
        return false;
    }

    char buffer[LENGTH + 1];

    // 2) Read Strings from File with fscanf(file, "%s", word) until function returns EOF
    while (fscanf(dictionary_set, "%s", buffer) != EOF)
    {
        // 3) Create a new Node by using malloc first to create a memory, check for NULL and then copy word into node using strcpy
        node *new_word = malloc(sizeof(node));
        new_word->next = NULL;

        if (new_word == NULL)
        {
            printf("Error occured during allocation of memory for a new node");
            return false;
        }

        strcpy(new_word->word, buffer);

        // 4) Hash word using the hash function this function is going to determine which index into the hash table you should use when you are inserting this new node.
        int hash_value = hash(buffer);

        // 5) Once you determined which linked list you should use, the next step is to actually insert that wod into the linked list
        if (table[hash_value] == NULL)
        {
            table[hash_value] = new_word;
        }
        else
        {
            new_word->next = table[hash_value];
            table[hash_value] = new_word;
        }
        total_words++;
    }
    fclose(dictionary_set);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return total_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *current_node = table[i];

        while (current_node != NULL)
        {
            current_node = current_node->next;
            free(table[i]);
            table[i] = current_node;
        }
    }
    return true;
}
