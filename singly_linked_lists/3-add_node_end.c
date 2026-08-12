#include <stdlib.h>
#include <string.h>
#include "lists.h"

/**
 * add_node_end - Adds a new node at the end of a list
 * @head: Pointer to the head of the list
 * @str: String to duplicate into the new node
 *
 * Return: Address of the new node, or NULL on failure
 */
list_t *add_node_end(list_t **head, const char *str)
{
	list_t *new;
	list_t *temp;
	unsigned int len;

	new = malloc(sizeof(list_t));
	if (new == NULL)
		return (NULL);

	new->str = strdup(str);
	if (new->str == NULL)
	{
		free(new);
		return (NULL);
	}

	len = 0;
	while (str[len] != '\0')
		len++;

	new->len = len;
	new->next = NULL;

	if (*head == NULL)
	{
		*head = new;
		return (new);
	}

	temp = *head;
	while (temp->next != NULL)
		temp = temp->next;

	temp->next = new;

	return (new);
}
