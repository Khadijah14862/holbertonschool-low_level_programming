#include <stdlib.h>
#include <string.h>
#include "hash_tables.h"

/**
 * update_node - Updates the value of an existing node
 * @node: Node to update
 * @value: New value
 *
 * Return: 1 on success, 0 on failure
 */
static int update_node(hash_node_t *node, const char *value)
{
	char *new_value;

	new_value = strdup(value);
	if (new_value == NULL)
		return (0);

	free(node->value);
	node->value = new_value;

	return (1);
}

/**
 * create_node - Creates a new hash table node
 * @key: Key to duplicate
 * @value: Value to duplicate
 *
 * Return: Pointer to new node, or NULL on failure
 */
static hash_node_t *create_node(const char *key, const char *value)
{
	hash_node_t *new;

	new = malloc(sizeof(hash_node_t));
	if (new == NULL)
		return (NULL);

	new->key = strdup(key);
	if (new->key == NULL)
	{
		free(new);
		return (NULL);
	}

	new->value = strdup(value);
	if (new->value == NULL)
	{
		free(new->key);
		free(new);
		return (NULL);
	}

	return (new);
}

/**
 * hash_table_set - Adds or updates an element in a hash table
 * @ht: Hash table
 * @key: Key
 * @value: Value associated with the key
 *
 * Return: 1 on success, 0 on failure
 */
int hash_table_set(hash_table_t *ht, const char *key, const char *value)
{
	unsigned long int index;
	hash_node_t *node;
	hash_node_t *new;

	if (ht == NULL || key == NULL || value == NULL || key[0] == '\0')
		return (0);

	index = key_index((const unsigned char *)key, ht->size);
	node = ht->array[index];

	while (node != NULL)
	{
		if (strcmp(node->key, key) == 0)
			return (update_node(node, value));
		node = node->next;
	}

	new = create_node(key, value);
	if (new == NULL)
		return (0);

	new->next = ht->array[index];
	ht->array[index] = new;

	return (1);
}
