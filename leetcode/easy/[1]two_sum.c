#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int         value;
    int         key;
} HashItem;

inline int hash(int key, int size) {
    return (key % size + size) % size;
}

typedef struct {
    size_t      size;
    HashItem*   items;
} HashTable;

inline int get_bound(size_t size) {
    return -(size + 1);
}

HashTable* create_table(size_t size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->items = (HashItem*)malloc(sizeof(HashItem) * size);

    int bound = get_bound(size);
    for (int i = 0; i < size; i++) {
        table->items[i].key = bound;
        table->items[i].value = 0;
    } 

    return table;
}

void free_table(HashTable* table) {
    free(table->items);
    free(table);
}

// get hash value

// check if that spot is empty
// if empty -> put item in that index

// else linear probing
// check for next empty spot

void insert_item(HashTable* table, int key, int value) {
    int index = hash(key, table->size);

    int bound = get_bound(table->size);

    while (table->items[index].key != bound) index = (index + 1) % table->size;

    table->items[index].key = key;
    table->items[index].value = value;
}


// returns 1 if found
// returns 0 if not found

// get hash value
// track starting hash index

// loop either till key is found or hash value has reached start

int search_key(HashTable* table, int key, int* value) {
    int index = hash(key, table->size);
    int start = index;
    int bound = get_bound(table->size);

    while (table->items[index].key != bound) {
        if (table->items[index].key == key) {
            *value = table->items[index].value;
            return 1;
        }

        index = (index + 1) % table->size;
        
        if (index == start) 
            return 0;
    }

    *value = table->items[index].value;

    return 0;
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    *returnSize = 0;  
    
    if (numsSize < 2)
        return NULL; 

    HashTable* table = create_table(numsSize);
    int value = 0;

    for (int i = 0; i < numsSize; i++) {
        int complement = target - nums[i];

        if (search_key(table, complement, &value)) {
            int* result = (int*)malloc(sizeof(int) * 2);
            *returnSize = 2;

            result[0] = value;
            result[1] = i; 

            free(table);
            return result;
        } 

        insert_item(table, nums[i], i);
    }

    return NULL;
}