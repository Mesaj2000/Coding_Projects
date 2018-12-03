C implementation of the binary tree game, [Minimax](https://en.wikipedia.org/wiki/Minimax), explained [here](https://youtu.be/5oXyibEgJr0?t=76).

Demonstration of the tree structure in C.


Full Code:
```c
// James Simmons
// 12/2/2018
// Implementation of the binary tree game, "Minimax"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define NUM_LAYERS 5
#define MAXLEN 2
#define max(x,y) ((x) >= (y)) ? (x) : (y)
#define min(x,y) ((x) <= (y)) ? (x) : (y) 

typedef struct node
{
	int value;
	int layer;
	struct node* left;
	struct node* right;
} node;

int playAgain(void);
void instructions(void);
void printScore(node* current, node* top);
void getPlayerMove(node** this);
void getCompMove(node** this);
void fillTree(node* this, int layer);
void printTree(node* this);
void printPath(node* this);
void destroyTree(node* this);

int main(void)
{
	srand(time(0));
	instructions();
	
	node* top = calloc(1, sizeof(node));
	
	do
	{
		fillTree(top, 0);
		node* current = top;
		while(current->layer < NUM_LAYERS)
		{
			printTree(current);
			printf("\n");
			getPlayerMove(&current);
			if (current->layer >= NUM_LAYERS)
				break;
			getCompMove(&current);
		}
		
		printScore(current, top);
		
	} while (playAgain());
		
	printf("Thanks for playing!\n");
	destroyTree(top);
}

// Print the instructions to the game
void instructions(void)
{
	printf("Welcome to Minimax!\n");
	printf("You and the computer will take turns bisecting a set of numbers.\n");
	printf("Your goal is to finish on the largest number possible.\n");
	printf("The computer will try to finish on the lowest number possible.\n");
	printf("Good Luck!\n");
}

// Print the score: the difference between the number obtained and the best possible number
void printScore(node* current, node* top)
{
	printf("Your score was %d. (lower is better)\n", top->value - current->value);
	if (top->value - current->value == 0)
	{
		printf("Wow! A perfect score! Good job!\n");
	}
	else
	{ 
		printf("The correct path was: ");
		printPath(top);
	}
}

// Prompt the user if they would like to play again
// Return 1 if yes, 0 if no
int playAgain(void)
{
	char* input = calloc(MAXLEN, sizeof(char));
	printf("Would you like to play again? y / n\n");
	while (1)
	{
		scanf("%s", input);
		
		if (tolower(input[0]) == 'y')
			return 1;
		else if (tolower(input[0]) == 'n')
			return 0;
		else
			printf("That is not a valid response.\n");
	}
}

// Prompt the player if they would like to select the left or right side of the remaining numbers
// Only evaluate the first character of their response, since that's all you need
void getPlayerMove(node** this)
{
	char* input = calloc(MAXLEN, sizeof(char));
	printf("(L)eft or (R)ight?\n");
	while (1)
	{
		scanf("%s", input);
		
		if (tolower(input[0]) == 'l')
		{
			*this = (*this)->left;
			return;
		}
		else if (tolower(input[0]) == 'r')
		{
			*this = (*this)->right;
			return;
		}
		else
		{
			printf("That is not a valid response.\n");
		}
	}
}

// The computer makes the ideal descision for any given state
// Defaults to left if the value is identical
void getCompMove(node** this)
{
	int chooseLeft = (*this)->value == (*this)->left->value;
	printf("The computer chooses %s.\n", chooseLeft ? "left" : "right");
	*this = chooseLeft ? (*this)->left : (*this)->right;
}

// Fills the tree with random values at the bottom layer
// and the ideal descisions at each other layer
void fillTree(node* this, int layer)
{
	this->layer = layer;
	
	// Bottomost layer gets random values
	if (layer >= NUM_LAYERS)
	{
		this->value = rand() % 10;
		return;
	}
	
	// Not bottommost layer, so create more layers
	this->left = calloc(1, sizeof(node));
	fillTree(this->left, layer + 1);
	this->right = calloc(1, sizeof(node));
	fillTree(this->right, layer + 1);
	
	// Propogate up the ideal descisions
	// Odds go small, evens go big
	if (this->layer % 2)
		this->value = min(this->left->value, this->right->value);
	else
		this->value = max(this->left->value, this->right->value);
}

// Print out the entire bottom layer of the tree
void printTree(node* this)
{
	if (this->layer >= NUM_LAYERS)
	{
		printf("%d", this->value);
		return;
	}

	printTree(this->left);
	
	for (int i = 0; i < NUM_LAYERS - this->layer; i++)
		printf(" ");
	
	printTree(this->right);
}

// Print out a valid path to the equilibrium position
// Defaults left if multiple valid paths
void printPath(node* this)
{
	if (this->layer >= NUM_LAYERS)
	{
		printf("%d\n", this->value);
		return;
	}
	int chooseLeft = this->value == this->left->value;
	printf("%s ", chooseLeft ? "L" : "R");
	printPath(chooseLeft ? this->left : this->right);
}

// Free up all the memory in the tree
void destroyTree(node* this)
{	
	if (this->left != NULL)
		destroyTree(this->left);
	if (this->right != NULL)
		destroyTree(this->right);
	
	free(this);
}
```
