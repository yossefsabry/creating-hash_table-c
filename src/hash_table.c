#include "hash_table.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "prime.h"

// global variables
#define HT_PRIME_1 151
#define HT_PRIME_2 163
#define HT_INITIAL_BASE_SIZE 6
static ht_item HT_DELETED_ITEM = {NULL, NULL};

/**
 * @brief init the item
 * @param k is the key value for the item
 * @param v is the value
 */
ht_item *ht_new_item(const char *k, const char *v) {
    ht_item *i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
};

void ht_del_item(ht_item *i) {
        free(i->key);
        free(i->value);
        free(i);
};

/**
 * @brief delete an table
 * @param ht the hash table that want ot delete
 */
void ht_table_delete(ht_hash_table *ht) {
    for (size_t i = 0; i < ht->size; i++) {
        ht_item *item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_del_item(item);
        };
    };
    free(ht->items);
    free(ht);
};

/**
 *@brief the function for hash the table item key
 *@param s the item in table
 *@param a prime number larger than the size of the alphabet > 128
 *@param m is the number for num_buckets
 */
static int ht_hash(const char *s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    };
    return (int)hash;
};

/**
 *@brief the function for double hasing
 *@param s the item in table
 *@param num_buckets is the number for num_buckets
 *@param attempt is the number of attempt
 */
static int ht_get_hash(const char *s, const int num_buckets, const int attempt) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
};

/**
 * @brief search the value of the key
 * @param ht the hash table
 * @param key the key that want to search
 * @return the value of the key
 */
void ht_insert(ht_hash_table *ht, const char *key, const char *value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70 ) {
        ht_resize_increase(ht); 
    }
    ht_item *item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item *cur_item = ht->items[index];
    int i = 1;
    while (cur_item != NULL && cur_item != &HT_DELETED_ITEM) {
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    };
    ht->items[index] = item;
    ht->count++;
};

/**
 * @brief search the value of the key
 * @param ht the hash table
 * @param key the key that want to search
 * @return the value of the key
 */
char *ht_search(ht_hash_table *ht, const char *key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM)
            if (strcmp(item->key, key) == 0)
                return item->value;
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    };
    return NULL;
};

/**
 * @brief delete the value of the key
 * @param ht the hash table
 * @param key the key that want to delete
 */
void ht_delete(ht_hash_table *ht, const char *key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10)
        ht_resize_decrease(ht);
    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht->items[index] = &HT_DELETED_ITEM;
                ht_del_item(item);
            };
        };
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    };
    ht->count--;
};

/**
  *@description the resize function for the hash
  *@param the new size for the hash table
 */
static ht_hash_table *ht_new_sized(const int base_size) {
    ht_hash_table *ht = malloc(sizeof(ht_hash_table));
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
    return ht;
};

/**
  *@description for initalzation the size for hash table first time
  *@param HT_INITIAL_BASE_SIZE the inital value for the size for the hash table
 */
ht_hash_table* ht_new() {
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

/**
 *@description for resize the hash table when there is overflow in the hash table
 *@param ht the hash table 
 *@param base_size the size for the new hash table
 */
static void ht_resize(ht_hash_table* ht, const int new_size) {

    ht_item **items = realloc(ht->items,new_size * sizeof(ht_item *));
    if (items == NULL) {
        puts("error in hte calloc size for new items for new size \n");
        return;
    }

    printf("-- the old size of the hash tiabel is: %d --\n", ht->size);
    ht->base_size = new_size;
    ht->size = new_size;
    ht->items = items;
    printf("-- success resize the hash table new size: %d --\n", ht->size);
}


/**
 *@description for resize the hash table when there is overflow in the hash table
 *@param ht the hash table 
 *@param base_size the size for the new hash table
 * HINT THIS FUNCITON IS NOT WORKING CORRECTLY PROBLEM IN THE RESIZE FUNCTION
 */
/*static void ht_resize(ht_hash_table* ht, const int base_size) {
    puts("wlcome from the reize function and yossef \n");
    ht_hash_table *new_ht = ht_new_sized(base_size);
    printf("the new size for hash table is: %d\n", new_ht->size);
    printf("the new base_size for hash table is: %d\n", new_ht->base_size);
    for (size_t i = 0; i < ht->size; i++) {
        ht_item* item =  ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM 
                && item->key != NULL && item->value != NULL) {
            printf(" the key: %s, the value: %s \n", item->key, item->value);
            ht_insert(new_ht, item->key, item->value);
            puts("adding and element for the new_ht \n");
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->base_size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_table_delete(new_ht);
    puts("finsh resize the hash table \n");
};
*/

/**
  @description for the increase the size for hash table
  @param ht the hash table
 */
void ht_resize_increase(ht_hash_table* ht) {
    const int newSize = ht->base_size * 2;
    ht_resize(ht, newSize);
}

/**
  @description for the decrease the size for hash table
  @param ht the hash table
 */
static void ht_resize_decrease(ht_hash_table* ht) {
    const int newSize = ht->base_size / 2;
    ht_resize(ht, newSize);
}


/**
  @description for the print the hash table
  @param ht the hash table
 */
void ht_print(ht_hash_table *ht) {
    for (size_t i = 0; i < ht->size; i++) {
        ht_item *item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            printf("==> the key: %s, the value: %s \n", item->key, item->value);
        }
    }
}
