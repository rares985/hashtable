#include "hashtable.h"
#include "hash.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


hashtable_t *create_hashtable(uint32_t size)
{
    hashtable_t *ht = NULL;
    uint32_t i = 0;
	if (size < 1) {
        /* Accept only sizes greater than 1 */
		fprintf(stderr, "Invalid hashtable size\n");
		return NULL;
	}

	ht = calloc(1, sizeof(*ht));

	ht->size = size;
	ht->table = calloc(size, sizeof(*ht->table));

	for (i = 0; i < size; i++) {
		ht->table[i] = NULL;
    }

	return ht;
}

void add_value(hashtable_t *ht, char *word)
{
	uint32_t index = hash(word, ht->size);
	bucket_t *it = ht->table[index];
	bucket_t *last = it;
	bucket_t *entry = calloc(1, sizeof(*entry));

	entry->value = calloc(strlen(word) + 1, sizeof(*entry->value));
    
	strcpy(entry->value, word);
	entry->next = NULL;

	if (it == NULL) {
		ht->table[index] = entry;
	} else {
		while (it != NULL) {
			if (strcmp(it->value, word) == 0) {
				fprintf(stderr, "[ERROR]: Duplicate\n");
				return;
			}
			last = it;
			it = it->next;
		}
		last->next = entry;
	}
}

void remove_value(hashtable_t *ht, char *word)
{
	uint32_t index = hash(word, ht->size);

	bucket_t *it = ht->table[index];
	bucket_t *last = it;
	bucket_t *head = it;

	if (it == NULL) {
		fprintf(stderr, "[ERROR]: Not found\n");
		return;
	}
	if (strcmp(head->value, word) == 0) {
		ht->table[index] = head->next;
		free(head->value);
		free(head);
		return;
	}
	while (it != NULL) {
		if (strcmp(it->value, word) == 0) {
			last->next = it->next;
			free(it->value);
			free(it);
			return;
		}
		last = it;
		it = it->next;
	}
	fprintf(stderr, "Value not found");
}

void find_value(hashtable_t *ht, char *word, int fd)
{
	uint32_t index = hash(word, ht->size);
	bucket_t *it = ht->table[index];

	while (it != NULL) {
		if (strcmp(it->value, word) == 0) {
			dprintf(fd, "True\n");
			return;
		}
		it = it->next;
	}
	dprintf(fd, "False\n");
}

void resize(hashtable_t *ht, double modifier)
{
	uint32_t old_size = ht->size;
	bucket_t **old_table = ht->table;

	ht->size *= modifier;
	ht->table = calloc(ht->size, sizeof(*ht->table));

	uint32_t i;
	bucket_t *it;

	for (i = 0; i < old_size; i++) {
		it = old_table[i];
		while (it != NULL) {
			add_value(ht, it->value);
			it = it->next;
		}
	}
	bucket_t *tmp;

	for (i = 0; i < old_size; i++) {
		it = old_table[i];
		while (it != NULL) {
			tmp = it;
			it = it->next;
			free(tmp->value);
			free(tmp);
		}
		old_table[i] = NULL;
	}
	free(old_table);
	old_table = NULL;
}

void clear_hashtable(hashtable_t *ht)
{
	uint32_t i;
	bucket_t *it, *tmp;

	for (i = 0; i < ht->size; i++) {
		it = ht->table[i];
		while (it != NULL) {
			tmp = it;
			it = it->next;
			free(tmp->value);
			free(tmp);
		}
		ht->table[i] = NULL;
	}
}

void delete_hashtable(hashtable_t *ht)
{
	clear_hashtable(ht);
	free(ht->table);
	free(ht);
}

void print_bucket(hashtable_t *ht, uint32_t index, int fd)
{
	bucket_t *it = ht->table[index];

	if (it == NULL)
		return;
	dprintf(fd,"[%d]: ",index);
	while (it != NULL) {
		dprintf(fd, "%s ", it->value);
		it = it->next;
	}
	dprintf(fd, "\n");
}

void print_hashtable(hashtable_t *ht, int fd)
{
	uint32_t i;

	for (i = 0; i < ht->size; i++)
		print_bucket(ht, i, fd);
}
