#include "dog.h"
#include <stdlib.h>

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
	unsigned int name_length;
	unsigned int owner_length;
	unsigned int i;

	if (name == NULL || owner == NULL)
		return (NULL);

	name_length = 0;
	while (name[name_length] != '\0')
		name_length++;

	owner_length = 0;
	while (owner[owner_length] != '\0')
		owner_length++;

	dog = malloc(sizeof(dog_t));
	if (dog == NULL)
		return (NULL);

	dog->name = malloc(sizeof(char) * (name_length + 1));
	if (dog->name == NULL)
	{
		free(dog);
		return (NULL);
	}

	dog->owner = malloc(sizeof(char) * (owner_length + 1));
	if (dog->owner == NULL)
	{
		free(dog->name);
		free(dog);
		return (NULL);
	}

	for (i = 0; i <= name_length; i++)
		dog->name[i] = name[i];

	for (i = 0; i <= owner_length; i++)
		dog->owner[i] = owner[i];

	dog->age = age;

	return (dog);
}
