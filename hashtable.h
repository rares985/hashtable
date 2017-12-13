#ifndef HASHTABLE_H_
#define HASHTABLE_H_

typedef unsigned int uint32_t;

typedef struct node {
	char *value;
	struct node *next;
} bucket_t;

typedef struct hashtable {
	uint32_t size;
	struct node **table;
} hashtable_t;

hashtable_t *create_hashtable(uint32_t size);
void delete_hashtable(hashtable_t *ht);

void add_value(hashtable_t *ht, char *word);
void remove_value(hashtable_t *ht, char *word);
void find_value(hashtable_t *ht, char *word, int fd);

void print_bucket(hashtable_t *ht, uint32_t index, int fd);
void print_hashtable(hashtable_t *ht, int fd);

void resize(hashtable_t *ht, double modifier);
void clear_hashtable(hashtable_t *ht);

#endif /* HASHTABLE_H_ */
