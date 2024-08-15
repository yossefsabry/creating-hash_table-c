#include "src/hash_table.h"
#include <stdio.h>

int main() {
    ht_hash_table *ht = ht_new(); // creating a new hash table

    ht_insert(ht, "1", "ahmed"); // inserting a new item in the hash table
    ht_insert(ht, "1", "yo");
    ht_insert(ht, "2", "foo");
    ht_insert(ht, "3", "yoi");
    ht_insert(ht, "4", "bar");
    ht_insert(ht, "5", "aya");
    ht_insert(ht, "6", "sra");
    ht_insert(ht, "7", "ooo");
    ht_insert(ht, "8", "joo");
    ht_insert(ht, "9", "soliman");

    const char* key = "7";
    char* value = ht_search(ht, key); // searching using the key
    printf("the value of the %s is : %s \n", key, value); 

    ht_delete(ht, key); // deleting using the key
    printf("** deleting the key %s **\n", key);

    ht_print(ht); // print all the item in the hash table

    ht_table_delete(ht); // starting free the memory from the hash table

    return 0;
};
