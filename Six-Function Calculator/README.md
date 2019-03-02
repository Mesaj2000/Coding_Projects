# Six-Function Calculator

Converts infix-notation arithmetic expression into postfix-notation, then evaluates.
Functionality for addition, subtraction, multiplication, division, exponentiation, and modular evaluation.

C implementation of the stack and queue abstract data structures.
Demonstration of the Linked List data structure.

Full Code:
```c
// James Simmons
// Practice with stacks and queues
// Conversion of infix arithmetic notation to postfix arithmetic notation
// Evaluation of postfix arithmetic notation
// 2/28/2019

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Return value for empty stacks and queues
#define EMPTY INT_MIN
#define DEBUG 1

// Linked List
// Holds either character or integer information, depending on isNum
typedef struct node
{
	int data;
	int isNum;
	struct node *next;
} node;

// Stack structure implemented via linked lists
typedef struct stack
{
	node *top;
	int size;
} stack;

// Queue structure implemented via linked lists
typedef struct queue
{
	node *front;
	node *back;
	int size;
} queue;

node *makeNode(int data, int isNum);
void printq(queue *q);
void prints(stack *s);
queue *stringToInfix(char *str);
queue *infixToPostfix(queue *infix);
int postfixToInt(queue *postfix);
int checkInfix(queue *infix);
queue *enq(queue *q, node *temp);
node *deq(queue *q);
int front(queue *q);
int frontIsNum(queue *q);
int isEmptyq(queue *q);
stack *push(stack *s, node *temp);
node *pop(stack *s);
int peek(stack *s);
int isEmpty(stack *s);
stack *destroyStack(stack *s);
queue *destroyQueue(queue *q);
int power(int a, int b);

// Take in a single command-line argument, a string
// representing an infix-notation arithmetic expression.
// Convert said string into a queue of tokens (numbers
// and symbols) in postfix-notation.
// Use postfix queue to evaluate the expression.
int main(int argc, char **argv)
{
	queue *infix = NULL;
	queue *postfix = NULL;
	int answer = 0;

	if (argc < 2)
	{
		printf("No expression given\n");
		return 0;
	}

	infix = stringToInfix(argv[1]);

	if (!checkInfix(infix))
	{
		printf("Invalid expression\n");
		infix = destroyQueue(infix);
		return 0;
	}

	postfix = infixToPostfix(infix);

	answer = postfixToInt(postfix);
	printf("%d\n", answer);

	postfix = destroyQueue(postfix);
	infix = destroyQueue(infix);
	return 0;
}

// Allocate and return a node containing data
// and whether to interpret said data as an
// integer or a character.
node *makeNode(int data, int isNum)
{
	node *temp;
	if ((temp = malloc(sizeof(node))) == NULL)
		return NULL;
	temp->data = data;
	temp->isNum = isNum;
	temp->next = NULL;
	return temp;
}

// Traverse the linked list to print the contents of the queue,
// taking care to represent integers and characters appropriately.
void printq(queue *q)
{
	node *temp;

	if (isEmptyq(q))
	{
		printf("(empty queue)\n");
		return;
	}

	for (temp = q->front; temp != NULL; temp = temp->next)
	{
		if (temp->isNum)
			printf("%d ", temp->data);
		else
			printf("%c ", temp->data);
	}

	printf("\n");
}


// Traverse the linked list to print the contents of the stack,
// taking care to represent integers and characters appropriately.
void prints(stack *s)
{
	node *temp;

	if (isEmpty(s))
	{
		printf("(empty stack)\n");
		return;
	}

	for (temp = s->top; temp != NULL; temp = temp->next)
	{
		if (temp->isNum)
			printf("%d ", temp->data);
		else
			printf("%c ", temp->data);
	}

	printf("\n");
}

// Given a string representing an infix arithmetic expression,
// allocate and return a queue representing the same expression.
queue *stringToInfix(char *str)
{
	if (DEBUG)
		printf("Started stringToInfix()\n");

	int sym, i = 0;
	node *temp = NULL;
	queue *infix = NULL;

	if (str == NULL)
		return NULL;

	// Each iteration of this loop adds one node to the queue
	for (i = 0; (sym = str[i]) != '\0'; i++)
	{
		if (DEBUG)
		{
			printf("\tstr is:   %s\n", str + i);
			printf("\tinfix is: ");
			printq(infix);
		}

		// Consecutive characters in the string may represent parts
		// of one number. Advance forward to gather all of these
		// characters before adding the full number to the queue.
		if (isdigit(sym))
		{
			temp = makeNode(sym - '0', 1);

			while (isdigit(str[i+1]))
			{
				sym = str[++i];
				temp->data = (temp->data * 10) + (sym - '0');
			}

			infix = enq(infix, temp);

			if (DEBUG)
				printf("\t\tenqueueing %d\n", temp->data);

			temp = NULL;
		}

		// Add any arithmetic symbols to the queue.
		// Ignore all other symbols
		else
		{
			switch (sym)
			{
				case '+':
				case '-':
				case '*':
				case '/':
				case '%':
				case '^':
				case '(':
				case ')':
					infix = enq(infix, makeNode(sym, 0));
					if (DEBUG)
						printf("\t\tenqueueing %c\n", sym);

					break;
				default:
					if (DEBUG)
						printf("\t\tinvalid character \'%c\'\n", sym);
			}
		}

		if (DEBUG)
			printf("\t\tadvancing str\n");
	}

	if (DEBUG)
	{
		printf("\tstr is:   %s\n", str + i);
		printf("\tinfix is: ");
		printq(infix);
	}

	return infix;
}

// Given a queue representing an infix arithmetic expression,
// allocate and return a queue representing the same expression
// in postfix notation.
queue *infixToPostfix(queue *infix)
{
	if (DEBUG)
		printf("Started infixToPostfix()\n");

 	char sym;
	node *temp = NULL;
	queue *postfix = NULL;
	stack *s = NULL;

	if (infix == NULL)
		return NULL;

	// Each iteration of this loop moves one node from the infix queue
	// to the postfix queue or to the stack, and may move one or more
	// nodes from the stack to the postfix queue.
	while (!isEmptyq(infix))
	{
		if (DEBUG)
		{
			printf("\tinfix is:   ");
			printq(infix);
			printf("\tpostfix is: ");
			printq(postfix);
			printf("\tstack is:   ");
			prints(s);
		}

		// Enqueue numbers directly into the postfix expression
		if (frontIsNum(infix))
		{
			if (DEBUG)
			{
				printf("\t\tdequeueing %d from infix\n", front(infix));
				printf("\t\tenqueueing %d to postfix\n", front(infix));
			}

			postfix = enq(postfix, deq(infix));
		}

		// When we reach a close-parenthesis, move all nodes from
		// the stack to the queue until reaching the corresponding
		// open-parenthesis, which we remove but do not enqueue.
		// Remove, also, the close-parenthesis itself from the queue
		else if (front(infix) == ')')
		{
			while (peek(s) != '(')
			{
				if (DEBUG)
				{
					printf("\t\tpopping %c from stack\n", peek(s));
					printf("\t\tenqueueing %c to postfix\n", peek(s));
				}

				postfix = enq(postfix, pop(s));
			}

			if (DEBUG)
				printf("\t\tpopping %c from stack\n", peek(s));
			free(pop(s));

			if (DEBUG)
				printf("\t\tdequeueing %c from infix\n", front(infix));
			free(deq(infix));
		}

		// Push any arithmetic symbols onto the stack, taking care to pop
		// existing symbols off if required by the rules of postfix notation.
		// Simply ignore all other characters.
		else
		{
			sym = front(infix);
			switch (sym)
			{
				case '+':
				case '-':
					while (!(peek(s) == '(' || isEmpty(s)))
					{
						if (DEBUG)
						{
							printf("\t\tpopping %c from stack\n", peek(s));
							printf("\t\tenqueueing %c to postfix\n", peek(s));
						}

						postfix = enq(postfix, pop(s));
					}

				case '*':
				case '/':
				case '%':
					while (peek(s) == '*' || peek(s) == '/' || peek(s) == '%' || peek(s) == '^')
					{
						if (DEBUG)
						{
							printf("\t\tpopping %c from stack\n", peek(s));
							printf("\t\tenqueueing %c to postfix\n", peek(s));
						}

						postfix = enq(postfix, pop(s));
					}

				case '^':
					while (peek(s) == '^')
					{
						if (DEBUG)
						{
							printf("\t\tpopping %c from stack\n", peek(s));
							printf("\t\tenqueueing %c to postfix\n", peek(s));
						}

						postfix = enq(postfix, pop(s));
					}

				case '(':
					if (DEBUG)
					{
						printf("\t\tdequeueing %c from infix\n", front(infix));
						printf("\t\tpushing %c to stack\n", front(infix));
					}

					s = push(s, deq(infix));
			}
		}
	}

	// Once we have concluded moving all numbers into the postfix queue,
	// empty out the stack into the postfix queue.
	while (!isEmpty(s))
	{
		if (DEBUG)
		{
			printf("\tinfix is:   ");
			printq(infix);
			printf("\tpostfix is: ");
			printq(postfix);
			printf("\tstack is:   ");
			prints(s);

			printf("\t\tpopping %c from stack\n", peek(s));
			printf("\t\tenqueueing %c to postfix\n", peek(s));
		}

		postfix = enq(postfix, pop(s));
	}

	if (DEBUG)
	{
		printf("\tinfix is:   ");
		printq(infix);
		printf("\tpostfix is: ");
		printq(postfix);
		printf("\tstack is:   ");
		prints(s);
	}

	s = destroyStack(s);

	return postfix;
}

// Given a queue representing an arithmetic expression
// in postfix notation, evaluate the expression.
int postfixToInt(queue *postfix)
{
	if (DEBUG)
		printf("Started postfixToInt()\n");

	int answer;
	char sym;
	node *temp, *op1, *op2;
	stack *s = NULL;

	// Each iteration of this loop processes and
	// removes one node in the queue.
	while (!isEmptyq(postfix))
	{
		if (DEBUG)
		{
			printf("\tpostfix is: ");
			printq(postfix);
			printf("\tstack is:   ");
			prints(s);
		}

		// Push all numbers onto the stack as we encounter them.
		if (frontIsNum(postfix))
		{
			if (DEBUG)
			{
				printf("\t\tdequeueing %d\n", front(postfix));
				printf("\t\tpushing %d\n", front(postfix));
			}

			s = push(s, deq(postfix));
		}

		// When we encounter a symbol: pop two numbers off the stack,
		// evaluate them according to the symbol, and push the result
		// back onto the stack.
		else
		{
			if (DEBUG)
				printf("\t\tdequeueing %c\n", front(postfix));

			temp = deq(postfix);
			sym = temp->data;
			free(temp);

			if (DEBUG)
				printf("\t\tpopping %d as op2\n", peek(s));
			op2 = pop(s);

			if (DEBUG)
				printf("\t\tpopping %d as op1\n", peek(s));
			op1 = pop(s);

			switch (sym)
			{
				case '+':
					answer = op1->data + op2->data;
					break;
				case '-':
					answer = op1->data - op2->data;
					break;
				case '*':
					answer = op1->data * op2->data;
					break;
				case '/':
					answer = op1->data / op2->data;
					break;
				case '%':
					answer = op1->data % op2->data;
					break;
				case '^':
					answer = power(op1->data, op2->data);
					break;
				default:
					answer = 0;
					printf("ERROR! ERROR! BAD SYMBOL IN postfixToInt()!\n");
			}

			if (DEBUG)
			{
				printf("\t\tevaluating %d %c %d as %d\n", op1->data, sym, op2->data, answer);
				printf("\t\tpushing %d\n", answer);
			}

			s = push(s, makeNode(answer, 1));
			free(op1);
			free(op2);
		}
	}

	if (DEBUG)
	{
		printf("\tpostfix is: ");
		printq(postfix);
		printf("\tstack is:   ");
		prints(s);
	}

	// When the evaluation is complete, there remains
	// only one number in the stack: the answer.
	temp = pop(s);
	answer = temp->data;
	free(temp);
	s = destroyStack(s);
	return answer;
}

// Return 1 if the infix expression is valid, 0 otherwise
// Modify the infix expression to add '*' between parenthesis
// E.g. " 3 ( 2 + 5 ) " = " 3 * ( 2 + 5 ) "
int checkInfix(queue *infix)
{
	if (DEBUG)
	{
		printf("Started checkInfix()\n");
		printf("\tinfix is: ");
		printq(infix);
	}

	int parenCount = 0;
	node *temp, *save;

	if (infix == NULL)
		return 0;

	temp = infix->front;

	// Expressions can only start with numbers and open parentheses
	if (!(temp->isNum == 1 || temp->data == '('))
		return 0;

	// Traverse the queue
	for (; temp != NULL; temp = temp->next)
	{
		if (temp->isNum)
		{
			// Expressions may end in numbers
			if (temp->next == NULL)
				continue;

			// Expression cannot contain consecutive numbers
			if (temp->next->isNum)
				return 0;

			// Insert '*' between numbers and parentheses
			if (temp->next->data == '(')
			{
				if (DEBUG)
					printf("\t\tinserting '*' between %d and %c\n", temp->data, temp->next->data);
				save = temp->next;
				temp->next = makeNode('*', 0);
				temp->next->next = save;
				infix->size++;
			}
		}
		else if (temp->data == ')')
		{
			// Expression cannot, at any point, have more close
			// parentheses than open parentheses
			if (--parenCount < 0)
				return 0;

			// Expressions may end in close parenthesis
			if (temp->next == NULL)
				continue;

			// Insert '*' between numbers and parentheses,
			// or between two sets of parentheses
			if (temp->next->isNum || temp->next->data == '(')
			{
				if (DEBUG)
				{
					if (temp->next->isNum)
						printf("\t\tinserting '*' between %c and %d\n", temp->data, temp->next->data);
					else
						printf("\t\tinserting '*' between %c and %c\n", temp->data, temp->next->data);
				}
				save = temp->next;
				temp->next = makeNode('*', 0);
				temp->next->next = save;
				infix->size++;
			}
		}
		else
		{
			// Expression cannot end in symbols (except close-parenthesis)
			if (temp->next == NULL)
				return 0;

			// Keep track of our number of open parentheses
			if (temp->data == '(')
				parenCount++;

			// Expression cannot contain consecutive symbols,
			// except open parentheses
			if (!(temp->next->isNum || temp->next->data == '('))
				return 0;
		}
	}


	// Expression must have the same number of open parentheses
	// as close parentheses
	if (parenCount != 0)
		return 0;

	return 1;
}

// Insert the node to the back of the queue.
// (Linked-List Tail-Insertion)
queue *enq(queue *q, node *temp)
{
	if (q == NULL)
		if ((q = calloc(1, sizeof(queue))) == NULL)
			return NULL;

	if (temp == NULL)
		return q;

	if (isEmptyq(q))
	{
		q->front = q->back = temp;
		q->size++;
		return q;
	}

	q->back->next = temp;
	q->back = temp;
	q->size++;

	return q;
}

// Retrieve and return the node at the front of the queue.
// (Linked-List Head-Deletion)
node *deq(queue *q)
{
	node *temp;

	if (isEmptyq(q))
		return NULL;

	if (q->front == q->back)
		q->back == NULL;

	temp = q->front;
	q->front = temp->next;
	temp->next = NULL;
	q->size--;

	return temp;
}

// Return the data within the node at the front of the queue
// without removing it from the queue.
int front(queue *q)
{
	return isEmptyq(q) ? EMPTY : q->front->data;
}

// Return whether the node at the front of the queue is an
// integer or a character.
int frontIsNum(queue *q)
{
	return isEmptyq(q) ? 0 : q->front->isNum;
}

// Return whether the queue contains no nodes.
int isEmptyq(queue *q)
{
	return q == NULL || q->size == 0;
}

// Insert the node on top of the stack
// (Linked-List Head-Insertion)
stack *push(stack *s, node *temp)
{
	if (s == NULL)
		if ((s = calloc(1, sizeof(stack))) == NULL)
			return NULL;

	if (temp == NULL)
		return s;

	temp->next = s->top;
	s->top = temp;
	s->size++;

	return s;
}

// Retrieve and return the node from the top of the stack.
// (Linked-List Head-Deletion)
node *pop(stack *s)
{
	node *temp;

	if (isEmpty(s))
		return NULL;

	temp = s->top;
	s->top = temp->next;
	temp->next = NULL;
	s->size--;

	return temp;
}

// Return the data within the node on the top of the stack
// without removing it from the stack.
int peek(stack *s)
{
	return isEmpty(s) ? EMPTY : s->top->data;
}

// Return whether the stack contains no nodes.
int isEmpty(stack *s)
{
	return s == NULL || s->size == 0;
}

// Deallocate all nodes within the stack,
// and the stack struct itself.
stack *destroyStack(stack *s)
{
	if (s == NULL)
		return NULL;

	while (!isEmpty(s))
		free(pop(s));

	free(s);
	return NULL;
}

// Deallocate all nodes within the queue,
// and the queue struct itself.
queue *destroyQueue(queue *q)
{
	if (q == NULL)
		return NULL;

	while (!isEmptyq(q))
		free(deq(q));

	free(q);
	return NULL;
}


// Evaluates a raised to the power of b
int power(int a, int b)
{
	int root, extra;
	if (b == 0)
		return 1;
	root = power(a, b/2);
	extra = b % 2 ? a : 1;
	return root * root * extra;
}

```
