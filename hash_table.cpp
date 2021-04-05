/*
 Student Name:
 Date:

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {
    //Allocate memory for the new HashTableEntry struct on heap.
    HashTableEntry* myNewHashTableEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    // Initialize the components of the new HashTableEntry struct.
    myNewHashTableEntry->key = key;
    myNewHashTableEntry->value = value;
    //Set the next entry as NULL
    myNewHashTableEntry->next  =  NULL;
    return myNewHashTableEntry;
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
    //Set the bucket that we try to search
    unsigned int pos =  hashTable->hash(key);
    //Set the bucket at the pos as elements
    HashTableEntry* elements = hashTable->buckets[pos];
    //for all the elements, if the we find the key, then return the hash table entry, else return NULL
    while(elements){
        if(elements->key == key)
            return elements;
        elements = elements->next;
    }

    return NULL;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets are empty, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {
    unsigned int pos = 0;
    int numBuckets = hashTable -> num_buckets;
    while(pos < numBuckets){
        HashTableEntry* elements = hashTable->buckets[pos];
        //For all buckets,if elements are not NULL, free the elements.
        while(elements){
            HashTableEntry* nextElements = elements->next;
            void* value = elements->value;
            //if value exists, free the value
            if(value) free(elements->value);
            free(elements);
            elements = nextElements;
        }
       ++pos;
    }
    //free all the buckets and hashTable
    free(hashTable->buckets);
    free(hashTable);
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {
    HashTableEntry* elements = findItem(hashTable, key);
    // If element with same key already exists
    if(elements){
        void* oldvalue = elements->value;
        // Overwrite the key and value
        elements->value = value;
        elements->key = key;
        //return the previous value
        return oldvalue;
    }
    //if not
    else{
        unsigned int pos =  hashTable->hash(key);
        HashTableEntry* elements = hashTable->buckets[pos];
        // insert the element at the head
        hashTable->buckets[pos] = createHashTableEntry(key, value);
        // shift the other elements to the next
        hashTable->buckets[pos]->next = elements;
         return NULL;
    }
}

void* getItem(HashTable* hashTable, unsigned int key) {
    HashTableEntry* element = findItem(hashTable, key);
    //If element with the key exists, return value, else return NULL
    if(element){
        return element->value;
    }
    return NULL;
}

void* removeItem(HashTable* hashTable, unsigned int key) {
    unsigned int pos =  hashTable->hash(key);
    //Set the first element as NULL
    HashTableEntry* elements1 = NULL;
    //elements2 is the elements that we want to remove
    HashTableEntry* elements2 = hashTable->buckets[pos];
    while(elements2){
        if(elements2->key == key){
            // if the element with key locates at other than the header
            if(elements1) elements1->next = elements2->next;
            //if the element with key locates at the header
            else if(!elements1) hashTable->buckets[pos] = elements2->next;
            void* value = elements2->value;
            //remove the element
            free(elements2);
            return  value;
        }
        else{
            // Set element1 to element2, and shift element2 to next
            elements1 = elements2;
            elements2 = elements2->next;
        }
    }
    return NULL;
}

void deleteItem(HashTable* hashTable, unsigned int key) {
    // execute removeItem that will remove the item with key
    void* value = removeItem(hashTable, key);
    // if the value exists, delete the corresponding value
    if(value) free(value);
}