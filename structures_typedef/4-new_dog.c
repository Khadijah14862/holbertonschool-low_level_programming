#include "dog.h"
#include <stdlib.h>

/**
 * string_length - Returns the length of a string
 * @s: String to measure
 *
 * Return: Length of the string
 */
static unsigned int string_length(char *s)
{
	unsigned int length;

	length = 0;
	while (s[length] != '\0')
		length++;

	return (length);
}

/**
 * copy_string - Creates a copy of a string
 * @src: String to copy
 *
 * Return: Pointer to the copied string, or NULL on failure
 */
static char *copy_string(char *src)
{
	char *copy;
	unsigned int length;
	unsigned int i;

	length = string_length(src);
	copy = malloc(sizeof(char) * (length + 1));
	if (copy == NULL)
		return (NULL);

	for (i = 0; i <= length; i++)
		copy[i] = src[i];

	return (copy);
}

/**
 * new_dog - Creates a new dog
 * @name: Name of the dog
 * @age: Age of the dog
 * @owner: Owner of the dog
 *
 * Return: Pointer to the new dog, or NULL on failure
 */
dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *dog;

	if (name == NULL || owner == NULL)
		return (NULL);

	dog = malloc(sizeof(dog_t));
	if (dog == NULL)
		return (NULL);

	dog->name = copy_string(name);
	if (dog->name == NULL)
	{
		free(dog);
		return (NULL);
	}

	dog->owner = copy_string(owner);
	if (dog->owner == NULL)
	{
		free(dog->name);
		free(dog);
		return (NULL);
	}

	dog->age = age;

	return (dog);
}
