// James Simmons
// FizzBuzz practice in C, using structs and linked lists
// 2/19/2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Each node represents one word and its respective number.
// By default, "Fizz", 3; "Buzz", 5
// The nodes also form a linked list, to easily access subsequent nodes.
typedef struct node
{
	char *word;
	int divisor;
	struct node *next;
} node;

// Linked list struct to easily manage the nodes.
// Also tracks the number of nodes in the list,
// as well as the total length of all words.
typedef struct list
{
	node *head;
	node *tail;
	int size;
	int allwords;
} list;

list *initList();
void tailInsert(char *word, int divisor, list *list);
list *destroyList(list *list);
void appendMany(char *str, list *list, int index);
void appendAndPrint(char *str, list *list, int index);

int main(void)
{
	int i;
	list *list = initList();
	char *str = malloc(sizeof(char) * (list->size * list->allwords + 1));

	for (i = 0; i < 500; i++)
		appendAndPrint(str, list, i);

	list = destroyList(list);
	return 0;
}

// Initialize the list to contain whatever words we decide.
// This is the only thing we should need to edit to change which
// words and respective values govern the printed values.
list *initList()
{
	list *list = malloc(sizeof(list));
	tailInsert("The", 3, list);
	tailInsert("Quick", 5, list);
	tailInsert("Brown", 7, list);
	tailInsert("Fox", 11, list);
	tailInsert("Jumps", 13, list);
	tailInsert("Over", 17, list);
	tailInsert("Lazy", 19, list);
	tailInsert("Dog", 23, list);
	return list;
}

// Insert a new node at the tail of the list, filling both its data
// fields and increasing the relevant list metadata fields.
void tailInsert(char *word, int divisor, list *list)
{
	int wordlen = strlen(word);

	if (list == NULL)
		return;

	if (list->tail == NULL)
	{
		list->head = list->tail =  malloc(sizeof(node));
	}
	else
	{
		list->tail->next = malloc(sizeof(node));
		list->tail = list->tail->next;
	}

	list->tail->word = malloc(sizeof(char) * (wordlen + 1));
	strcpy(list->tail->word, word);

	list->tail->divisor = divisor;

	list->size++;
	list->allwords += wordlen;
}

// Iteratively deallocate all memory stored in the list.
// Always returns NULL
list *destroyList(list *list)
{
	node *temp, *save;
	if (list == NULL)
		return NULL;

	save = temp = list->head;
	while (temp != NULL)
	{
		save = temp;
		temp = temp->next;
		free(save);
	}

	list->head = list->tail = NULL;
	free(list);
	return NULL;
}

// Traverse the list, appending the "word" of a given node
// to the end of "str" if the node's "divisor" is evenly
// divisible by the index.
void appendMany(char *str, list *list, int index)
{
	node *temp;
	if (list == NULL || str == NULL)
		return;

	for (temp = list->head; temp != NULL; temp = temp->next)
		if (index % temp->divisor == 0)
			strcat(str, temp->word);
}

// Refresh "str" to the empty string, append all appropriate
// elements of the list based on the index. If "str" is still
// empty, print the index. Otherwise, print the contents of "str".
void appendAndPrint(char *str, list *list, int index)
{
	strcpy(str, "");
	appendMany(str, list, index);
	
	if (strcmp(str, "") == 0)
			printf("%d\n", index);
	else
		printf("%s\n", str);
}
