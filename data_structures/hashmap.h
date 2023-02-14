#define HASHTABLE_SIZE 100

typedef struct BUCKET_NODE
{
    char *str;
    char *tok_name;
    struct BUCKET_NODE *next;
} bucket_node;

typedef struct HASHTABLE_NODE
{
    int idx;
    bucket_node *bucket_ptr;
} hashtable_node;

typedef struct HASHTABLE
{
    hashtable_node *table[HASHTABLE_SIZE];
} hashtable;

hashtable initHashtable();

int hash(char *str, int length);

bool insert(hashtable *ht, char *str, int length, char *tok);

bool exists(hashtable ht, char *str, int length);

bool delete(hashtable ht, char *str, int length);

char *get(hashtable *ht, char *str, int length);