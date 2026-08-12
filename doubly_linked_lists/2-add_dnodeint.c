#include <stdlib.h>
#include "lists.h"

/**
 * add_dnodeint - Adds a new node at the beginning of a dlistint_t list
 * @head: Pointer to the head of the list
 * @n: Integer to store in the new node
 *
 * Return: Address of the new node, or NULL on failure
 */
dlistint_t *add_dnodeint(dlistint_t **head, const int n)
{
	dlistint_t *new;

	if (head == NULL)
		return (NULL);

	new = malloc(sizeof(dlistint_t));
	if (new == NULL)
		return (NULL);

	new->n = n;
	new->prev = NULL;
	new->next = *head;

	if (*head != NULL)
		(*head)->prev = new;

	*head = new;

	return (new);
}
