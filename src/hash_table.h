
typedef struct {
    char *key;
    char *value;
} ht_item;

typedef struct {
    int size;
    int base_size;
    int count;
    ht_item **items;
} ht_hash_table;

ht_item *ht_new_item(const char *k, const char *v);
void ht_del_item(ht_item *i);
ht_hash_table *ht_new();
void ht_table_delete(ht_hash_table *ht);
static int ht_hash(const char *s, const int a, const int m);
static int ht_get_hash(const char *s, const int num_buckets, const int attempt);
void ht_insert(ht_hash_table *ht, const char *key, const char *value);
char *ht_search(ht_hash_table *ht, const char *key);
void ht_delete(ht_hash_table *ht, const char *key);
void ht_resize_increase(ht_hash_table* ht); 
static void ht_resize_decrease(ht_hash_table* ht);
void ht_print(ht_hash_table *ht);
